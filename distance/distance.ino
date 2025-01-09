#include <IRremote.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float yaw = 0.0; // Yaw angle in degrees
unsigned long prevTime = 0;

float speedVariable = 0.4;

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected
int detectBallPin = 3;
bool hasBall = false;

// Pin where the analog device is connected
const int analogPin = A0;
int analogValue = 0;               // Variable to store the analog reading
int baseline = 0;                  // Variable to store the baseline IR light reading
int average = 0;                   // Variable to store the calculated average
const int amountOfInputsForAverage = 6;  // Number of inputs to calculate average

int sampleCounter = 0;             // Counter to track the number of samples
int sum = 0;                       // Variable to store the sum of the readings

void setup() {
  Serial.begin(9600);  // Start serial communication for monitoring
  Serial.println("Initialization begun!");

  pinMode(detectBallPin, INPUT_PULLUP);

  baseline = initialization();  // Set baseline
  Serial.print("Baseline: ");
  Serial.println(baseline);

  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  prevTime = millis(); // Initialize time
}

void loop() {
  analogValue = analogRead(analogPin);  // Read analog input
  average = updateAverage(analogValue);  // Update the rolling average
  baseline = updateBaseline();  // Dynamically update the baseline

  // Calculate the IR light intensity change (y) and the distance (x)
  int y = baseline - average;
  float distance = calculateDistance(y, baseline);

  // Print the results
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("Average: ");
  Serial.println(average);
  Serial.print("IR Light Change (y): ");
  Serial.println(y);
  Serial.print("Estimated Distance (x): ");
  Serial.println(distance);

  delay(200); // Small delay to prevent flooding the serial monitor
}

// Initialization function to set the baseline
int initialization() {
  int turnDuration = 5000;  // Time (ms) to perform the turn
  unsigned long startTime = millis();
  int tempBaseline = 0;
  int samples = 0;

  while (millis() - startTime < turnDuration) {
    analogValue = analogRead(analogPin);
    tempBaseline += analogValue;
    samples++;
  }

  return tempBaseline / samples;  // Return the average as the baseline
}

// Function to calculate distance based on the inverse square root function
float calculateDistance(int y, int b) {
  if (y <= 0) return 0; // Prevent invalid calculations

  float x = pow((float)y / (0.9513 * b), -2.746);
  return x;
}

// Rolling average calculation
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

// Dynamically update the baseline
int updateBaseline() {
  if (average > baseline) {
    return average;
  }
  return baseline;
}
