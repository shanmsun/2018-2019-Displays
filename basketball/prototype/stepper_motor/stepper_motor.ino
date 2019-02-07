int by_en = 10;
int by_r = 9; //blue
int by_l = 8; //yellow

int rg_en = 7;
int rg_r = 5; //green
int rg_l = 6; //red


int ctrl_seq[4][6] = {{1, 1, 0, 0, 0, 0},
               {0, 0, 0, 1, 1, 0},
               {1, 0, 1, 0, 0, 0},
               {0, 0, 0, 1, 0, 1}};

int pin_seq[6] = {by_en, by_r, by_l, rg_en, rg_r, rg_l};
int t = 1000;
void setup() {
  // put your setup code here, to run once:
  pinMode (by_en, OUTPUT);
  pinMode (by_r, OUTPUT);
  pinMode (by_l, OUTPUT);

  pinMode (rg_en, OUTPUT);
  pinMode (rg_r, OUTPUT);
  pinMode (rg_l, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 6; j++){
      digitalWrite(pin_seq[j],ctrl_seq[i][j]);
      Serial.print(ctrl_seq[i][j]);
      delay(t);
    }
    Serial.println("next sequence");
  }
  
}
