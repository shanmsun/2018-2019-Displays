#define DEBOUNCE_TIME 70

const int joystick_up = A0;
const int joystick_down = A1;
const int joystick_right = A2;
const int joystick_left = A3;
const int button_up = A4;
const int button_down = A5;

enum SnakeDirection {
  x_up,
  x_down,
  y_up,
  y_down,
  z_up,
  z_down
};
SnakeDirection snakeDirection;

int input_state[6][2] = {
  {joystick_up, 1}, //{pin #, recent_State}
  {joystick_down, 1},
  {joystick_left, 1},
  {joystick_right, 1},
  {button_up, 1},
  {button_down, 1}
};

int input_state_len = sizeof(input_state)/sizeof(input_state[0]);

void setup() {
  // put your setup code here, to run once:
  pinMode(joystick_up, INPUT_PULLUP);
  pinMode(joystick_down, INPUT_PULLUP);
  pinMode(joystick_right, INPUT_PULLUP);
  pinMode(joystick_left, INPUT_PULLUP);

  Serial.begin(9600);
}

//pressed -> read = 0
//open -> read = 1 due to INPUT_PULLUP
bool button_debounce(int input[2]){
  int button_readVal1 = digitalRead(input[0]);
  
  if (button_readVal1 != input[1]){
    delay (DEBOUNCE_TIME);
    int button_readVal2 = digitalRead(input[0]);
    
    if (button_readVal1 == button_readVal2){ //still the same input value -> not a noise
      input[1] = button_readVal2;
      if (button_readVal2 == 0){//button is pressed
        return true;
      }
      else{
        return false;
      }
    }
  }
}

void determine_inputDirection(int button_pin){
      switch (button_pin){
        
      case A0:
        if (snakeDirection != y_down){
          Serial.println("y_up");
          snakeDirection = y_up;            
        }
   
      case A1:
        if (snakeDirection != y_up){
          Serial.println("y_down");
          snakeDirection = y_down;
        }

      case A2:
        if (snakeDirection != x_down){
          Serial.println("x_up");
          snakeDirection = x_up;
        }

      case A3:
        if (snakeDirection != x_up){
          Serial.println("x_down");
          snakeDirection = x_down;          
        }

      case A4:
      if (snakeDirection != z_down){
        Serial.println("z_up");
        snakeDirection = z_up;
      }
      
      case A5:
        if (snakeDirection != z_down){
          Serial.println("z_down");
          snakeDirection =  z_down;  
        }
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

void loop() {
  getInput();
}

