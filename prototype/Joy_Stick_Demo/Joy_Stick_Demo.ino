/*
  Thumb Joystick demo
 connect the module X<->A0;Y<->A1 and K<-> D3;
 */
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  int sensorK = digitalRead(D3)
  Serial.print("The X and Y coordinate is:");
  Serial.print(sensorValue1, DEC);
  Serial.print(",");
  Serial.println(sensorValue2, DEC);
  Serial.println(" ");
  delay(200);
}
