//Base Include
#include <PanelManager.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG false;

//LED Array and Color Display LED Defines
#define LED_ARR   33
#define NUMPIXELS 54
Adafruit_NeoPixel pixels(NUMPIXELS, LED_ARR, NEO_GRBW + NEO_KHZ800);
uint16_t last_x = 0;
uint16_t last_y = 0;
#define LED_COLOR 32
Adafruit_NeoPixel p(1, LED_COLOR, NEO_GRBW + NEO_KHZ800);

//Touchscreen Defines
#define TX_TS 17
#define RX_TS 16
#define TSBAUD 9600
HardwareSerial touchScreen(2);

//Potentiometer Defines
#define RED_POT 15
uint8_t prev_red = 0;
#define GREEN_POT 27
uint8_t prev_green = 0;
#define BLUE_POT 12
uint8_t prev_blue = 0;

//Reset Button Defines
#define RESET 13
#define DEBOUNCE 1
int lastState = LOW;
int lastHeldState = LOW;
int currState;
unsigned long lastDebounceTime = 0;

//Setup Code Ran on Startup
void setup() {
  Serial.begin(115200);
  touchScreen.begin(TSBAUD, SERIAL_8N1, TX_TS, RX_TS);
  pixels.begin();
  pinMode(RESET, INPUT_PULLUP);
  Serial.println("SUCCESFUL STARTUP");
}

void loop() {
  //Code for creating the color to be painted onto the matrix
  uint8_t check = 0;
  uint8_t red = analogRead(RED_POT) / 16;
  uint8_t green = analogRead(GREEN_POT) / 16;
  uint8_t blue = analogRead(BLUE_POT) / 16;
  if(red - prev_red < 5) { red = prev_red; check++;}
  if(green - prev_green < 5) { green = prev_green; check++;}
  if(blue - prev_blue < 5) { blue = prev_blue; check++;}
  if(red < 15) red = 0;
  if(green < 15) green = 0;
  if(blue < 15) blue = 0;

  p.setPixelColor(0, red, green, blue);
  p.show();
  
  //Code for resetting all leds when the button is pressed
  currState = digitalRead(RESET);
  if (currState != lastState) {
    lastDebounceTime = millis();
    lastState = currState;
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE) {
    lastHeldState = currState;
  }
  if(lastHeldState == LOW){
    pixels.clear();
    pixels.show();
    Serial.println("LEDS RESET");
    return;
  }


  //Code for finding which section of the screen is pressed
  uint8_t bufferSize = 5;
  static byte buffer[5];
  static uint8_t index = 0;
  uint16_t x_coord;
  uint16_t y_coord;
  //while available keep reading in until we find the start byte
  while (touchScreen.available() && !x_coord && !y_coord) {
    byte b = touchScreen.read();
    if (index == 0 && b != 0x81) continue;
    //then we read in bytes until our buffer is full
    buffer[index++] = b;

    if (index == bufferSize) {
      //Parse X/Y coordinates
      x_coord = buffer[1] | (buffer[2] << 7);
      y_coord = buffer[3] | (buffer[4] << 7);
      index = 0;
    }
  }

  if(!x_coord || !y_coord) return;
  

  //Implement code for drawing to the pixel associated with that portion of screen
  uint16_t adjusted_X = ((x_coord - 200) / 425);
  uint16_t adjusted_Y = ((y_coord - 500) / 575);
  if(adjusted_X > 8) return;
  if(adjusted_Y > 5) return;
  if(adjusted_Y % 2) adjusted_X = (8 - adjusted_X);
  if(check == 3 && last_x == adjusted_X && last_y == adjusted_Y) return;
  else {
    last_x = adjusted_X;
    last_y = adjusted_Y;
  }

  uint16_t led_i = adjusted_X + (adjusted_Y * 9);
  pixels.setPixelColor(led_i, red, green, blue);
  pixels.show();

  delay(1);
}
