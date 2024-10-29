// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

//Supersigmamobile69420 only for basic motions
//All movements ddone here are to be pre-determined by the program, all movements are hard coded

int m1_clockwise = 2; //left motor
int m1_counterclockwise = 3;
int m2_clockwise = 4; //right motor
int m2_counterclockwise = 5;

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {

  Serial.begin(9600);
  Serial.println("Program has begun.");
  
  //initialize pins
  for (int i = 0; i < size(m_pins); i++) {
      pinMode(m_pins[i], OUTPUT);
  }

  forward(255);

  delay(3000);

  left(255);

  delay(5000);

  stop();
  
}

void loop() {
  /*for (int i = 0; i < size(m_pins); i++) {
      analogWrite(m_pins[i], HIGH);
      delay(1000);
      analogWrite(m_pins[i], LOW);
  }  */
}

int size(int beat[]) {
  // You need to pass the size of the array explicitly when calling this function
  return sizeof(m_pins) / sizeof(m_pins[0]);
}

void forward(int speed)
{
  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);

  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
}

void backwards(int speed)
{
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);

  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}

void stop()
{
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);

  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
}

void left(int speed)
{
  digitalWrite(m1_clockwise, LOW);
  analogWrite(m2_clockwise, speed);

  analogWrite(m1_counterclockwise, speed);
  digitalWrite(m2_counterclockwise, LOW);
}

void right(int speed)
{
  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);

  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);
}

void backwardsLeft(int speed)
{
  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);

  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);
}

void backwardsRight(int speed)
{
  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);

  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);
}