#include <Adafruit_NeoPixel.h>
#include <PanelManager.h>

#define LEDPIN 12
#define LEDCOUNT 10//20
#define REDPOT 32
#define GRNPOT 33
#define BLUPOT A2 //34
#define TOTPOT A4 //36

Adafruit_NeoPixel strip(LEDCOUNT, LEDPIN, NEO_GRBW + NEO_KHZ800);
PanelManager panelManager("Panel-Backlit");

void setup() {
  Serial.begin(115200);
  panelManager.InitManager();
  analogSetPinAttenuation(REDPOT, ADC_11db);
  analogSetPinAttenuation(GRNPOT, ADC_11db);
  analogSetPinAttenuation(BLUPOT, ADC_11db);
  analogSetPinAttenuation(TOTPOT, ADC_11db);
  strip.begin();
  strip.show();
}

void loop() {
  int t = constrain((analogRead(TOTPOT) / 16), (panelManager.minBrightness * 255 / 100), panelManager.maxBrightness * 255 / 100);
  int r = constrain((analogRead(REDPOT) / 16) * t / 255, 0, 255);
  int g = constrain((analogRead(GRNPOT) / 16) * t / 255, 0, 255);
  int b = constrain((analogRead(BLUPOT) / 16) * t / 255, 0, 255);
  Serial.print("R: "); Serial.print(r);
  Serial.print(" | G: "); Serial.print(g);
  Serial.print(" | B: "); Serial.print(b);
  Serial.print(" | T: "); Serial.println(t);
  SetColor(r, g, b);
  delay(100);
}

void SetColor(int r, int g, int b) {
  for (int i = 0; i < LEDCOUNT; i++) strip.setPixelColor(i, r, g, b);
  strip.show();
}