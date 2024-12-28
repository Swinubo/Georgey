#include <IRremote.h>

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected

// Pin where the analog device is connected
const int analogPin = A0;
int analogValue = 0;               // Variable to store the analog reading
int baseline = 0;               // Variable to store the baseline darkness reading
int average = 0;                   // Variable to store the calculated average
int previousAverage = 0;        // Variable to store the previous calculated average
int significantChangeVal = 0;       // Variable to store the change required to be considered significsant between the baseline and the average
const int amountOfInputsForAverage = 6;  // Number of inputs to calculate average

int readings[amountOfInputsForAverage] = {0};  // Array to hold the last readings

//Motor pins
const int m1_clockwise = 6; //left motor //2
const int m1_counterclockwise = 5; //3
const int m2_clockwise = 8; //right motor //4
const int m2_counterclockwise = 7; //5

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {
  Serial.begin(9600);  // Start serial communication for monitoring

  // Initialize pins for motors
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
      pinMode(m_pins[i], OUTPUT);
  }

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver
}

void loop() {
  analogValue = analogRead(analogPin);  // Read analog input
  Serial.print("Analog Value: ");
  Serial.println(analogValue);  // Print the analog value to Serial Monitor

  // Shift the readings and add the new value
  for (int i = amountOfInputsForAverage - 1; i > 0; i--) {
    readings[i] = readings[i - 1];  // Shift old readings to the right
  }
  readings[0] = analogValue;  // Add the new reading to the start of the array

  //Update the average reading
  previousAverage = average;
  average = updateAverage();
  Serial.print("Average: ");
  Serial.println(average);

  // Update the baseline
  baseline = updateBaseline();
  Serial.print("Baseline: ");
  Serial.println(baseline);

  // Update the sigChange
  significantChangeVal = significantChange();
  Serial.print("Significant Change: ");
  Serial.println(significantChangeVal);

  if (baseline > 20 && detectBall() && IrReceiver.decode())
  {
    forward(255);
    delay(10000);
  }
  else
  {
    right(255);
    delay(200);
    stop();
  }

  delay(500);  // Wait half a second before reading again
}

bool detectBall(){
  if (baseline - average >= significantChangeVal && significantChangeVal > 0 && baseline - average > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int significantChange(){
  return 0.93*baseline - 32.2;
}

int updateAverage(){
  average = 0;  // Reset the average to 0

  // Calculate the average of the readings
  for (int i = 0; i < amountOfInputsForAverage; i++) {
    average += readings[i];
  }

  return average / amountOfInputsForAverage;
}

int updateBaseline() {
  // Update the baseline if the average is greater
  if (average > baseline) {
    return average;
  } else {
    return baseline;
  }
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

void right(int speed)
{
  delay(500);
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