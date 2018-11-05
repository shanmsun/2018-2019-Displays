#include <EEPROM.h>
#include "Adafruit_LEDBackpack.h"

//fixed constatnts
#define MAX_X 8
#define MAX_Y 5
#define MAX_Z 8
//in miliseconds
#define LAYER_TIMEOUT 1
#define DRAW_CUBE_NUM 1
#define MOVE_SNAKE_DELAY 30
#define SNAKE_SIZE_MAX 320
#define BLINK_APPLE_DELAY 15
#define GAME_OVER_DELAY 175
//for score display
#define PLAYERSCORE_ADDRESS 0x70
#define BESTSCORE_ADDRESS 0x77
//for button debounce
#define DEBOUNCE_TIME 70
//addresses for eeprom
#define BEST_SCORE_ADDR 0 //uses 0, 1 to score the best score
#define GAME_PLAY_COUNT_ADDR 2 //uses 2, 3 to store the times played.
#define GAME_SCORE_COUNT_ADDR 4 //base + player score*2. This 2 byte address will store how many ppl have gotten this score.

Adafruit_7segment playerScore_dis = Adafruit_7segment();
Adafruit_7segment bestScore_dis = Adafruit_7segment();

//pins for the led cube
const int LED_PIN_X[MAX_Y][MAX_X] = {
    {37,36,35,34,38,40,42,43},          //A
    {15,16,3,2,23,41,39,17},  //B
    {14,24,22,30,18,28,19,29},  //C
    {13,31,32,25,27,33,26,4},  //D
    {12,11,10,9,8,7,6,5}   //E
};

const int LED_PIN_Z[MAX_Z] = {
  46,51,44,45,47,48,49,50       //starting from te bottom{1, 2, 3, 4, 5, 6, 7, 8}
};

enum SnakeDirection {
  x_up,
  x_down,
  y_up,
  y_down,
  z_up,
  z_down
};
SnakeDirection snakeDirection;

enum GameState {
  on,
  off,
  win,
  ate_body,
  hit_wall
};
GameState gamestate;

struct CubeElement {
  unsigned char x;
  unsigned char y;
  unsigned char z;
  bool operator==(const CubeElement &ce) const;
  void operator=(const CubeElement &ce);
};

bool CubeElement::operator==(const CubeElement &ce) const {
  return (x == ce.x) && (y == ce.y) && (z == ce.z);
}

void CubeElement::operator=(const CubeElement &ce) {
  x = ce.x;
  y = ce.y;
  z = ce.z;
}

int playerScore = 0;
int bestScore = 0;

int joystick_up = A0; //14
int joystick_down = A1; //15
int joystick_right = A2; //16
int joystick_left = A3; //17
int button_up = A4; //18
int button_down = A5; //19
int start = A6; //20

int input_state[6][2] = {
  {joystick_up, 1}, //{pin #, recent_State}
  {joystick_down, 1},
  {joystick_right, 1},
  {joystick_left, 1},
  {button_up, 1},
  {button_down, 1}
};

int start_button_state[2] = {start, 1};
int input_state_len = sizeof(input_state)/sizeof(input_state[0]);

int snakeSize = 2; //max is 320
int snakeMoveDelay = 0;
CubeElement snake[SNAKE_SIZE_MAX] = {0};
CubeElement apple;
int blinkApple = 0;
int gameOverDelay = 0;
int gameOverDelayCount = 0;

//ledState[z][y][x]
bool ledState[MAX_Z][MAX_Y][MAX_X] = {0};

void initSnakeGame(){
  snakeDirection = z_up;
  playerScore = 0;
  blinkApple = 0;
  gameOverDelay = 0;
  gameOverDelayCount = 0;

  for (int i = 0; i < SNAKE_SIZE_MAX; i++) {
    snake[i].x = 0;
    snake[i].y = 0;
    snake[i].z = 0;
  }
  snake[0].z = 1;

  snakeSize = 2;

  apple.x = random(8);
  apple.y = random(1,5);
  apple.z = random(8);
  for (int z=0; z<MAX_Z; z++){
    for(int x=0; x<MAX_X; x++){
      for(int y=0; y<MAX_Y; y++){
        ledState[z][y][x] = false;
      }
    }
  }
  ledState[snake[0].z][snake[0].y][snake[0].x] = true;
  ledState[apple.z][apple.y][apple.x] = true;
}

