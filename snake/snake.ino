enum SnakeDirection{
  x_up,
  x_down,
  y_up,
  y_down,
  z_up,
  z_down
};
SnakeDirection snakeDirection;

typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned char z;
}CubeElement;

CubeElement snake[320] = {0};
int snakeSize = 320;
CubeElement apple;

//ledState[z][y][x]
bool ledState[5][8][8] = {0};



void setup() {
  // put your setup code here, to run once:
  snakeDirection = z_up;

  for(int i=0; i<snakeSize; i++){
    snake[i].x = 0;
    snake[i].y = 0;
    snake[i].z = 0;
  }

  for(int z=0; z<5; z++){
    for(int y=0; y<8; y++){
      for(int x=0; x<8; x++){
        ledState[z][y][x] = false;
      }
    }
  }

  //Ann

  //Jackie

  //Gio
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //Ann
  getButtonInput();

  //Jackie

  //Gio
  for(int i=0; i<snakeSize; i++){
    if(snake[i].x != 0 || snake[i].y != 0 || snake[i].z != 0){
      digitalWrite(LED_BUILTIN, LOW);
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  for(int z=0; z<5; z++){
    for(int y=0; y<8; y++){
      for(int x=0; x<8; x++){
        if(ledState[z][y][x]){
          digitalWrite(LED_BUILTIN, LOW);
        }
        else{
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
  }
  
}

void getButtonInput(){
  
}
