#define IR_SIGNAL_PIN_ANALOG A0  // Analog pin for intensity measurement
#define IR_SIGNAL_PIN_DIGITAL 2 // Digital pin for frequency measurement

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(IR_SIGNAL_PIN_DIGITAL, INPUT);  // Set the digital pin as input
}

void loop() {
  // Measure IR intensity
  int intensity = analogRead(IR_SIGNAL_PIN_ANALOG);  // Read analog voltage
  Serial.print("IR Intensity: ");
  Serial.println(intensity);

  // Measure IR frequency
  unsigned long highDuration = pulseIn(IR_SIGNAL_PIN_DIGITAL, HIGH); // Measure HIGH pulse
  unsigned long lowDuration = pulseIn(IR_SIGNAL_PIN_DIGITAL, LOW);  // Measure LOW pulse

  if (highDuration > 0 && lowDuration > 0) {
    // Calculate frequency
    float period = highDuration + lowDuration;  // Total period in microseconds
    float frequency = 1000000.0 / period;       // Convert to Hz
    Serial.print("IR Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");
  } else {
    Serial.println("No frequency detected");
  }

  delay(200);  // Delay for readability
}