void setup() {
  // put your setup code here, to run once:
  bestScore = readIntEeprom(BEST_SCORE_ADDR);
  //Ann
  //7seg setup
  playerScore_dis.begin(PLAYERSCORE_ADDRESS);
  bestScore_dis.begin(BESTSCORE_ADDRESS);

  playerScore_dis.setBrightness(15); //full brightness
  bestScore_dis.setBrightness(15);

  playerScore_dis.blinkRate(0); //no blinking
  bestScore_dis.blinkRate(0);

  pinMode(joystick_up, INPUT_PULLUP);
  pinMode(joystick_down, INPUT_PULLUP);
  pinMode(joystick_right, INPUT_PULLUP);
  pinMode(joystick_left, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);

  //Jackie
  randomSeed(analogRead(0));
  gamestate = off;
  initSnakeGame();

  //Gio
  //set the led cube pins as outputs
  for(int x=0; x<MAX_X; x++){
    for(int y=0; y<MAX_Y; y++){
      pinMode(LED_PIN_X[y][x], OUTPUT);
      digitalWrite(LED_PIN_X[y][x], LOW);
    }
  }
  //set the led cube z pins output
  for(int z=0; z<MAX_Z; z++){
    pinMode(LED_PIN_Z[z], OUTPUT);
    digitalWrite(LED_PIN_Z[z], HIGH);
  }

}


bool upd_ledmtx() {
  //storing snake head
  //for the later body update
  CubeElement prev_head;
  prev_head = snake[0];

  //updating the snake head
  gamestate = hit_wall;
  switch (snakeDirection) {
  case x_up:
    if (snake[0].x == 7) return false;
    snake[0].x++;
    break;

  case x_down:
    if (snake[0].x == 0) return false;
    snake[0].x--;
    break;

  case y_up:
    if (snake[0].y == 4) return false;
    snake[0].y++;
    break;

  case y_down:
    if (snake[0].y == 0) return false;
    snake[0].y--;
    break;

  case z_up:
    if (snake[0].z == 7) return false;
    snake[0].z++;
    break;

  case z_down:
    if (snake[0].z == 0) return false;
    snake[0].z--;
    break;

  default: break;
  }
  gamestate = on;

  //ate its body
  for (int i = 1; i < snakeSize; i++) {
    if (snake[0] == snake[i]) {
      gamestate = ate_body;
      return false;
    }
  }

  //updating the led mtx
  //lighting up head's new position
  ledState[snake[0].z][snake[0].y][snake[0].x] = true;

  //got apple, length+1
  if (snake[0] == apple) {
    playerScore++;
    snakeSize++;
  }
  //erasing previous tail's position
  else {
    if (snakeSize ==  1)
      ledState[prev_head.z][prev_head.y][prev_head.x] = false;
    else
      ledState[snake[snakeSize - 1].z][snake[snakeSize - 1].y][snake[snakeSize - 1].x] = false;
  }

  //updating the snake body
  for (int i = snakeSize - 1; i > 0; i--) {
    if (i == 1) snake[i] = prev_head;
    else snake[i] = snake[i - 1];
  }

  //generate new apple
  if (snake[0] == apple) {
    int cnt = random(320 - snakeSize);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 8; k++) {
          //find the cnt'th spot in led that is free
          if (ledState[i][j][k] == false) cnt--;
          if (cnt < 0) {
            apple.x = k;
            apple.y = j;
            apple.z = i;

            ledState[apple.z][apple.y][apple.x] = true;
            return true;
          }
        }
      }
    }

  }
  else
    ledState[apple.z][apple.y][apple.x] = true;
  return true;
}

/**
 * Reads the current values of ledState. If the value is true, then it will turn the led on
 * It loops through all the layers, turning only 1 layer on at a time.
 * The delay is set to LAYER_TIMEOUT
 */
