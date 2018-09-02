  const int vrx = A5;
  const int vry = A4;
  const int button_pin = 8;
  const int led_pin = 9;
  

void setup() {
  // put your setup code here, to run once:

  pinMode (led_pin, OUTPUT);
  pinMode (button_pin, INPUT_PULLUP);

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (digitalRead(button_pin) == HIGH){
    digitalWrite(led_pin, HIGH);
  }
  else{
    digitalWrite(led_pin, LOW);
  }

  if (Serial){
    Serial.print("Horizontal pin reading: ");
    Serial.println(analogRead(vrx));
    Serial.print ("Vertical pin reading: ");
    Serial.println(analogRead(vry));
    delay(1000);
  }
}
