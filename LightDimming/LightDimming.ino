// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int led_pin1 = 10;
int led_pin2 = 11;

float power = 255; // Start at full power (255 for PWM)

bool fadingOut = true;

void setup() {
  Serial.begin(9600);
  Serial.print("Program has begun.");
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
}

void loop() {

  // Gradually decrease the power but keep it within the range of 0 to 255
  
  if (power <= 0) { //add if statement as leds will go bonkers if power goes to negative
    fadingOut = false;
  }
  if (power >= 200)
  {
    fadingOut = true;
  }

  if (fadingOut)
  {
    power -= 5; // Reduce power
  }
  else
  {
    power += 5; //gradually increase power
  }

  activateLEDS(power); // Activate LEDs with power level

  delay(100);
}

void activateLEDS(float MyVolt)
{
  // Use analogWrite to control brightness (PWM, value between 0 and 255)
  analogWrite(led_pin1, MyVolt); //digitWrite can only take in HIGH or LOW
  analogWrite(led_pin2, MyVolt); //therefore the function analogWrite must be used
}