void setLedStateToCube(){
  //blink the apple
  if(blinkApple < BLINK_APPLE_DELAY){
    ledState[apple.z][apple.y][apple.x] = true;
    blinkApple++;
  }
  else if(blinkApple < BLINK_APPLE_DELAY*2){
    ledState[apple.z][apple.y][apple.x] = false;
    blinkApple++;
  }
  else{
    ledState[apple.z][apple.y][apple.x] = false;
    blinkApple = 0;
  }

  for(int z=0; z<MAX_Z; z++){
    for(int y=0; y<MAX_Y; y++){
      for(int x=0; x<MAX_X; x++){
        if(ledState[z][y][x]){
          digitalWrite(LED_PIN_X[y][x], HIGH);
        }
        else{
          digitalWrite(LED_PIN_X[y][x], LOW);
        }
      }
    }
    digitalWrite(LED_PIN_Z[z], LOW);
    //show the current layer before clearing to the next one
    delay(LAYER_TIMEOUT);
    //turn the layer off before starting the next one
    digitalWrite(LED_PIN_Z[z], HIGH);
  }
  return;
}

/**
 * We want to keep the appearance that all the leds are on at the same time,
 * but once we loop through the MAX_Z layers, if we return to update the snake location,
 * we will move the snake once every (8*LAYER_TIMEOUT) (once every 32 milisecons). To
 * prevent this, we will draw the led cube until MOVE_SNAKE_DELAY has been reached. At
 * which point, we will return to allow the snake position to update.
 */
void drawLedCube(){
  for(int i=0; i<DRAW_CUBE_NUM; i++)
    setLedStateToCube();
}

/**
 * Set one led high, wait 500 msec then set it low, and turn on the next one
 */
void testEachLed(){
  for(int z=0; z<MAX_Z; z++){
    digitalWrite(LED_PIN_Z[z], LOW);
    for(int y=0; y<MAX_Y; y++){
      for(int x=0; x<MAX_X; x++){
        digitalWrite(LED_PIN_X[y][x], HIGH);
        delay(500);
        digitalWrite(LED_PIN_X[y][x], LOW);
      }
    }
    digitalWrite(LED_PIN_Z[z], HIGH);
  }
}

/**
 * Test the layers of the led cube
 */
void testLayers(){
  digitalWrite(LED_PIN_X[0][0], HIGH);
  digitalWrite(LED_PIN_X[MAX_Y-1][0], HIGH);
  digitalWrite(LED_PIN_X[0][MAX_X-1], HIGH);
  digitalWrite(LED_PIN_X[MAX_Y-1][MAX_X-1], HIGH);

   for(int z=0; z<MAX_Z; z++){
    digitalWrite(LED_PIN_Z[z], LOW);
    delay(500);
    digitalWrite(LED_PIN_Z[z], HIGH);
  }
}

void display_score(int playerScore){
  // when playerScore exceeds bestScore, increment & display it
  if (playerScore > bestScore){
    bestScore = playerScore;
    writeIntEeprom(BEST_SCORE_ADDR, bestScore);
  }

  playerScore_dis.print(playerScore);
  playerScore_dis.writeDisplay();
  bestScore_dis.print(bestScore);
  bestScore_dis.writeDisplay();
}

//pressed -> read = 0
//open -> read = 1 due to INPUT_PULLUP
bool start_button_debounce(int input[2]){
  int button_readVal1 = digitalRead(input[0]);

  if (button_readVal1 != input[1]){
    delay (DEBOUNCE_TIME);
    int button_readVal2 = digitalRead(input[0]);

    if (button_readVal1 == button_readVal2){ //still the same input value -> not a noise
      //Serial.println("state changed");
      input[1] = button_readVal2;
      if (button_readVal2 == 0){//button is pressed
        Serial.println("pressed");
        return true;
      }
      else{
        Serial.println("released");
      }
    }
  }
  return false;
}

