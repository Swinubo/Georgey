#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD address if needed

int RECV_PIN = 4;  // IR receiver pin

// Analog sensor pin
const int analogPin = A0;
int analogValue = 0;
int baseline = 0;
int average = 0;

// Variables for calculating distance
int amountOfInputsForAverage = 6;
int sampleCounter = 0;
int sum = 0;

// Calculate distance using the given formula
float calculateDistance(int baseline, int average) {
    float b = (float)(baseline);
    float a = (float)(average);
    float y = (float)(baseline - average); // You can adjust y based on your sensor data or other factors
    
    // Solve for x using the equation x = (y / (1.01 * b - 42.73)) ^ (1 / -0.264)
    float x = pow((y / ((1.01 * b) - 42.73)), (1 / -0.264));

    return x; // Distance in cm
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


// Calculate significant change threshold
int significantChange() {
    return (0.0487 * baseline + 0.5);
}

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
    Wire.begin();
    lcd.init();
    lcd.backlight();

    // Initialize baseline value  
    baseline = initialization();

    Serial.println("Initialization complete!");
    Serial.print("Baseline: ");
    Serial.println(baseline);
}

void loop() {
    analogValue = analogRead(analogPin);  // Read analog input

    // Update the average reading
    average = updateAverage(analogValue);

    // Calculate the distance
    if (baseline - average > significantChange() && IrReceiver.decode())
    {
      float distance = calculateDistance(baseline, average);
      if (distance < 1)
      {
        distance = 1.0;
      }
      // Display the distance on the LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      lcd.print(distance);
      lcd.print(" cm");
    }
    else
    {
        // Display the distance on the LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unable to detect ball!");
    }

      lcd.setCursor(0, 1);
      lcd.print("b, a: ");
      lcd.print(baseline);
      lcd.print(", ");
      lcd.print(average);
}

// Initialization function to get the baseline
int initialization() {
    int turnDuration = 5000;
    unsigned long startTime = millis();
    while (millis() - startTime < turnDuration) {
        analogValue = analogRead(analogPin);
        average = updateAverage(analogValue);
        baseline = average;
    }
    if (baseline < 43) {
      baseline = 43;
    }
    return baseline;
}

