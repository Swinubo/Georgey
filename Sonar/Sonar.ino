// ----------------------------------------------------------------
// Arduino Ultrasoninc Sensor HC-SR04 distance measurement
// ----------------------------------------------------------------

//Library for LCD with I2C interface.
#include <LiquidCrystal_I2C.h>

//Set the LCD address to 0x27 for a 16 chars and 2 line display.
//LiquidCrystal_I2C lcd(0x27,16,2);

const byte echoPin = 8; // attach Arduino pin D2 to pin Echo of HC-SR04
const byte trigPin = 7; // attach Arduino pin D3 to pin Trig of HC-SR04

long duration; // variable for the duration of sound wave travel (uS)
long distance; // variable for the distance measurement (cm)
int sumDistance; // variable for the sum of distance measurement to be averaged
int avgDistance; // average distance
byte avgCounter; // used to count the number of measurements to be averaged
byte avgCount=10; // number of measurements to average

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 baudrate -> commenteed out as no communications to serial are being made
  delay(10);


  //Initialize the lcd, clear the display and turn on the backlight.
  /*lcd.init();
  delay(100);
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("SONAR demo by");
  //delay(2000);
  displ_name("Gaspar S. Mendes");
  displ_name("Mihran Chowdhury");
  displ_name("Luca Santoro");

  lcd.clear();*/
}

void loop() {

  //Reset the sum of the distance readings to be averaged.
  sumDistance = 0;
  
  //Take the average distance over 5 measurements while ignoring unlikely high values.
  for (avgCounter = 1; avgCounter <= avgCount; avgCounter++){
    // Clears the trigPin condition
    delayMicroseconds(2);
    digitalWrite(trigPin, LOW);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.0343 / 2; // Speed of sound wave divided by 2 (go and back)
    // Sum the distance reading if it was less than 200.

    if(distance < 100){
      sumDistance += distance;
    }   
    delay(150);
  }

  avgDistance = sumDistance/avgCount;

  //lcd.setCursor(0,0);
  Serial.print("Distance: ");
  Serial.print(avgDistance);
  Serial.println(" cm ");

}

/*void displ_name(String name){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(name); 
  delay(3000);
}*/