void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT); //initialize pinmode 5 as output ALSO YELLOW
  pinMode(6, OUTPUT); //initialize pinmode 6 as output ALSO RED
  pinMode(7, OUTPUT); //initialize pinmode 7 as output ALSO GREEN
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7, HIGH); //turn the LED on
  delay(4000); //delaying the program by one second effectively keeping the LED on for a second
  digitalWrite(7, LOW); //turn LED off
  digitalWrite(5, HIGH); //turn the LED on
  delay(2000); //delaying the program by one second effectively keeping the LED on for a second
  digitalWrite(5, LOW); //turn LED off
  digitalWrite(6, HIGH); //turn the LED on
  delay(4000); //delaying the program by one second effectively keeping the LED on for a second
  digitalWrite(6, LOW); //turn LED off
}
