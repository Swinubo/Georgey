// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

//Gyattasensa is for practicing using a sensor
//All movements done here are to be pre-determined by the program, all movements are hard coded

const int m1_clockwise = 2; //left motor
const int m1_counterclockwise = 3;
const int m2_clockwise = 4; //right motor
const int m2_counterclockwise = 5;

// this constant won't change. It's the pin number of the sensor's output:
const int trigPin = 7;
const int echoPin = 8;

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {
  Serial.begin(9600);
  Serial.println("Program has begun.");
  
  // Initialize pins for motors
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
      pinMode(m_pins[i], OUTPUT);
  }

  // Initialize pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int cm = sendSignal();
  Serial.println(cm);

  if (cm < 5 && cm > 0)  // Check that distance is within range
  {
    stop();
  }
  else
  {
    forward(255);
  }
}

// Modified sendSignal function with timing adjustments
int sendSignal() {
  long duration, cm;

  // Ensure a clean HIGH pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo response
  duration = pulseIn(echoPin, HIGH);
  Serial.println(duration);

  if (duration == 0) {
    Serial.println("No echo received or out of range.");
    return -1;  // Return -1 if no echo is received
  }

  cm = microsecondsToCentimeters(duration);
  return cm;
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


void stop() {
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);

  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
