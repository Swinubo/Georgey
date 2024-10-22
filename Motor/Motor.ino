// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int m1_clockwise = 2;
int m1_counterclockwise = 3;
int m2_clockwise = 4;
int m2_counterclockwise = 5;

int m_pins[] = {m1_clockwise, m1_counterclockwise, m2_clockwise, m2_counterclockwise};

void setup() {

  Serial.begin(9600);
  Serial.print("Program has begun.");
  
  //initialize pins
  for (int i = 0; i <= size(m_pins); i++) {
      pinMode(m_pins[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i <= size(m_pins); i++) {
      digitalWrite(m_pins[i], HIGH);
      delay(1000);
      digitalWrite(m_pins[i], LOW);
  }  
}

int size(int beat[])
{
  // Calculate the number of elements in the array and pass it to loopBeat
  //sizeOf() returns the value of the array in bytes. You check the total number of bytes, divided with one of the bytes value (i picked 0 just for readability, it doesnt matter which one i pick as they all have an equal byte value)
  //to get the number of items in the array
  return sizeof(beat) / sizeof(beat[0]);
}


//mr spina suggests using a certain chip
//allows for multiple motors to be conencted at once