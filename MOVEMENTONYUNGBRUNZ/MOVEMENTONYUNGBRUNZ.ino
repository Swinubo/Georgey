// Motor control pins
const int motorPins[] = {3, 5 , 6, 9};

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization begun!");

    // Setup motor pins
    for (int i = 0; i < 4; i++) {
        pinMode(motorPins[i], OUTPUT);
    }
}

void loop() {
  forward(255);
  delay(500);
  backward(255);
  delay(500);
}

void forward(int speed) {
  analogWrite(3, speed);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, speed);
}

void backward(int speed) {
  analogWrite(3, 0);
  analogWrite(5, speed);
  analogWrite(6, speed);
  analogWrite(9, 0);
}

void left(int speed) {
  analogWrite(3, speed);
  analogWrite(5, 0);
  analogWrite(6, speed);
  analogWrite(9, 0);
}

void right(int speed) {
  analogWrite(3, 0);
  analogWrite(5, speed);
  analogWrite(6, 0);
  analogWrite(9, speed);
}