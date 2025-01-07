#include <IRremote.h>

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected

// Pin where the analog device is connected
const int analogPin = A0;
int analogValue = 0;               // Variable to store the analog reading
int previousAverage = 0;        // Variable to store the previous calculated average
int significantChangeVal = 0;       // Variable to store the change required to be considered significsant between the baseline and the average
int average = 0;
const int amountOfInputsForAverage = 24;  // Number of inputs to calculate average

int sampleCounter = 0;             // Counter to track the number of samples
int sum = 0;                       // Variable to store the sum of the readings 

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
  /*Serial.print("Analog Value: ");
  Serial.println(analogValue);  // Print the analog value to Serial Monitor*/
  previousAverage = average;
  average = updateAverage(analogValue);

  if (IrReceiver.decode())
  {
    Serial.print("GOING FORWARD!");
    forward(255);
    delay(1000);
    while (previousAverage >= average)
    {
      analogValue = analogRead(analogPin);  // Read analog input
      Serial.print("Analog Value: ");
      Serial.println(analogValue);  // Print the analog value to Serial Monitor
      previousAverage = average;
      average = updateAverage(analogValue);
    }
  }
  IrReceiver.resume();  // Prepare to receive the next signal

  Serial.println("GOING RIGHT!");
  right(100);
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
  //delay(500);
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