#include <IRremote.h>
#include <Wire.h>
#include <MPU6050.h>

// Initialize devices
MPU6050 mpu;

float yaw = 0.0;
unsigned long prevTime = 0;

// Motor control pins (only work on PWM pins)
const int m1_clockwise = 6;
const int m1_counterclockwise = 5;
const int m2_clockwise = 9;
const int m2_counterclockwise = 10;
int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

int RECV_PIN = 7;  // IR receiver pin
float speedVariable = 1; // Adjust speed variable
int detectBallPin = 12;
bool hasBall = false;

int thresholdWhiteA0 = 510;  // Adjust this based on test readings
int thresholdWhiteA1 = 230;  // Adjust this based on test readings

bool detectColour= true;


void setup() {
    Serial.begin(9600);
    Serial.println("Initialization begun!");

    // Setup motor pins
    for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
        pinMode(m_pins[i], OUTPUT);
    }
    
    pinMode(detectBallPin, INPUT_PULLUP);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

    // Initialize MPU6050
    mpu.initialize();
    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected successfully!");
    } else {
        Serial.println("MPU6050 connection failed!");
    }
}

void loop() {
  yawVal();
  detectWhite();
  if (IrReceiver.decode()) {
      stop();
      delay(250);
      signed long startTime = millis();
      Serial.println("GOING FORWARD!");
      forward(200 * speedVariable, 200 * speedVariable);
      
    while (millis() - startTime < 1500) {
    detectWhite();
    if (iReceiveLost())
      {
        break;
      }
      yawVal();
      Serial.println(digitalRead(detectBallPin));
      
      if (!digitalRead(detectBallPin))
      {
        Serial.println(yawVal());
        if (yawVal() >= 270 || yawVal() <= 90) {
          while (!checkBallLost())
          {
            detectWhite();
            forward(255*speedVariable,255*speedVariable);
          }
        } else if (yawVal() > 90 && yawVal() < 180)
        {
          right(255*speedVariable,255*speedVariable);
          signed long startTime = millis();     
          while (millis() - startTime < 500) {
            detectWhite();
          }
        }
        else
        {
          left(255*speedVariable,255*speedVariable);
          signed long startTime = millis();     
          while (millis() - startTime < 500) {
            detectWhite();
          }
        }
      }
    }
    IrReceiver.resume();
  }
  Serial.println("GOING RIGHT!");
  right(255 * speedVariable, 255 * speedVariable);
}

bool checkBallLost() {
  static unsigned long ballLostStartTime = 0; // Stores the time when pin first detects 1
  if (digitalRead(detectBallPin) == 1) {
    if (ballLostStartTime == 0) {
      ballLostStartTime = millis(); // Start the timer when pin goes HIGH
    }

    // Check if 1 seconds (1000 ms) have passed
    if (millis() - ballLostStartTime >= 1000) {
      ballLostStartTime = 0; // Reset timer
      return true; // Ball is officially lost
    }
  } else {
    ballLostStartTime = 0; // Reset timer if pin goes back to 0
  }
  return false;
}

// Read and calculate yaw
int yawVal() {
    int16_t gz;
    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;
    prevTime = currentTime;

    gz = mpu.getRotationZ();
    float gyroZ = gz / 131.0;

    yaw += gyroZ * dt;
    yaw = fmod(yaw + 360.0, 360.0);
    yaw -= 0.03;

    return yaw;
}

bool iReceiveLost() {
  static unsigned long signalLostStartTime = 0; // Stores time when signal is first lost

  if (!IrReceiver.decode()) { // No IR signal received
    if (signalLostStartTime == 0) {
      signalLostStartTime = millis(); // Start timer when signal is first lost
    }

    // Check if 0.8 seconds (800 ms) have passed
    if (millis() - signalLostStartTime >= 800) {
      signalLostStartTime = 0; // Reset timer
      return true; // Signal is officially lost
    }
  } else {
    signalLostStartTime = 0; // Reset timer if signal is received
    IrReceiver.resume(); // Resume receiving signals
  }
  
  return false;
}

void detectWhite() {
  if (detectColour)
  {
    int a0Back = analogRead(A0);  // Read sensor value
    int a1Front = analogRead(A1);  // Read sensor value

    if (a1Front > thresholdWhiteA1) { //a0Back > thresholdWhiteA0 && 
        Serial.println("Detected: Green Surface");
    } 
    else if (a0Back < thresholdWhiteA0){
        Serial.println("Detected: White Surface");
        forward(255, 255);
        delay(500);
    }
    else
    {
      Serial.println("Detected: White Surface");
      backward(255, 255);
      delay(500);
    }
  }
}

// Function to move forward
void left(int speed1, int speed2) {
  analogWrite(m1_counterclockwise, speed1);
  digitalWrite(m1_clockwise, LOW);
  analogWrite(m2_clockwise, speed2);
  digitalWrite(m2_counterclockwise, LOW);
}

// Function to move back
void right(int speed1, int speed2) {
  digitalWrite(m1_counterclockwise, LOW);
  analogWrite(m1_clockwise, speed1);
  digitalWrite(m2_clockwise, LOW);
  analogWrite(m2_counterclockwise, speed2);
}

// Function to move left
void forward(int speed1, int speed2) {
  digitalWrite(m1_counterclockwise, LOW);
  analogWrite(m1_clockwise, speed1);
  analogWrite(m2_clockwise, speed2);
  digitalWrite(m2_counterclockwise, LOW);
}

// Function to move right
void backward(int speed1, int speed2) {
  analogWrite(m1_counterclockwise, speed1);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
  analogWrite(m2_counterclockwise, speed2);
}

//Function to stop movement
void stop() {
  digitalWrite(m1_counterclockwise, LOW);
  digitalWrite(m1_clockwise, LOW);
  digitalWrite(m2_clockwise, LOW);
  digitalWrite(m2_counterclockwise, LOW);
}