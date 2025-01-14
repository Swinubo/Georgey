#include <IRremote.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float yaw = 0.0; // Yaw angle in degrees
unsigned long prevTime = 0;

float speedVariable = 1;

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected

int detectBallPin = 3;
bool hasBall = false;

// Pin where the analog device is connected
const int analogPin = A0;
int analogValue = 0;               // Variable to store the analog reading
int baseline = 0;                  // Variable to store the baseline darkness reading
int average = 0;                   // Variable to store the calculated average
int significantChangeVal = 0;      // Variable to store the change required to detect a ball
const int amountOfInputsForAverage = 6;  // Number of inputs to calculate average

int sampleCounter = 0;             // Counter to track the number of samples
int sum = 0;                       // Variable to store the sum of the readings

// Motor pins
const int m1_clockwise = 6;          // left motor
const int m1_counterclockwise = 5;   // left motor reverse
const int m2_clockwise = 8;          // right motor
const int m2_counterclockwise = 7;   // right motor reverse

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

int init_yaw = 35;

void setup() {
  Serial.begin(9600);  // Start serial communication for monitoring
  Serial.println("Initialization begun!");

  pinMode(detectBallPin, INPUT_PULLUP);

  // Initialize pins for motors
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
    pinMode(m_pins[i], OUTPUT);
  }

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver

  baseline = initialization();  // Set baseline
  significantChangeVal = significantChange();  // Calculate significant change value

  Serial.println("Initialization complete!");
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("Significant Change Value: ");
  Serial.println(significantChangeVal);

  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  prevTime = millis(); // Initialize time

  // Stabilize and calculate initial yaw
  float tempYaw = 0.0;
  for (int i = 0; i < 10; i++) { // Take 10 readings to stabilize
    tempYaw += yawVal();
    delay(50); // Small delay between readings
  }
  init_yaw = tempYaw / 10.0; // Calculate average
  Serial.print("Initial Yaw: ");
  Serial.println(init_yaw);
}

void loop() {
  analogValue = analogRead(analogPin);  // Read analog input
  Serial.print("Analog Value: ");
  Serial.println(analogValue);

  // Update the average reading
  int previousAverage = average;
  average = updateAverage(analogValue);

  // Update baseline dynamically
  baseline = updateBaseline();

  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("Baseline: ");
  Serial.println(baseline);

  if (detectBall() && IrReceiver.decode()) {
    stop();
    delay(250);
    signed long startTime = millis();
    Serial.println("GOING FORWARD!");
    forward(255 * speedVariable, 255 * speedVariable);
    while (millis() - startTime < 1500) {
      Serial.println(digitalRead(3));
      if (digitalRead(3) == 0) {
        Serial.println("SWITCH IS ON ON ONON ONONONONOONONONONONOONONONONON");
        hasBall = true;
        Serial.print("Before while!");
        while (yawVal() < 350 || (yawVal() < 0 && yawVal() > 10)) {
          Serial.print("Yaw: ");
          Serial.println(yawVal());
          Serial.print("init_yaw: ");
          Serial.println(init_yaw);
          forward(90 * speedVariable, 120 * speedVariable);
        }
        Serial.println(hasBall);
        while (hasBall) {
          forward(255 * speedVariable, 255 * speedVariable); // Peter's y = x^3 can be introduced here
          Serial.println("Moving forward with the ball!");
          delay(100); // Small delay to prevent flooding the serial monitor
        }
        break;
      }
    }
  }
  IrReceiver.resume();  // Prepare to receive the next signal

  Serial.println("GOING RIGHT!");
  right(100 * speedVariable, 100 * speedVariable);
}

int initialization() {
  int turnDuration = 5000;  // Time (ms) to perform the turn
  unsigned long startTime = millis();

  while (millis() - startTime < turnDuration) {
    analogValue = analogRead(analogPin);
    average = updateAverage(analogValue);
    baseline = average;  // Keep updating baseline
  }
  return baseline;
}

bool detectBall() {
  return (baseline - average >= significantChangeVal);
}

int significantChange() {
  // Calculate a threshold for significant change based on the baseline
  return (0.0487 * baseline + 0.5);  // 0.5 added for rounding
}

int updateAverage(int newValue) {
  // Add the new value to the sum
  sum += newValue;
  sampleCounter++;

  // If we've collected enough samples, calculate the average and reset
  if (sampleCounter == amountOfInputsForAverage) {
    int newAverage = sum / amountOfInputsForAverage;
    // Reset sum and counter for the next average calculation
    sum = 0;
    sampleCounter = 0;
    return newAverage;
  }

  // If not enough samples yet, keep the current average
  return average;
}

int updateBaseline() {
  // Update the baseline if the current average is greater
  if (average > baseline) {
    return average;
  }
  return baseline;
}

int yawVal() {
  int16_t gz; // Gyroscope Z-axis value

  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // Calculate delta time in seconds
  prevTime = currentTime;

  // Get gyroscope Z-axis data directly
  gz = mpu.getRotationZ();
  float gyroZ = gz / 131.0; // Convert raw value to degrees/second

  // Integrate gyroscope data to estimate yaw
  yaw += gyroZ * dt;

  // Normalize yaw angle to 0-360 degrees
  yaw = fmod(yaw + 360.0, 360.0);

  return yaw - 0.03;
}

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
