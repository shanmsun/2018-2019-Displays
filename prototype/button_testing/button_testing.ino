#include <Adafruit_NeoPixel.h>

const int ledpin = 6;
Adafruit_NeoPixel tester = Adafruit_NeoPixel (3, ledpin, NEO_GRB + NEO_KHZ800); //3 pixels, pin7
const int button1 = 20;
int prev_button_state = 1;
int cur_button_state;
int led_state = 0;

void setup() {
  //set 2 buttons to input
  pinMode (button1, INPUT);
  tester.begin();
  tester.show(); //initialize it to "off"
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1_read = digitalRead(button1);

  //---------------------dealing with debounce with 1 button-----------------------------//
  if (button1_read != prev_button_state){
    delay (100);
    if (button1_read == digitalRead(button1)){
      cur_button_state = button1_read;
      if (cur_button_state == 0){//button is pressed
        if (led_state == 0){
          tester.setPixelColor(0, 255, 0, 0); //light up the LED
          tester.show();
          led_state = 1;
        }
        else {
          tester.setPixelColor(0, 0, 0, 0); //turn off the LED
          tester.show();
          led_state = 0;
        }
        
      }
      prev_button_state = cur_button_state; //update the button state
    }
  }
}
