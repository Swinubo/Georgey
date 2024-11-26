void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //No surprises, radiohead
  tone(6, 440); // a
  delay(300);
  tone(6, 261); //middle c
  delay(300);
  tone(6, 349); //f
  delay(300);
  tone(6, 261); //middle c
  delay(300);

  tone(6, 440); // a
  delay(300);
  tone(6, 261); //middle c
  delay(300);
  tone(6, 349); //f
  delay(300);
  tone(6, 261); //middle c
  delay(300);

  tone(6, 440); // a
  delay(300);
  tone(6, 261); //middle c
  delay(300);
  tone(6, 349); //f
  delay(300);
  tone(6, 261); //middle c
  delay(300);

  tone(6, 466); // Bb
  delay(300);
  tone(6, 277); //c#
  delay(300);
  tone(6, 349); //f
  delay(300);
  tone(6, 392); //g
  delay(300);
}
