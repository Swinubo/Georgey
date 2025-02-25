#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// Initialize devices
MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD address if needed

float yaw = 0.0;
unsigned long prevTime = 0;

float speedVariable = 0.5;

int RECV_PIN = 4;  // IR receiver pin
int detectBallPin = 2;
bool hasBall = false;
int dribblerPin = 3;

// Analog sensor (LDR, etc.)
const int analogPin = A0;
int analogValue = 0;
int baseline = 0;
int average = 0;
int significantChangeVal = 0;
const int amountOfInputsForAverage = 6;

int sampleCounter = 0;
int sum = 0;

// Motor control pins (only work on pwm pins)
const int m1_clockwise = 6;
const int m1_counterclockwise = 5;
const int m2_clockwise = 9;
const int m2_counterclockwise = 10;
int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

int init_yaw = 35;

int sensorPin = A1;  // The OUT pin of the module is connected to A1
int thresholdWhite = 600;  // Adjust this based on test readings

bool detectColour= false;

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization begun!");

    Wire.begin();
    lcd.init();
    lcd.backlight();

    // Setup motor pins
    for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
        pinMode(m_pins[i], OUTPUT);
    }

    pinMode(detectBallPin, INPUT_PULLUP);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

    // Initialize MPU6050
    mpu.initialize();
    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected successfully!");
        lcd.setCursor(0, 0);
        lcd.print("MPU6050 Ready!");
    } else {
        Serial.println("MPU6050 connection failed!");
        lcd.setCursor(0, 0);
        lcd.print("MPU FAIL!");
        while (1); // Stop execution
    }

    prevTime = millis();
    baseline = initialization();
    significantChangeVal = significantChange();

    Serial.println("Initialization complete!");
    Serial.print("Baseline: ");
    Serial.println(baseline);
    Serial.print("Significant Change Value: ");
    Serial.println(significantChangeVal);
}

void loop() {
  analogValue = analogRead(analogPin);  // Read analog input
  Serial.print("Analog Value: ");
  Serial.println(analogValue);

  // Update the average reading
  int previousAverage = average;
  average = updateAverage(analogValue);

  // Update baseline dynamically
  //baseline = updateBaseline();

  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("Baseline: ");
  Serial.println(baseline);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Analog: ");
  lcd.print(detectBall());
  lcd.setCursor(0, 1);
  lcd.print("Digital: ");
  lcd.print(IrReceiver.decode());

  if (IrReceiver.decode()) {
      stop();
      delay(250);
      signed long startTime = millis();
      Serial.println("GOING FORWARD!");
      /*lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("forwards!");*/
      detectWhite();
      forward(255 * speedVariable, 255 * speedVariable);
      
      while (millis() - startTime < 1500) { //replace time with calculateDistance(baseline, average)
          Serial.println(digitalRead(detectBallPin));
          if (digitalRead(detectBallPin) == 0) {
              Serial.println("Ball detected!");
              analogWrite(dribblerPin, 255);
              hasBall = true;
              while (!(yawVal() >= 350)) {
                detectWhite();
                forward(200 * speedVariable, 255 * speedVariable);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(yawVal());
              }
              while (hasBall) {
                detectWhite();
                  forward(255 * speedVariable, 255 * speedVariable);
                  delay(100);
              }
              break;
          }
      }
  }
  IrReceiver.resume();
  
  Serial.println("GOING RIGHT!");
  right(130 * speedVariable, 130 * speedVariable);
  detectWhite();
}

// Initialization function
int initialization() {
    int turnDuration = 5000;
    unsigned long startTime = millis();
    while (millis() - startTime < turnDuration) {
        analogValue = analogRead(analogPin);
        average = updateAverage(analogValue);
        baseline = average;
    }
    return baseline;
}

// Ball detection logic
bool detectBall() {
    return (baseline - average >= significantChangeVal);
}

// Calculate significant change threshold
int significantChange() {
    return (0.0487 * baseline + 0.5);
}

// Update moving average
int updateAverage(int newValue) {
    sum += newValue;
    sampleCounter++;
    if (sampleCounter == amountOfInputsForAverage) {
        int newAverage = sum / amountOfInputsForAverage;
        sum = 0;
        sampleCounter = 0;
        return newAverage;
    }
    return average;
}

// Dynamic baseline update
int updateBaseline() {
    if (average > baseline) {
        return average;
    }
    return baseline;
}

// Read and calculate yaw
int yawVal() {
    int16_t gz;
    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;
    prevTime = currentTime;

    gz = mpu.getRotationZ();
    float gyroZ = gz / 131.0;

    yaw += gyroZ * dt;
    yaw = fmod(yaw + 360.0, 360.0);

    return yaw - 0.03;
}

// Calculate movement time
int timer() {
    int calculatedTime = (baseline - average) / significantChangeVal * 70;
    return max(calculatedTime, 100);
}

// Calculate distance using the given formula
float calculateDistance(int baseline, int average) {
    float b = (float)(baseline);
    float a = (float)(average);
    float y = (float)(baseline - average); // You can adjust y based on your sensor data or other factors
    
    // Solve for x using the equation x = (y / (1.01 * b - 42.73)) ^ (1 / -0.264)
    float x = pow((y / ((1.01 * b) - 42.73)), (1 / -0.264)) +3;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(x);

    //Convert distance to millis
    x =x*60 +50;

    if (x < 500)
    {
      x = 500;
    }

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(x);    

    return x; // Distance in cm
}

void detectWhite() {
  if (detectColour)
  {
    int sensorValue = analogRead(sensorPin);  // Read sensor value

    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);  // Print sensor value

    if (sensorValue > thresholdWhite) {
        Serial.println("Detected: Green Surface");  // Swapped labels
    } else {
        Serial.println("Detected: White Surface");
        while(sensorValue < thresholdWhite)
        {
          backwards(255, 255);
        }
    }
  }
}


// Motor control functions
void forward(int speed1, int speed2) {
    analogWrite(m2_counterclockwise, speed2);
    analogWrite(m1_counterclockwise, speed1);
    digitalWrite(m1_clockwise, LOW);
    digitalWrite(m2_clockwise, LOW);
}

void backwards(int speed1, int speed2) {
    analogWrite(m1_clockwise, speed1);
    analogWrite(m2_clockwise, speed2);
    digitalWrite(m1_counterclockwise, LOW);
    digitalWrite(m2_counterclockwise, LOW);
}

void right(int speed1, int speed2) {
    analogWrite(m1_clockwise, speed1);
    analogWrite(m2_counterclockwise, speed2);
    digitalWrite(m1_counterclockwise, LOW);
    digitalWrite(m2_clockwise, LOW);
}

void left(int speed1, int speed2) {
    analogWrite(m1_counterclockwise, speed1);
    analogWrite(m2_clockwise, speed2);
    digitalWrite(m1_clockwise, LOW);
    digitalWrite(m2_counterclockwise, LOW);
}

void stop() {
    digitalWrite(m1_counterclockwise, LOW);
    digitalWrite(m2_counterclockwise, LOW);
    digitalWrite(m1_clockwise, LOW);
    digitalWrite(m2_clockwise, LOW);
}
