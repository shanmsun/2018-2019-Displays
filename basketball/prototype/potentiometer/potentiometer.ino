int pot_pin = A0;
int output = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pot_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  output = analogRead(pot_pin);
  Serial.println(output);
}
