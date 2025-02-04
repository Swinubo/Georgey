#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD address if needed

const int analogPin = A0;  // Analog sensor pin
const int numSamples = 10; // Number of samples for averaging
int baseline = 0;          // Baseline for IR sensor readings
int analogValue = 0;       // Current analog sensor value
int average = 0;           // Rolling average of sensor readings

void setup() {
  Serial.begin(9600);  // Start serial communication
  Serial.println("Initializing system...");

  lcd.init();           // Initialize LCD
  lcd.backlight();      // Turn on the backlight

  baseline = initializeBaseline();  // Establish a baseline
  Serial.print("Baseline initialized: ");
  Serial.println(baseline);
}

void loop() {
  // Read and process the sensor input
  analogValue = readMultipleSamples(analogPin, numSamples);  // Averaged reading
  average = updateRollingAverage(analogValue);               // Rolling average
  baseline = updateDynamicBaseline(average, baseline);       // Dynamic baseline adjustment

  // Calculate the IR light change and estimated distance
  int y = baseline - average;  // IR light intensity change
  float distance = calculateDistance(y, baseline);

  // Print debug information (optional, you can comment this out)
  Serial.print("Analog Value: ");
  Serial.println(analogValue);
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("IR Light Change (y): ");
  Serial.println(y);
  Serial.print("Estimated Distance: ");
  Serial.println(distance);
  Serial.println();

  // Display the distance on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");

  delay(200);  // Delay for stability
}

// Function to initialize the baseline
int initializeBaseline() {
  int tempBaseline = 0;
  for (int i = 0; i < numSamples * 10; i++) { // Collect more samples for stability
    tempBaseline += analogRead(analogPin);
    delay(10);
  }
  return tempBaseline / (numSamples * 10);  // Return average as the baseline
}

// Function to read multiple samples and calculate the average
int readMultipleSamples(int pin, int samples) {
  int total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delayMicroseconds(500);  // Short delay between readings
  }
  return total / samples;  // Return average value
}

// Function to update the rolling average
int updateRollingAverage(int newValue) {
  static int rollingSum = 0;
  static int sampleCount = 0;
  static const int rollingWindow = 6;  // Size of the rolling window

  rollingSum += newValue;
  sampleCount++;

  if (sampleCount >= rollingWindow) {
    int rollingAverage = rollingSum / rollingWindow;
    rollingSum = 0;
    sampleCount = 0;
    return rollingAverage;
  }
  return average;  // Return the last calculated average if window isn't full
}

// Function to dynamically update the baseline
int updateDynamicBaseline(int currentAverage, int currentBaseline) {
  // Adjust the baseline if the average exceeds it
  if (currentAverage > currentBaseline) {
    return currentAverage;
  }
  return currentBaseline;
}

// Function to calculate distance using an inverse square root function
float calculateDistance(int y, int b) {
  if (y <= 0) return 0;  // Avoid division by zero or invalid values

  float x = pow((float)y / (0.9513 * b), -2.746);
  return x;
}
