

// const int joystick_x_pin = A0;
// const int joystick_y_pin = A1;

#include <Adafruit_NeoPixel.h>

// Joystick pins
#define VRX_PIN A0
#define VRY_PIN A1
#define SW_PIN 17

// Joystick thresholds
#define LEFT_THRESHOLD 1000
#define RIGHT_THRESHOLD 1000
#define UP_THRESHOLD 1000
#define DOWN_THRESHOLD 1000

// LED Grid configuration
#define COMMAND_NO 0x00
#define COMMAND_LEFT 0x01
#define COMMAND_RIGHT 0x02
#define COMMAND_UP 0x04
#define COMMAND_DOWN 0x08

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 12  // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 64  // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int x_value, y_value;
int player_x = 0;
int player_y = 0; // Start at 0,0;

int command = COMMAND_NO;
int dot = 0;

void
setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();  // Set all pixel colors to 'off'
  updateGrid();
}

void loop() {

  x_value = analogRead(VRX_PIN);
  y_value = analogRead(VRY_PIN);

  // If the x left joystick is moved, move player 1 dot to the left on the x axis
  if(x_value < LEFT_THRESHOLD) movePlayer(-1, 0); //Move left

  // If the x right joystick is moved, move player 1 dot to the right on the x axis
  if(x_value > RIGHT_THRESHOLD) movePlayer(1, 0); //Move right

  // If the y up joystick is moved, move player 1 dot up on the y axis
  if(y_value < UP_THRESHOLD) movePlayer(0, -1); //Move up

  // If the y down joystick is moved, move player 1 dot down on the y axis
  if(y_value > DOWN_THRESHOLD) movePlayer(0, 1); //Move down

  delay(200);
}

// dx is change in x movement, dy is change in y movement
void movePlayer(int dx, int dy)
  {

    // player_x and player_y store the current position of the player

    // player position is updated
    int new_x = player_x + dx;
    int new_y = player_y + dy;

    if(new_x >= 0 && new_x <= 8 && new_y >= 0 && new_y <= 8)
    {
      player_x = new_x;
      player_y = new_y;
      updateGrid();
    }
  }

  void updateGrid()
  {
    pixels.clear();
    int ledIndex = getLEDIndex(player_x, player_y);
    pixels.setPixelColor(ledIndex, pixels.Color(0, 255, 0)); //Green pixel
    pixels.show();
  }

  int getLEDIndex(int x, int y)
  {
    if(y % 2 == 0)
    {
      return y * 8 + x; //Left to right rows
    }

    else
    {
      return y * 8 + (7 - x); //Right to left rows
    }
  }
