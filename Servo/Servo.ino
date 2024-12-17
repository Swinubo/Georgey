#include <Servo.h>
#include <IRremote.h>

int RECV_PIN = 3;  // Pin where your IR receiver's OUT pin is connected

Servo servo_4;
int angle = 0;

String receivedSignal;

int zone;
//zone 1= right
//zone 2 = front
//zone 3 = left
//zone 4 = back

const int m1_clockwise = 6; //left motor //2
const int m1_counterclockwise = 5; //3
const int m2_clockwise = 8; //right motor //4
const int m2_counterclockwise = 7; //5
int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

// this constant won't change. It's the pin number of the sensor's output:
const int trigPin = 10; //7
const int echoPin = 11; //8

void setup()
{
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver

  // Initialize pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

   // Initialize pins for motors
  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
      pinMode(m_pins[i], OUTPUT);
  } 

  servo_4.attach(4, 500, 2500);
  Serial.begin(9600);
}

void loop()
{
  servo_4.write(angle);
  zone = detectBall();
  changeDirection();
  //sendSignal();

  angle += 90;
  if (angle > 180)
  {
    angle =0;
  }
  delay(1000); // Wait for 1000 millisecond(s)
}

int detectBall()
{
  if (IrReceiver.decode()) {
    receivedSignal = String(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume(); // Add this line
    if (angle == 0)
    {
      return 1;
    }
    else if (angle == 90)
    {
      return 2;
    }
    else if (angle == 180)
    {
      return 3;
    }
  }     
  else
  {
      return 4;
  }
}

void changeDirection()
{
  if (zone == 1)
  {
    right(255);
    delay(150);
    forward(255);
  }  
  else if (zone == 2)
  {
    forward(255);
  }
  else if (zone == 3)
  {
    left(255);
    delay(150);
    forward(255);
  }
  else
  {
    right(255);
    delay(325);
    forward(255);
  }
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

void left(int speed)
{
  digitalWrite(m1_counterclockwise, LOW);  
  digitalWrite(m2_counterclockwise, LOW);  

  analogWrite(m1_clockwise, speed);
  digitalWrite(m2_clockwise, LOW);
}

void right(int speed)
{
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);

  digitalWrite(m1_clockwise, LOW);
  analogWrite(m2_clockwise, speed);
}

//write a function where the robot stops and then changes direction to avoid the stutter !!

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
  if (cm < 10 && cm > 0)  // Check that distance is within range
  {
    
    backwards(255);
    delay(1000);
    right(255);
    delay(350);
    forward(255);
  }

}