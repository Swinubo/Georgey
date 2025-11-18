#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows

int obstacles[10] = {16}; // reserve space for up to 10 obstacles
int numObstacles = 1;     // start with 1 obstacle

int jumpButton = 0;
bool jumping = false;

int movementState = 0; //where 0 is rightfootup, 1 is leftfootup

byte dinoRightFootUp[] = {
  B00111,
  B00111,
  B10110,
  B11100,
  B01000,
  B10100,
  B10110,
  B11000
};

byte dinoLeftFootUp[] = {
  B00111,
  B00111,
  B10110,
  B11100,
  B01000,
  B10100,
  B11100,
  B00110
};

byte obstacle[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111
};

void setup() {
  pinMode(jumpButton, INPUT_PULLUP);

  Wire.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  /*lcd.clear();
  movementState = movementDino(0, 1);
  if (jumping)
  {
    jumping = false;
  }
  if (!digitalRead(jumpButton)) //check if button pressed
  {
    jumping = true;
    lcd.clear();
    movementState = movementDino(0, 0);
  }

  obsticaleSpawner();
  delay(500);*/

  lcd.clear();
  obsticaleSpawner();
  unsigned long startTime = millis();
  while (millis() - startTime < 400)
  {
    if (!digitalRead(jumpButton) && !jumping) //check if button pressed
    {
      jumping = true;
      movementState = movementDino(0, 0);
    }
    else
    {
      jumping = false;
      movementState = movementDino(0, 1);
    }
    delay(200);
  }
}

int movementDino(int column, int row){
  if (movementState == 0)
  {
    drawDino(dinoRightFootUp, column, row);
    return 1;
  }
  else
  {
    drawDino(dinoLeftFootUp, column, row);
    return 0;
  }
  return 0;
}

void drawDino(byte *dino, int column, int row){
  lcd.createChar(0, dino);
  lcd.setCursor(column, row);
  lcd.write(0);
}

void obsticaleSpawner() {
  for (int i = 0; i < numObstacles; i++) {
    lcd.createChar(1, obstacle);
    lcd.setCursor(obstacles[i], 1); // (column, row)
    lcd.write(1);

    obstacles[i] -= 1; // move obstacle left

    if (obstacles[i] < 0) { 
      removeObstacle(i); 
      i--; // important! because after removing, all elements shift left
    }
  }
}

void removeObstacle(int index) {
  for (int i = index; i < numObstacles - 1; i++) {
    obstacles[i] = obstacles[i + 1]; // shift everything left
  }
  numObstacles--; // now we have 1 fewer obstacle
}