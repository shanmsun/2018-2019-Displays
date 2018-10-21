#include "Adafruit_LEDBackpack.h"

#define PLAYERSCORE_ADDRESS 0x70
#define BESTSCORE_ADDRESS 0x77

Adafruit_7segment playerScore_dis = Adafruit_7segment();
Adafruit_7segment bestScore_dis = Adafruit_7segment();

int playerScore = 0;
int bestScore = 15;

void setup() {
  //display setup
  playerScore_dis.begin(PLAYERSCORE_ADDRESS);
  bestScore_dis.begin(BESTSCORE_ADDRESS);

  playerScore_dis.setBrightness(15); //full brightness
  bestScore_dis.setBrightness(15);

  playerScore_dis.blinkRate(0); //no blinking
  bestScore_dis.blinkRate(0);
}

void loop() {
  for (int i = 0; i < 20; i++){
    display_score(i);
    delay(3000);
  }
}

void display_score(int playerScore){
  // when playerScore exceeds bestScore, increment & display it
  if (playerScore > bestScore){
    bestScore = playerScore;
  }
  
  playerScore_dis.print(playerScore);
  playerScore_dis.writeDisplay();
  bestScore_dis.print(bestScore);
  bestScore_dis.writeDisplay();


}

