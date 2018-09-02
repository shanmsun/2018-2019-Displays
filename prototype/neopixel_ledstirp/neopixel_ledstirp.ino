#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel tester = Adafruit_NeoPixel (2, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel timer_bar = Adafruit_NeoPixel (7, 7, NEO_GRB + NEO_KHZ800);

uint32_t red = tester.Color(255, 0, 0); //red full brightness
uint32_t green = tester.Color(0, 255, 0); 
uint32_t blue = tester.Color(0, 0, 255); 
uint32_t yellow = tester.Color (125, 125, 0); // yellow = half red + half green

uint32_t no_colour = timer_bar.Color (0, 0, 0);
uint32_t green2 = timer_bar.Color(0, 255, 0); //red full brightness

long colour_num_code; //randomly chosen colour
uint32_t colour_chosen; 

//uncomment this and put in values later
//--------------------ROW 1-----------------------
//Adafruit_NeoPixel not1 = Adafruit_Neopixel ();
//Adafruit_NeoPixel not2 = Adafruit_Neopixel ();
//Adafruit_NeoPixel not3 = Adafruit_Neopixel ();
//--------------------ROW 2-----------------------
//Adafruit_NeoPixel up = Adafruit_Neopixel ();
//Adafruit_NeoPixel down = Adafruit_Neopixel ();
//Adafruit_NeoPixel left = Adafruit_Neopixel ();
//Adafruit_NeoPixel right = Adafruit_Neopixel ();
//--------------------ROW 3-----------------------
//Adafruit_NeoPixel or = Adafruit_Neopixel ();
//Adafruit_NeoPixel nothing = Adafruit_Neopixel ();
//--------------------ROW 4-----------------------
//Adafruit_NeoPixel green = Adafruit_Neopixel ();
//Adafruit_NeoPixel yellow = Adafruit_Neopixel ();
//Adafruit_NeoPixel blue = Adafruit_Neopixel ();
//Adafruit_NeoPixel red = Adafruit_Neopixel ();

void setup() {
    //initialize random number generator
  randomSeed (analogRead(0));
  tester.begin();
  tester.show(); //initialize it to "off"
  timer_bar.begin();
  timer_bar.show(); //initialize it to "off"
}

void loop() {
  //light_up_word();
  display_timer();
}
/*void light_up_word (){
  colour_num_code = random (1, 5); //random number b/w 1 to 4
  if (colour_num_code == 1){ //red is chosen
    colour_chosen = red;
  }
  else if (colour_num_code == 2){//green is chosen
    colour_chosen = green;
  }
  else if (colour_num_code == 3){//blue is chosen
    colour_chosen = blue;
  }
  else{//yellow is chosen
    colour_chosen = yellow;
  }

  //change the colour of all pixels
  for (uint16_t i = 0; i < tester.numPixels(); i++){
    tester.setPixelColor(i, colour_chosen);
  }

  tester.show();// show it all once
  delay (200); //2 second delay
}
*/
void display_timer(){
  uint16_t num_pixel = timer_bar.numPixels();
  unsigned long delay_time = 5000/num_pixel; //timer is 5 seconds
  uint8_t colour_change_factor = 255/num_pixel;

  //start off with all the LEDs fully on in red
  for (uint16_t i = 0; i < num_pixel; i++){
    timer_bar.setPixelColor(i, green2);
  }
  timer_bar.show();

  delay(delay_time);
  
  for (uint16_t i = 0; i < num_pixel ; i++){
    for (uint16_t j = 0; j < num_pixel - 1 - i ; j++){
      timer_bar.setPixelColor(j, (i+1) * colour_change_factor, 255 - (i+1) * colour_change_factor, 0);
    }
    timer_bar.setPixelColor(num_pixel - 1 - i, no_colour);
    timer_bar.show();
    delay(delay_time);
  }  
}

