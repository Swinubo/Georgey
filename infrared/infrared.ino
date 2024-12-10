#include <IRremote.h>

int RECV_PIN = 4;  // Pin where your IR receiver's OUT pin is connected

const int m1_clockwise = 6; //left motor //2
const int m1_counterclockwise = 5; //3
const int m2_clockwise = 8; //right motor //4
const int m2_counterclockwise = 7; //5

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

String receivedSignal;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver
  Serial.println("Ready to receive IR signals");

  for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
      pinMode(m_pins[i], OUTPUT);
  }

}

void loop() {
  if (IrReceiver.decode()) {
    receivedSignal = String(IrReceiver.decodedIRData.command, HEX);
    Serial.print("Signal received: ");
    Serial.println(receivedSignal);  // Print the signal

    if (receivedSignal.equalsIgnoreCase("5")) {
      forward(255);
      Serial.println("Going forward!");
    }
    else if (receivedSignal.equalsIgnoreCase("a")) {
      left(255);
      Serial.println("Going left!");
    }
    else if(receivedSignal.equalsIgnoreCase("1e")) {
      right(255);
      Serial.println("Going right!");
    }
    else if (receivedSignal.equalsIgnoreCase("2")) {
      backwards(255);
      Serial.println("Going backwards!");
    }


    IrReceiver.resume();  // Prepare to receive the next signal
    delay(100);
  }
  else
  {
    stop();
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