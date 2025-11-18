#include <IRremote.h>

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
int thresholdWhiteA1 = 330;  // Adjust this based on test readings

bool detectColour = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization begun!");

    // Setup motor pins
    for (int i = 0; i < sizeof(m_pins) / sizeof(m_pins[0]); i++) {
        pinMode(m_pins[i], OUTPUT);
    }

    pinMode(detectBallPin, INPUT_PULLUP);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
    detectWhite();
    if (IrReceiver.decode()) {
        stop();
        delay(250);
        signed long startTime = millis();
        Serial.println("GOING FORWARD!");
        forward(200 * speedVariable, 200 * speedVariable);

        while (millis() - startTime < 1500) {
            detectWhite();
            if (iReceiveLost()) {
                break;
            }

            Serial.println(digitalRead(detectBallPin));

            if (!digitalRead(detectBallPin)) {
                while (!checkBallLost()) {
                    detectWhite();
                    forward(255 * speedVariable, 255 * speedVariable);
                }
            }
        }
        IrReceiver.resume();
    }
    Serial.println("GOING RIGHT!");
    right(255 * speedVariable, 255 * speedVariable);
}

bool checkBallLost() {
    static unsigned long ballLostStartTime = 0;
    if (digitalRead(detectBallPin) == 1) {
        if (ballLostStartTime == 0) {
            ballLostStartTime = millis();
        }
        if (millis() - ballLostStartTime >= 1000) {
            ballLostStartTime = 0;
            return true;
        }
    } else {
        ballLostStartTime = 0;
    }
    return false;
}

bool iReceiveLost() {
    static unsigned long signalLostStartTime = 0;
    if (!IrReceiver.decode()) {
        if (signalLostStartTime == 0) {
            signalLostStartTime = millis();
        }
        if (millis() - signalLostStartTime >= 800) {
            signalLostStartTime = 0;
            return true;
        }
    } else {
        signalLostStartTime = 0;
        IrReceiver.resume();
    }
    return false;
}

void detectWhite() {
    if (detectColour) {
        int a0Back = analogRead(A0);
        int a1Front = analogRead(A1);

        if (a0Back > thresholdWhiteA0 && a1Front > thresholdWhiteA1) {
            Serial.println("Detected: Green Surface");
        } else if (a0Back < thresholdWhiteA0) {
            Serial.println("Detected: White Surface");
            forward(255, 255);
            delay(250);
        } else {
            Serial.println("Detected: White Surface");
            backward(255, 255);
            delay(250);
        }
    }
}

// Function to move left
void forward(int speed1, int speed2) {
    analogWrite(m1_counterclockwise, speed1);
    digitalWrite(m1_clockwise, LOW);
    analogWrite(m2_clockwise, speed2);
    digitalWrite(m2_counterclockwise, LOW);
}

// Function to move right
void backward(int speed1, int speed2) {
    digitalWrite(m1_counterclockwise, LOW);
    analogWrite(m1_clockwise, speed1);
    digitalWrite(m2_clockwise, LOW);
    analogWrite(m2_counterclockwise, speed2);
}

// Function to move forward
void left(int speed1, int speed2) {
    digitalWrite(m1_counterclockwise, LOW);
    analogWrite(m1_clockwise, speed1);
    analogWrite(m2_clockwise, speed2);
    digitalWrite(m2_counterclockwise, LOW);
}

// Function to move backward
void right(int speed1, int speed2) {
    analogWrite(m1_counterclockwise, speed1);
    digitalWrite(m1_clockwise, LOW);
    digitalWrite(m2_clockwise, LOW);
    analogWrite(m2_counterclockwise, speed2);
}

// Function to stop movement
void stop() {
    digitalWrite(m1_counterclockwise, LOW);
    digitalWrite(m1_clockwise, LOW);
    digitalWrite(m2_clockwise, LOW);
    digitalWrite(m2_counterclockwise, LOW);
}
