#include <IRremote.h>

int RECV_PIN1 = 4;  // Pin for the first IR receiver
int RECV_PIN2 = 2;  // Pin for the second IR receiver

int detectBallPin = 3;
bool hasBall = false;

// Pins for analog devices
const int analogPin1 = A0;  // First analog sensor
const int analogPin2 = A1;  // Second analog sensor
int analogValue1 = 0;
int analogValue2 = 0;
int baseline = 0;  // Single baseline for both sensors
int average1 = 0;
int average2 = 0;
int significantChangeVal = 0;
const int amountOfInputsForAverage = 6;  // Number of inputs for average calculation

// Motor pins
const int m1_clockwise = 6;          // Left motor
const int m1_counterclockwise = 5;   // Left motor reverse
const int m2_clockwise = 8;          // Right motor
const int m2_counterclockwise = 7;   // Right motor reverse

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {
  Serial.begin(9600);  // Start serial communication for monitoring
  Serial.println("Initialization begun!");

  pinMode(detectBallPin, INPUT_PULLUP);

  // Initialize pins for motors
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
    pinMode(m_pins[i], OUTPUT);
  }

  IrReceiver.begin(RECV_PIN1, ENABLE_LED_FEEDBACK);  // Start the first IR receiver
  IrReceiver.begin(RECV_PIN2, ENABLE_LED_FEEDBACK);  // Start the second IR receiver

  baseline = initialization();  // Set a single baseline for both sensors
  significantChangeVal = significantChange();

  Serial.println("Initialization complete!");
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("Significant Change Value: ");
  Serial.println(significantChangeVal);
}

void loop() {
  analogValue1 = analogRead(analogPin1);  // Read from the first analog pin
  analogValue2 = analogRead(analogPin2);  // Read from the second analog pin

  // Update the averages
  average1 = updateAverage1(analogValue1);
  average2 = updateAverage2(analogValue2);

  Serial.print("Analog Value1: ");
  Serial.println(analogValue1);
  Serial.print("Analog Value2: ");
  Serial.println(analogValue2);
  Serial.print("Average1: ");
  Serial.println(average1);
  Serial.print("Average2: ");
  Serial.println(average2);
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("Analog front: ");
  Serial.println(detectBall(average1));
  Serial.print("Analog back: ");
  Serial.println(detectBall(average2));
  Serial.print("Digital: ");
  Serial.println(IrReceiver.decode());

  if ((detectBall(average1) || detectBall(average2)) && (IrReceiver.decode())) {
    if (detectBall(average2)) {
      right(255);
      delay(600);
    }
    unsigned long startTime = millis();
    Serial.println("GOING FORWARD!");
    forward(255);
    while (millis() - startTime < 750) {
      if (digitalRead(detectBallPin) == 0) {
        Serial.println("Ball detected! Stopping...");
        hasBall = true;
        break;
      }
    }
    forward(140);
    while (hasBall) {
      Serial.println("Going forward slowly...");
    }
  }
  IrReceiver.resume();  // Prepare to receive the next signal

  Serial.println("GOING RIGHT!");
  right(100);
}

int initialization() {
  int turnDuration = 2500;  // Time (ms) to perform the turn
  unsigned long startTime = millis();

  while (millis() - startTime < turnDuration) {
    analogValue1 = analogRead(analogPin1);
    average1 = updateAverage1(analogValue1);
    baseline = average1;  // Keep updating baseline
  }

  unsigned long startTime = millis();
  while (millis() - startTime < turnDuration) {
    analogValue1 = analogRead(analogPin1);
    average1 = updateAverage1(analogValue1);
    baseline = average1;  // Keep updating baseline
  }
  return baseline;
}

bool detectBall(int average) {
  return (baseline - average >= significantChangeVal);
}

int significantChange() {
  // Calculate a threshold for significant change based on the baseline
  return (0.0487 * baseline + 0.5);  // 0.5 added for rounding
}

int updateAverage1(int newValue1) {
  static int sum1 = 0;
  static int sampleCounter1 = 0;

  sum1 += newValue1;
  sampleCounter1++;

  if (sampleCounter1 == amountOfInputsForAverage) {
    int newAverage1 = sum1 / amountOfInputsForAverage;
    sum1 = 0;
    sampleCounter1 = 0;
    return newAverage1;
  }

  return average1;
}

int updateAverage2(int newValue2) {
  static int sum2 = 0;
  static int sampleCounter2 = 0;

  sum2 += newValue2;
  sampleCounter2++;

  if (sampleCounter2 == amountOfInputsForAverage) {
    int newAverage2 = sum2 / amountOfInputsForAverage;
    sum2 = 0;
    sampleCounter2 = 0;
    return newAverage2;
  }

  return average2;
}

void forward(int speed) {
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}

void backwards(int speed) {
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}

void right(int speed) {
  delay(500);
  digitalWrite(m1_counterclockwise, LOW);
  analogWrite(m2_counterclockwise, speed);
  analogWrite(m1_clockwise, speed);
  digitalWrite(m2_clockwise, LOW);
}

void left(int speed) {
  analogWrite(m1_counterclockwise, speed);
  digitalWrite(m2_counterclockwise, LOW);
  digitalWrite(m1_clockwise, LOW);
  analogWrite(m2_clockwise, speed);
}

void stop() {
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}
