#include <Adafruit_NeoPixel.h>
#include <PanelManager.h>

#define LEDPIN 12
#define LEDCOUNT 10
#define REDPOT 32
#define GRNPOT 33
#define BLUPOT 34
#define TOTPOT 36

Adafruit_NeoPixel strip(LEDCOUNT, LEDPIN, NEO_GRBW + NEO_KHZ800);
PanelManager panelManager("Panel-1");

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
  int r = analogRead(REDPOT) / 16;
  int g = analogRead(GRNPOT) / 16;
  int b = analogRead(BLUPOT) / 16;
  int t = analogRead(TOTPOT) / 16;
  int minT = panelManager.minBrightness * 255 / 100;
  int maxT = panelManager.maxBrightness * 255 / 100;
  t = constrain(t, minT, maxT);
  r = constrain(r * t / 255, 0, 255);
  g = constrain(g * t / 255, 0, 255);
  b = constrain(b * t / 255, 0, 255);
  Serial.print("MaB: "); Serial.print(panelManager.maxBrightness); 
  Serial.print(" | MiB: "); Serial.print(panelManager.minBrightness);
  Serial.print(" | Temp: "); Serial.println(temperatureRead()); 
  Serial.println("Red: " + String(r) + " | Green: " + String(g) + " | Blue: " + String(b) + " | Brightness: " + String(t));
  SetColor(r, g, b);
  delay(100);
}

void SetColor(int r, int g, int b) {
  for (int i = 0; i < LEDCOUNT; i++) strip.setPixelColor(i, r, g, b);
  strip.show();
}