#include <IRremote.h>

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected

// Pin where the analog device is connected
const int analogPin = A0;
const int digitalSensorPin = 2;  // Pin where your digital sensor is connected

int analogValue = 0;           // Variable to store the analog reading
int previousAnalogValue = 1023;  // Start with no signal (maximum analog value)
bool ballDetected = false;     // Flag to track ball detection

// Motor pins
const int m1_clockwise = 6;  // Left motor forward
const int m1_counterclockwise = 5;  // Left motor reverse
const int m2_clockwise = 8;  // Right motor forward
const int m2_counterclockwise = 7;  // Right motor reverse

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {
  Serial.begin(9600);  // Start serial communication for monitoring

  // Initialize motor pins
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
    pinMode(m_pins[i], OUTPUT);
  }

  pinMode(digitalSensorPin, INPUT);  // Set the digital sensor pin as input
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the IR receiver
}

void loop() {
  if (!ballDetected) {
    // Turn the robot while checking for the ball
    right(150);
    if (digitalRead(digitalSensorPin) == HIGH) {
      ballDetected = true;  // Ball detected
      stop();
      delay(500);  // Small delay to stabilize
    }
  } else {
    // Move forward and monitor signal intensity
    forward(255);
    analogValue = analogRead(analogPin);

    Serial.print("Analog Value: ");
    Serial.println(analogValue);

    if (analogValue > previousAnalogValue) {
      // Signal intensity weakening, stop the robot
      stop();
      Serial.println("Signal weakening, stopping.");
      ballDetected = false;  // Reset to look for the ball again if needed
    }

    previousAnalogValue = analogValue;  // Update the previous analog value
  }
}

void forward(int speed) {
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}

void right(int speed) {
  digitalWrite(m1_counterclockwise, LOW);
  analogWrite(m2_counterclockwise, speed);
  analogWrite(m1_clockwise, speed);
  digitalWrite(m2_clockwise, LOW);
}

void stop() {
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}
