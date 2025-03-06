#include <BluetoothSerial.h>
#include "esp_bt.h"
#include "esp_gap_bt_api.h"

#define CHAR_BUFFER_SIZE 64
#define LED_PIN 13 
#define POT_PIN 15

float maxBrightness = 100;
float minBrightness = 0;
float maxVolume = 100;
float minVolume = 0;
float ratio = 255 / 100;

const char* deviceName = "Panel-Tester"; 
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  SerialBT.begin(deviceName);
  
  
  esp_bt_controller_config_t bt_config = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  esp_bt_controller_init(&bt_config);
  esp_bt_controller_enable(ESP_BT_MODE_BTDM);
  esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
}

void loop() {
  char *msg = SerialReadLineChar();
  if (msg != nullptr)
    ProcessMessage(msg);

  analogWrite(LED_PIN, CMapBrightness(analogRead(POT_PIN) / 16, minBrightness, maxBrightness));
  delay(1);
}

bool ProcessMessage(char *cmd) {
  char *header = strtok(cmd, ":");
  char *body = strtok(nullptr, "");
  if (header == nullptr || body == nullptr) {
    Serial.println("Error: Command not recognized, or invalid value");
    return false;
  }

  //UPDATE:maxBrightess,minBrightness,maxVolume,minVole
  if (strcasecmp(header, "UPDATE") == 0) {
    char *maxB_str = strtok(body, ",");
    char *minB_str = strtok(nullptr, ",");
    char *maxV_str = strtok(nullptr, ",");
    char *minV_str = strtok(nullptr, ",");
    if (maxB_str && minB_str && maxV_str && minV_str) {
      maxBrightness = atof(maxB_str);
      minBrightness = atof(minB_str);
      maxVolume = atof(maxV_str);
      minVolume = atof(minV_str);
      return true;
    }
  }

  Serial.println("Error: Command not handled");
  return false;
}

char *SerialReadLineChar() {
  char c;
  bool msgRecieved = false;
  static uint8_t index = 0;
  static char charBuff[CHAR_BUFFER_SIZE] = {0};
  while (SerialBT.available() > 0) {
    c = (char)SerialBT.read();
    if ((c == '\r') || (c == '\n')) {
      if (index == 0) continue;
      charBuff[index] = '\0';
      msgRecieved = true;
      index = 0;
      break;
    }
    charBuff[index++] = c;
    delay(1);
  }
  return (msgRecieved ? charBuff : nullptr);
}
 
int CMapBrightness(float input, float min, float max) {  return constrain(input, min * ratio, max * ratio); }
