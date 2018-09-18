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

int score = 0;
int snakeSize = 320;
CubeElement snake[320] = {0};
CubeElement apple;

//ledState[z][y][x]
bool ledState[5][8][8] = {0};



void setup() {
  // put your setup code here, to run once:
  snakeDirection = z_up;

  for (int i = 0; i < snakeSize; i++) {
    snake[i].x = 0;
    snake[i].y = 0;
    snake[i].z = 0;
  }

  for (int z = 0; z < 5; z++) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        ledState[z][y][x] = false;
      }
    }
  }

  //Ann

  //Jackie
  //init snake at (0,0,0)
  //init apple randomly
  //init gamestate = on
  randomSeed(analogRead(0));
  apple.x = random(8);
  apple.y = random(8);
  apple.z = random(5);
  
  gamestate = on;
  ledState[snake[0].z][snake[0].y][snake[0].x] = true;
  ledState[apple.z][apple.y][apple.x] = true;

  //Gio
  pinMode(LED_BUILTIN, OUTPUT);

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
    if (snake[0].y == 7) return false;
    snake[0].y++;
    break;

  case y_down:
    if (snake[0].y == 0) return false;
    snake[0].y--;
    break;

  case z_up:
    if (snake[0].z == 4) return false;
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
    score++;
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
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 8; j++) {
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


void loop() {
  // put your main code here, to run repeatedly:

  //Ann
  getButtonInput();

  //Jackie
  upd_ledmtx();

  //Gio
  for (int i = 0; i < snakeSize; i++) {
    if (snake[i].x != 0 || snake[i].y != 0 || snake[i].z != 0) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  for (int z = 0; z < 5; z++) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        if (ledState[z][y][x]) {
          digitalWrite(LED_BUILTIN, LOW);
        }
        else {
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
  }

}

void getButtonInput() {

}
