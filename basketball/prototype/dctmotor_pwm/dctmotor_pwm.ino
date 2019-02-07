#include <math.h>

int dc_enable = 2;
int dc_control = 5; //freq = 980Hz
int pot = A0;

const int PWM_RANGE []= {60, 255}; //{min, max}
const float SCALE_FACTOR = 0.19; //pot reading(0-1023) to pwm range

void setup() {
  pinMode(dc_enable, OUTPUT);
  pinMode(dc_control, OUTPUT);
  pinMode(pot, INPUT);
}

void loop() {
  digitalWrite(dc_enable, HIGH);
  int pot_reading = analogRead(pot);
  analogWrite(dc_control, calculate_new_pwm(pot_reading)); 
}

int calculate_new_pwm(int pot_reading){
  int new_pwm = PWM_RANGE[0] + int(ceil(SCALE_FACTOR * pot_reading));
  if (new_pwm < PWM_RANGE[0]){
    new_pwm = PWM_RANGE[0];
  }
  else if (new_pwm > PWM_RANGE[1]){
    new_pwm = PWM_RANGE[1];
  }
  return new_pwm;
}

