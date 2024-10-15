// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int motor_pin2 = 2;
int motor_pin6 = 6;

void setup() {

  Serial.begin(9600);
  Serial.print("Program has begun. e");
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pin6, OUTPUT);
}

void loop() {
  digitalWrite(motor_pin2, LOW);
  digitalWrite(motor_pin6, HIGH);
}

//mr spina suggests using an
//allows for multiple motors to be conencted at once