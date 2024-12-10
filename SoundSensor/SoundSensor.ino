// KY-037 Microphone Sound Sensor - Tutorial
// Arduino Uno

int digitalPin = 7;  // KY-037 digital interface
int analogPin = A0;  // KY-037 analog interface
int ledPin = 13;     // Onboard LED
int digitalVal;      // Variable for digital readings
int analogVal;       // Variable for analog readings

void setup() {
  pinMode(digitalPin, INPUT);  // Set digital pin as input
  pinMode(analogPin, INPUT);   // Set analog pin as input
  pinMode(ledPin, OUTPUT);     // Set LED pin as output
  Serial.begin(9600);          // Initialize serial communication
}

void loop() {
  // Read the digital interface
  digitalVal = digitalRead(digitalPin);

  if (digitalVal == HIGH) {
    digitalWrite(ledPin, HIGH);  // Turn ON Arduino's LED
  } else {
    digitalWrite(ledPin, LOW);   // Turn OFF Arduino's LED
  }

  // Read the analog interface
  analogVal = analogRead(analogPin);
  
  // Print analog value to the Serial Monitor
  Serial.println(analogVal);
}
