// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int leftMotorPin = 6;
int rightMotorPin = 2;

void setup() {
  Serial.begin(9600);
  Serial.print("Program has begun. e");
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
}