bool button_debounce(int input[2]){
  int button_readVal1 = digitalRead(input[0]);

  if (button_readVal1 != input[1]){
    //delay (DEBOUNCE_TIME);
    setLedStateToCube();
    int button_readVal2 = digitalRead(input[0]);

    if (button_readVal1 == button_readVal2){ //still the same input value -> not a noise
      //Serial.println("state changed");
      input[1] = button_readVal2;
      if (button_readVal2 == 0){//button is pressed
        Serial.println("pressed");
        return true;
      }
      else{
        Serial.println("released");
      }
    }
  }
  return false;
}

void determine_inputDirection(int button_pin){
      Serial.println(button_pin); //debugging
      switch (button_pin){
        case A0: //14
          if (snakeDirection != y_down){
            Serial.println("y_up");
            snakeDirection = y_up;
          }
          break;

        case A1: //15
          if (snakeDirection != y_up){
            Serial.println("y_down");
            snakeDirection = y_down;
          }
          break;

        case A2: //16
          if (snakeDirection != x_down){
            Serial.println("x_up");
            snakeDirection = x_up;
          }
          break;

        case A3: //17
          if (snakeDirection != x_up){
            Serial.println("x_down");
            snakeDirection = x_down;
          }
          break;

        case A4: //18
          if (snakeDirection != z_down){
            Serial.println("z_up");
            snakeDirection = z_up;
          }
          break;

        case A5: //19
          if (snakeDirection != z_up){
            Serial.println("z_down");
            snakeDirection =  z_down;
          }
          break;
      }
}

void getInput(){
   for (int i = 0; i < input_state_len; i++){
      bool button_pressed = button_debounce(input_state[i]);
      if (button_pressed){
        determine_inputDirection(input_state[i][0]);
      }
  }
}

void clearLedState(){
  for(int z=0; z<MAX_Z; z++){
    for(int y=0; y<MAX_Y; y++){
      for(int x=0; x<MAX_X; x++){
        ledState[z][y][x] = false;
      }
    }
  }
}
void loop() {
  /*(// put your main code here, to run repeatedly:
  testEachLed();*/
  if (start_button_debounce(start_button_state)){
    //add one to play count
    incrementIntEeprom(GAME_PLAY_COUNT_ADDR);
    //store the previous players score
    incrementIntEeprom(GAME_SCORE_COUNT_ADDR + playerScore*2);

    gamestate = on;
    initSnakeGame();
  }

  if (gamestate == on){
    //Ann
    getInput();

    //Jackie
    if(snakeMoveDelay == MOVE_SNAKE_DELAY){
      upd_ledmtx();
      //display_score(playerScore);
      snakeMoveDelay = 0;
    }
    else{
      snakeMoveDelay++;
    }

    //Gio
    drawLedCube();

  }
  else{
    if(gameOverDelayCount < 8){
      if(gameOverDelay < GAME_OVER_DELAY){
        gameOverDelay++;
      }
      else if(gameOverDelay < GAME_OVER_DELAY*2){
        setLedStateToCube();
        gameOverDelay++;
      }
      else{
        gameOverDelay = 0;
        gameOverDelayCount++;
      }
    }
    else {
      clearLedState();
    }
  }
  display_score(playerScore);

}

/*
 * Write the the int to the address in EEPROM
 *
 * Storing an int requires sizeof(int) {2} address indexes, starting from address
 *
 * @return int of the next available address after the write
 */
int writeIntEeprom(int address, int val){
  int i;
  Serial.print("Writing ");
  Serial.print(val);
  Serial.print(" to 0x");
  Serial.println(address);
  for(i=0; i<sizeof(int); i++){
    EEPROM.write(address + i,
      (byte)((val>>i*8)&0x0FF));
  }
  return address + i;
}

/*
 * Read an int in eeprom at the address.
 *
 * @return int value stored in memory.
 */
int readIntEeprom(int address){
  int val = 0;
  int part;
  for(int i=0; i<sizeof(int); i++){
    part = EEPROM.read(address + i);
    val += (part<<i*8);
  }
  Serial.print("Read ");
  Serial.print(val);
  Serial.print(" from 0x");
  Serial.println(address);
  return val;
}

 /*
  * Increment value in memory by one and save it.
  */
void incrementIntEeprom(int address){
  int val = readIntEeprom(address);
  val += 1;
  writeIntEeprom(address, val);
}
