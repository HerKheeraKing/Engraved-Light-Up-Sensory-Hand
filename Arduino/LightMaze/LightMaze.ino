

// const int joystick_x_pin = A0;
// const int joystick_y_pin = A1;

#include <Adafruit_NeoPixel.h>

#define VRX_PIN A0
#define VRY_PIN A1
#define SW_PIN 17

#define LEFT_THRESHOLD 1000
#define RIGHT_THRESHOLD 1000
#define UP_THRESHOLD 1000
#define DOWN_THRESHOLD 1000

#define COMMAND_NO 0x00
#define COMMAND_LEFT 0x01
#define COMMAND_RIGHT 0x02
#define COMMAND_UP 0x04
#define COMMAND_DOWN 0x08

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 12  // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 81  // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int x_value = 0;
int y_value = 0;
int command = COMMAND_NO;
int b_value = 0;
int dot = 0;
unsigned long nextupdate = 0;
int test[2][10] = {
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 }
    };  

void
setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {

  pixels.clear();  // Set all pixel colors to 'off'

  x_value = analogRead(VRX_PIN);
  y_value = analogRead(VRY_PIN);

  //b_value = button.getState();

  // if joystick (y) analog value is down to 0, lights increment down to A_0

  // if joystick (y) analog value is all the way to 4095, lights increment up to A_10


  //print data to Serial Monitor on Arduino IDE
  // Serial.print("x = ");
  // Serial.print(x_value);
  // Serial.print(", y = ");
  // Serial.println(y_value);


  // if (y_value == 0)
  // {
  //   pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  // }

  // if()

  //if (y_value > 2048)
  //{
  // for (int i=0; i<NUMPIXELS; i++) {

  //pixels.setPixelColor(dot, pixels.Color(0, 150, 0));
  //}
  //}

  for (int i = 0; i < dot; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 60, 0));
  }

  unsigned long now = millis();
  if (now > nextupdate) {
    nextupdate = now + 100;
    if (y_value > 2248) {
      if (dot < test[0][9])
      {
        dot++;
      }
    } 
    
    else if (y_value < 1848) {
      dot--;
    }

    else if (x_value < 1848) {
      if (dot >= 9) {
        dot++;
      }
    }

    else if (x_value > 2248) {
      if (dot >= 12) {
        dot--;
      }
    }

    if (dot < 0) {
      dot = 0;
    }

    else if (dot >= NUMPIXELS) {
      dot = NUMPIXELS - 1;
    }

    Serial.println(dot);
  }

  pixels.show();  // Send the updated pixel colors to the hardware.

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  // for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

  //   // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  //   // Here we're using a moderately bright green color:
  //   pixels.setPixelColor(i, pixels.Color(0, 150, 0));

  // }


  // put your main code here, to run repeatedly:

  // int x_value, int y_value;
  // float x_volt, y_value;

  // x_value = analogRead(joystick_x_pin);
  // y_value = analogRead(joystick_y_pin);

  // x_volt = ((x_value * 3.3) / 4095);
  // y_volt = ((y_value * 3.3) / 4095);

  // Serial.print("X_Voltage = ");
  // Serial.print(x_volt);
  // Serial.print("\t");
  // Serial.print("Y_Voltage = ");
  // Serial.println(y_volt);
  // delay(100);
}
