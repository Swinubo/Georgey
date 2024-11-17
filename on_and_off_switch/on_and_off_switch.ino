#include <ezButton.h>
int IN1=5;
int IN2=6;
int IN3=7;
int IN4=8;
int ENA=9;
int ENB=10;
ezButton toggleSwitch(11);  // create ezButton object that attach to pin 11

void setup() {
  Serial.begin(9600);
  toggleSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  for (int i = 5; i <11; i ++) 
   {
     pinMode(i, OUTPUT);   
   }
}

void loop() {
  toggleSwitch.loop(); // MUST call the loop() function first

  if (toggleSwitch.isPressed())
    Serial.println("The switch: OFF -> ON");

  if (toggleSwitch.isReleased())
    Serial.println("The switch: ON -> OFF");

  int state = toggleSwitch.getState();
  if (state == HIGH)
    Serial.println("The switch: OFF");
  else
    Serial.println("The switch: ON");
     // rotate CW
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,200); 
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  analogWrite(ENB,200); 
  delay(1000);
  // pause for 1S
  analogWrite(ENA,0); 
  analogWrite(ENB,0); 
  delay(1000); 
  // rotate CCW
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
 
  analogWrite(ENA,100); 
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
  analogWrite(ENB,100); 
  delay(1000);
  // pause for 1S
  analogWrite(ENA,0); 
  analogWrite(ENB,0); 
  delay(1000); 
}