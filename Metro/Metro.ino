// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int motor_pin = 12;
int beat[] = {840, 70, 840, 70, 500, 50, 500, 50, 500, 50, 860, 140, 60, 440, 180, 80, 110}; 

void setup() {

  Serial.begin(9600);
  Serial.print("Program has begun. e");
  pinMode(motor_pin, OUTPUT);
  loopBeat(beat);
}

void loop() {

  
}

void setBeat(float beat, int wait)
{
  digitalWrite(motor_pin, HIGH);
  delay(beat);
  digitalWrite(motor_pin, LOW);
  delay(wait);
}

void loopBeat(int beat[])
{
  for (int i = 0; i < beat[size(beat)]; i+=2) //constraint variable establised, when should the cosntraint varaible stop being called, iterate the constraint
  {
    setBeat(beat[i], beat[i+1]);
  }
  loopBeat(beat);
}

int size(int beat[])
{
  // Calculate the number of elements in the array and pass it to loopBeat
  //sizeOf() returns the value of the array in bytes. You check the total number of bytes, divided with one of the bytes value (i picked 0 just for readability, it doesnt matter which one i pick as they all have an equal byte value)
  //to get the number of items in the array
  return sizeof(beat) / sizeof(beat[0]);
}