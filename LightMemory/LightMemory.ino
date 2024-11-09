const int inpPin= 2;
const int voltagePin= 3;
bool LEDonOrOff = false; //represents the voltage going into pin  2

void setup() {
  Serial.begin(9600);
  pinMode(inpPin, INPUT);  
  pinMode(voltagePin, OUTPUT);  
}

void loop() {
  int state = digitalRead(inpPin); // Read the state of the pin

  if (state == HIGH) {
    Serial.println("Pin is HIGH");
    LEDonOrOff = !LEDonOrOff;
    switchLight();
    delay(5000);
  }

}

void switchLight()
{
  if (LEDonOrOff == true)
  {
    digitalWrite(voltagePin, HIGH);  
  }
  else
  {
    digitalWrite(voltagePin, LOW);
  }
}
