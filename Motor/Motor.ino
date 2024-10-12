// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int motor_pin = 12;

void setup() {

  Serial.begin(9600);
  Serial.print("Program has begun. e");
  pinMode(motor_pin, OUTPUT);
}

void loop() {
  digitalWrite(motor_pin, HIGH);
  delay(5000);
  digitalWrite(motor_pin, LOW);
  delay(2000);
}