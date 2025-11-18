 #include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// Initialize devices
MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD address if needed

float yaw = 0.0;
unsigned long prevTime = 0;

float speedVariable = 0.6; //on new 9v use 0.5

int RECV_PIN = 7;  // IR receiver pin
int detectBallPin = 2;
bool hasBall = false;
int dribblerPin = 3;

// Motor control pins (only work on pwm pins)
const int m1_clockwise = 6;
const int m1_counterclockwise = 5;
const int m2_clockwise = 9;
const int m2_counterclockwise = 10;
int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

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

    pinMode(dribblerPin, OUTPUT);

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
    lcd.clear();
}

void loop() {
  yawVal();
  
  lcd.setCursor(0, 0);
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
              digitalWrite(dribblerPin, HIGH);
              hasBall = true;
              lcd.clear();
              while (!((yawVal() >= 355 && yawVal() <= 360) || (yawVal() >= 0 && yawVal() <= 5))) {
                detectWhite();
                right(200*speedVariable, 200*speedVariable);
                lcd.setCursor(0, 0);
                lcd.print("Yaw: ");
                lcd.print(yawVal());
                Serial.print("Yaw: ");
                Serial.println(yawVal());
                if (checkBallLost())
                {
                  Serial.println("Checked ball false!");
                  hasBall = false;
                  analogWrite(dribblerPin, 0);
                  break;
                }
              }
              while (hasBall) {
                analogWrite(dribblerPin, 0);
                detectWhite();
                forward(255 * speedVariable, 255 * speedVariable);
                delay(100);
                if (checkBallLost())
                {
                  hasBall = false;
                }
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

bool checkBallLost() {
  static unsigned long ballLostStartTime = 0; // Stores the time when pin first detects 1
  if (digitalRead(detectBallPin) == 1) {
    if (ballLostStartTime == 0) {
      ballLostStartTime = millis(); // Start the timer when pin goes HIGH
    }

    // Check if 3 seconds (3000 ms) have passed
    if (millis() - ballLostStartTime >= 3000) {
      ballLostStartTime = 0; // Reset timer
      return true; // Ball is officially lost
    }
  } else {
    ballLostStartTime = 0; // Reset timer if pin goes back to 0
  }
  return false;
}

void detectWhite() {
  if (detectColour)
  {
    int sensorValue = analogRead(sensorPin);  // Read sensor value

    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);  // Print sensor value

    if (sensorValue > thresholdWhite) {
        Serial.println("Detected: Green Surface");
    } else {
        Serial.println("Detected: White Surface");
        backwards(255, 255);
        delay(500);
    }
  }
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

    return yaw - 0.2;
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