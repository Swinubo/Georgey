#include <Wire.h>
#include <MPU6050.h>
#include <IRremote.h>

// Initialize MPU6050
MPU6050 mpu;

float yaw = 0.0;
unsigned long prevTime = 0;

float speedVariable = 1; // Adjust as needed

int RECV_PIN = 8;  // IR receiver pin
int detectBallPin = 10;
bool hasBall = false;
int dribblerPin = 11;

// Motor control pins
const int motorPins[] = {3, 5, 6, 9};

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization begun!");

    Wire.begin();

    // Setup motor pins
    for (int i = 0; i < 4; i++) {
        pinMode(motorPins[i], OUTPUT);
    }

    pinMode(detectBallPin, INPUT_PULLUP);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

    // Initialize MPU6050
    mpu.initialize();
    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected successfully!");
    } else {
        Serial.println("MPU6050 connection failed!");
        while (1); // Stop execution
    }

    prevTime = millis();
}

void loop() {
    yawVal();
    IrReceiver.resume();
    delay(500);    
    Serial.println(IrReceiver.decode());
    if (IrReceiver.decode()) {
        stop();
        delay(250);
        signed long startTime = millis();
        Serial.println("GOING FORWARD!");
        forward(180 * speedVariable);
        while (millis() - startTime < 500) {
            if (digitalRead(detectBallPin) == 0) {
                Serial.println("Ball closes switch!");
                analogWrite(dribblerPin, 255);
                hasBall = true;
                Serial.println("Hit ball!");
                while (!(yawVal() >= 350)) {
                  Serial.println("Turning for yaw value...");
                    right(110 * speedVariable);
                    if (checkBallLost()) {
                        Serial.println("Checked ball false!");
                        hasBall = false;
                        analogWrite(dribblerPin, 0);
                        break;
                    }
                }
                while (hasBall) {
                    Serial.println("Going straight ahead with the  ball!");
                    analogWrite(dribblerPin, 0);
                    forward(255 * speedVariable);
                    delay(100);
                    if (checkBallLost()) {
                        hasBall = false;
                    }
                }
                break;
            }
        }
    }


    Serial.println("GOING RIGHT!");
    right(130 * speedVariable);
}

bool checkBallLost() {
    static unsigned long ballLostStartTime = 0;
    if (digitalRead(detectBallPin) == 1) {
        if (ballLostStartTime == 0) {
            ballLostStartTime = millis();
        }
        if (millis() - ballLostStartTime >= 3000) {
            ballLostStartTime = 0;
            return true;
        }
    } else {
        ballLostStartTime = 0;
    }
    return false;
}

int yawVal() {
    int16_t gz;
    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;
    prevTime = currentTime;
    gz = mpu.getRotationZ();
    float gyroZ = gz / 131.0;
    yaw += gyroZ * dt;
    yaw = fmod(yaw + 360.0, 360.0);
    return yaw - 0.2;
}

// Motor control functions
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

void stop() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(motorPins[i], LOW);
    }
}
