int pin2 = 2;
int pin4 = 4;

bool LEDvar = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(4, INPUT_PULLUP);

  // Ensure LED starts off
  digitalWrite(2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  bool pressed = false;
  while(!pressed)
  {
    if (digitalRead(pin4) == LOW && LEDvar == false) {
      LEDvar = true;
      pressed = true;
    }
    else if (digitalRead(pin4) == LOW && LEDvar == true) {
      LEDvar = false;
      pressed = true;
    }
  }

  /*if (digitalRead(pin4) == LOW && LEDvar == false) {
    LEDvar = true;
  }
  else if (digitalRead(pin4) == LOW && LEDvar == true) {
    LEDvar = false;
  }*/

  if (LEDvar == true)
  {
    digitalWrite(pin2, HIGH);
  }
  if (LEDvar == false)
  {
    digitalWrite(pin2, LOW);
  }
  delay(1000);
}
