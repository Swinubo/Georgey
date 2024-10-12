/*
//my first program
void setup() {  //will only run once at the beginning of the program
  // put your setup code here, to run once:
  Serial.begin(9600); // sets data rate in bits per second, the higher the number put here, the more data exchanged in a second
  delay(1000);
  Serial.println("Hello world!"); //prints data to a serial board

}

void loop() { // will run continuously after the first setup()
  // put your main code here, to run repeatedly:

}
*/

//Me? I'm just hanging around (totally didn't steal most of the blink basic example)

//Library for LCD with I2C interface.
#include <LiquidCrystal_I2C.h>

//Set the LCD address to 0x27 for a 16 chars and 2 line display.
LiquidCrystal_I2C lcd(0x27,16,2);

// Variables
int delayTime = 1000;
char on[20] = "LIGHT IS ON!";
char off[20] = "LIGHT IS OFF!";

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // sets data rate in bits per second, the higher the number put here, the more data exchanged in a second
  //Initialize the lcd, clear the display and turn on the backlight.
  lcd.init();
  delay(100);
  lcd.clear();
  lcd.backlight();
  delay(1000);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)                    // wait for a second
  Serial.println(on);
  lcd.setCursor(0,0);
  lcd.print(on);
  delay(delayTime);
  lcd.clear();

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  Serial.println(off);
  lcd.setCursor(0,0);
  lcd.print(off);
  delay(delayTime);                      // wait for a second
  lcd.clear();
}