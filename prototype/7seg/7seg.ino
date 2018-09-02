#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define YOUR_ADDRESS 0x70
#define BEST_ADDRESS 0x77

Adafruit_7segment your_score_dis = Adafruit_7segment();
Adafruit_7segment best_score_dis = Adafruit_7segment();

int score = 0;
void setup() {
  //display setup
  your_score_dis.begin(YOUR_ADDRESS);
  best_score_dis.begin(BEST_ADDRESS);

  your_score_dis.setBrightness(15); //full brightness
  best_score_dis.setBrightness(15);

  your_score_dis.blinkRate(2); //no blinking - 0| 2 is faster than 3 blinking
  best_score_dis.blinkRate(3);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  your_score_dis.print(score);//writeDigitNum(4, 1);
  best_score_dis.print(2);//writeDigitNum(4, 2);
  your_score_dis.writeDisplay();
  best_score_dis.writeDisplay();

  score ++;
}
