const int led1 = 4;
const int led2 = 8;

const int button1 = 6;
const int button2 = 7;

int prev_button_state = 1;
int cur_button_state;
int led_state = 0;
void setup() {
  // put your setup code here, to run once:
  //set 2 LEDs to output
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  //set 2 buttons to input
  pinMode (button1, INPUT_PULLUP);
  pinMode (button2, INPUT_PULLUP);
  //LEDs are off at the start
  digitalWrite (led1, HIGH);
  led_state = 1;
  digitalWrite (led2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1_read = digitalRead(button1);

  //---------------------dealing with debounce with 1 button-----------------------------//
  if (button1_read != prev_button_state){
    //delay (100);
    
    if (button1_read == digitalRead(button1)){
      cur_button_state = button1_read;

      if (cur_button_state == 0){//button is pressed
        //check the answer
        if (led_state == 1){
          digitalWrite (led2, HIGH);
        }
      }
      prev_button_state = cur_button_state; //update the button state
    }
  }
}
