int pin2 = 2;
int pin4 = 4;

bool LEDvar = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(4, INPUT);

  // Ensure LED starts off
  digitalWrite(2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(pin4) == HIGH) {
    LEDvar = true;
  }

  if (LEDvar == true)
  {
    digitalWrite(pin2, HIGH);
  }
}
