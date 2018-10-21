//fixed constatnts
#define MAX_X 2
#define MAX_Y 2
#define MAX_Z 2
//in miliseconds
#define LAYER_TIMEOUT 4
#define MOVE_SNAKE_DELAY 3000

//pins for the led cube
const int LED_PIN_X[MAX_Y][MAX_X] = {
    {2,3},
    {4,5}
};

const int LED_PIN_Z[MAX_Z] = {
  6,7
};

//ledState[z][y][x]
bool ledState[MAX_Z][MAX_Y][MAX_X] = {0};

/**
 * Reads the current values of ledState. If the value is true, then it will turn the led on
 * It loops through all the layers, turning only 1 layer on at a time.
 * The delay is set to LAYER_TIMEOUT
 */
void setLedStateToCube(){
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
    digitalWrite(LED_PIN_Z[z], HIGH);
    //show the current layer before clearing to the next one
    delay(LAYER_TIMEOUT);
    //turn the layer off before starting the next one
    digitalWrite(LED_PIN_Z[z], LOW);
  }
  delay(LAYER_TIMEOUT);
  delay(LAYER_TIMEOUT);
  delay(LAYER_TIMEOUT);
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
  for(int i=0; i<(MOVE_SNAKE_DELAY/LAYER_TIMEOUT*MAX_Z); i++)
    setLedStateToCube();
}

void setup() {
  // put your setup code here, to run once:
  for(int x=0; x<MAX_X; x++){
    for(int y=0; y<MAX_Y; y++){
      pinMode(LED_PIN_X[y][x], OUTPUT);
    }
  }
  //set the led cube z pins output
  for(int z=0; z<MAX_Z; z++){
    pinMode(LED_PIN_Z[z], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ledState[0][0][0] = !ledState[0][0][0];
  ledState[1][1][1] = !ledState[1][1][1];

  drawLedCube();
}
