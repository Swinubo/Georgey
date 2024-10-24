// ----------------------------------------------------------------
// Arduino Nano Metro Simulator
// ----------------------------------------------------------------

int m1_clockwise = 2;
int m1_counterclockwise = 3;
int m2_clockwise = 4;
int m2_counterclockwise = 5;

int m_pins[] = {m2_clockwise, m2_counterclockwise, m1_clockwise, m1_counterclockwise};

void setup() {

  Serial.begin(9600);
  Serial.println("Program has begun.");
  
  //initialize pins
  for (int i = 0; i < size(m_pins); i++) {
      pinMode(m_pins[i], OUTPUT);
  }

  for (int i = 0; i < size(m_pins); i++) {
      digitalWrite(m_pins[i], HIGH);
      delay(1000);
      digitalWrite(m_pins[i], LOW);
  } 
}

void loop() {
  /*for (int i = 0; i < size(m_pins); i++) {
      digitalWrite(m_pins[i], HIGH);
      delay(1000);
      digitalWrite(m_pins[i], LOW);
  }  */
}

int size(int beat[]) {
  // You need to pass the size of the array explicitly when calling this function
  return sizeof(m_pins) / sizeof(m_pins[0]);
}
