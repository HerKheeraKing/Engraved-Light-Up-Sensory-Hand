//Base Include
#include <PanelManager.h>
#include <Adafruit_NeoPixel.h>

#define LED_ARR   14
#define NUMPIXELS 54
Adafruit_NeoPixel pixels(NUMPIXELS, LED_ARR, NEO_GRBW + NEO_KHZ800);
#define LED_COLOR A1


#define SCREEN A5
#define RED_POT A4
uint8_t prev_red = 0;
#define GREEN_POT A3
uint8_t prev_green = 0;
#define BLUE_POT A2
uint8_t prev_blue = 0;

#define RESET 12
#define DEBOUNCE 1
int lastState = LOW;
int lastHeldState = LOW;
int currState;
unsigned long lastDebounceTime = 0;

bool test = true;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pinMode(RESET, INPUT_PULLDOWN);
}

void loop() {

  if(test)
  {
    uint8_t r = 30;
    uint8_t g = 30;
    uint8_t b = 30;

    for(int i=0; i<NUMPIXELS + 1; i++) {
      pixels.setPixelColor(i, r, g, b);
      pixels.show();
    }
    delay(100);
    return;
  }

  //Code for creating the color to be painted onto the matrix
  uint8_t red = analogRead(RED_POT) / 16;
  Serial.print("RED VAL:");
  Serial.println(red);
  uint8_t green = analogRead(GREEN_POT) / 16;
  Serial.print("GREEN VAL:");
  Serial.println(green);
  uint8_t blue = analogRead(BLUE_POT) / 16;
  Serial.print("BLUE VAL:");
  Serial.println(blue);
  Serial.println();



  //Implement code for finding which section of the screen is pressed



  //float ratio = analogRead(SOFT_POT)/ 4095.0f;
  //int light = (ratio * NUMPIXELS) - 1;
  //Serial.println(analogRead(SOFT_POT));
  //Serial.println(light);


  //Implement code for drawing to the pixel associated with that portion of screen
  





  if(lastHeldState == HIGH){
    pixels.clear();
    pixels.show();
    Serial.println("LEDs reset");
  }
  else{
    if(prev_red - red > 5) red = prev_red;
    if(prev_green - green > 5) green = prev_green;
    if(prev_blue - blue > 5) blue = prev_blue;
      for(int i=0; i<NUMPIXELS + 1; i++) {
      pixels.setPixelColor(i, red, green, blue);
      pixels.show();
    }
  }
  //for(int i=0; i<NUMPIXELS + 1; i++) {
    //if(i == light)
      //pixels.setPixelColor(i, pixels.Color(red, green, blue));
    //else
      //pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    //pixels.show();
  //}


  //Code for resetting all pins when the button is pressed
  currState = digitalRead(RESET);
  Serial.println(currState);
  Serial.println();
  if (currState != lastState) {
    lastDebounceTime = millis();
    lastState = currState;
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE) {
    lastHeldState = currState;
  }
  
  

  delay(10);
}
