#include <LiquidCrystal.h>
#include <IRremote.h>

int RECV_PIN = 2;  // Pin where your IR receiver's OUT pin is connected

// LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

String receivedSignal;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
}

void loop() {
  if (IrReceiver.decode()) {
    receivedSignal = String(IrReceiver.decodedIRData.command, HEX);
    Serial.print("Signal received: ");
    Serial.println(receivedSignal);  // Print the signal

    if (receivedSignal.equalsIgnoreCase("5")) {
      lcd.clear();
      lcd.print("Going forward!");
    }
    else if (receivedSignal.equalsIgnoreCase("a")) {
      lcd.clear();
      lcd.print("Going left!");
    }
    else if(receivedSignal.equalsIgnoreCase("1e")) {
      lcd.clear();
      lcd.print("Going right!");
    }
    else if (receivedSignal.equalsIgnoreCase("2")) {
      lcd.clear();
      lcd.print("Going backwards!");
    }
    IrReceiver.resume();  // Prepare to receive the next signal
    delay(100);
  }
}
