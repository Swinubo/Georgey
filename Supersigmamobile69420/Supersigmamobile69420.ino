// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

//Supersigmamobile69420 only for basic motions
//All movements ddone here are to be pre-determined by the program, all movements are hard coded

int m1_clockwise = 5; //left motor from front //2
int m1_counterclockwise = 6; //3
int m2_clockwise = 7; //right motor from front //4
int m2_counterclockwise = 8; //5

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
  delay(500);

  forward(255);
  delay(3000);
  
  left(255);
  delay(500);

  forward(255);
  delay(3000);
  
  left(255);
  delay(500);

  forward(255);
  delay(3000);
  
  left(255);
  delay(500);

  stop();
  
}

void loop() {

}

int size(int myList[]) {
  // You need to pass the size of the array explicitly when calling this function
  return sizeof(myList) / sizeof(myList[0]);
}

void forward(int speed)
{
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);

  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}

void backwards(int speed)
{
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);

  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}

void stop()
{
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);

  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
}

void left(int speed)
{
  delay(500);
  digitalWrite(m1_counterclockwise, LOW);
  analogWrite(m2_counterclockwise, speed);

  analogWrite(m1_clockwise, speed);
  digitalWrite(m2_clockwise, LOW);
}

void right(int speed)
{
  delay(500);
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);

  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}

void backwardsLeft(int speed)
{
  delay(500);
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);

  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}

void backwardsRight(int speed)
{
  delay(500);
  analogWrite(m1_counterclockwise, speed);
  analogWrite(m2_counterclockwise, speed);

  analogWrite(m1_clockwise, speed);
  analogWrite(m2_clockwise, speed);
}