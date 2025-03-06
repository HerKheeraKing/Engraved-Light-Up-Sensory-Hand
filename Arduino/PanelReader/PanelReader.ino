#include <BluetoothSerial.h>
#include <Preferences.h>
#include "esp_bt.h"
#include "esp_gap_bt_api.h"

#define CHAR_BUFFER_SIZE 32
#define OUTPUTSTREAM SerialBT

BluetoothSerial SerialBT;
Preferences prefs;
const char device_name[] PROGMEM = "Panel-Backlit"; 
const char pref_namespace[] PROGMEM = "UserPrefs"; 
String currentUser = "NULL";
float maxBrightness = 100, minBrightness = 0, maxVolume = 100, minVolume = 0;
char otherPanels[5][32]; 
int slaveCount = 0;

void scanCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
  switch (event) {
    case ESP_BT_GAP_DISCOVERY_STARTED:
      Serial.println("Discovery started");
      break;
    case ESP_BT_GAP_DISC_RES_EVT:
      // Ensure the Bluetooth address is available
      if (param->disc_res.bda != nullptr) {
        // Print out the BD address (Bluetooth address)
        Serial.print("Found device with address: ");
        for (int i = 0; i < 6; i++) {
          Serial.printf("%02X", param->disc_res.bda[i]);
          if (i < 5) {
            Serial.print(":");
          }
        }
        Serial.println();

        // Connect to the device using its BD address
        if (SerialBT.connect(param->disc_res.bda)) {
          Serial.println("✅ Connected successfully!");
          SerialBT.println("Hello, slave!");
        } else {
          Serial.println("❌ Failed to connect.");
        }
      }
      break;
    default:
      break;
  }
}



void setup() {
  Serial.begin(115200); 
  SerialBT.begin(device_name, true);
  delay(2000);

  esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

  LoadPreferences();

  esp_bt_controller_config_t bt_config = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  if (esp_bt_controller_init(&bt_config) == ESP_OK) {
    esp_bt_controller_enable(ESP_BT_MODE_BTDM);
  }

  esp_bt_gap_register_callback(scanCallback);
  esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void loop() {
  char *msg = SerialReadLineChar();
  if (msg != nullptr && strcasecmp(msg, "Init") == 0) {
    OUTPUTSTREAM.print("DefaultUser:");
    OUTPUTSTREAM.println(currentUser[0] == 'N' ? "1" : "0");
    OUTPUTSTREAM.print("USER:");
    OUTPUTSTREAM.println(currentUser);
  } 
  else if (msg != nullptr)
    ProcessMessage(msg);

  delay(1);
}








bool ProcessMessage(char *cmd) {
  char *header = strtok(cmd, ":");
  char *body = strtok(NULL, "");
  if (header == nullptr || body == nullptr) {
    Serial.println("Error: Command not recognized, or invalid value");
    return false;
  }

  //USER:name,maxBrightness,minBrightness,maxVolume,minVolume
  if (strcasecmp(header, "USER") == 0) {
    char *user = strtok(body, ",");
    char *maxB_str = strtok(nullptr, ",");
    char *minB_str = strtok(nullptr, ",");
    char *maxV_str = strtok(nullptr, ",");
    char *minV_str = strtok(nullptr, ",");
    if (user && maxB_str && minB_str && maxV_str && minV_str) {
      currentUser = String(user);
      maxBrightness = atof(maxB_str);
      minBrightness = atof(minB_str);
      maxVolume = atof(maxV_str);
      minVolume = atof(minV_str);
      SavePreferences();
      SendToAllPanels();
      return true;
    }
  }
  //SETV:type=val
  else if (strcasecmp(header, "SETV") == 0) {
    char *varType = strtok(body, "=");
    char *val = strtok(nullptr, "");
    if (varType && val) {
      if (strcasecmp(varType, "MaxBright") == 0)
        maxBrightness = atof(val);
      else if (strcasecmp(varType, "MinBright") == 0)
        minBrightness = atof(val);
      else if (strcasecmp(varType, "MaxVol") == 0)
        maxVolume = atof(val);
      else if (strcasecmp(varType, "MinVol") == 0)
        minVolume = atof(val);
    }
    SavePreferences();
    SendToAllPanels();
    return true;
  }

  Serial.println("Error: Command Not handled");
  return false;
}

void SendToAllPanels() {
  SerialBT.println("UPDATE:" + String(maxBrightness) + "," + String(minBrightness) + "," + String(maxVolume) + "," + String(minVolume)); 
}

char *SerialReadLineChar() {
  char c;
  bool msgRecieved = false;
  static uint8_t index = 0;
  static char charBuff[CHAR_BUFFER_SIZE] = {0};
  while (OUTPUTSTREAM.available() > 0) {
    c = (char)OUTPUTSTREAM.read();
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

void SavePreferences() {
  if (!prefs.begin(pref_namespace, false)) {
    Serial.println("Error: Failed to open preferences storage!");
    return;
  }
  prefs.putString("currentUser", currentUser);
  prefs.putFloat("maxBrightness", maxBrightness);
  prefs.putFloat("minBrightness", minBrightness);
  prefs.putFloat("maxVolume", maxVolume);
  prefs.putFloat("minVolume", minVolume);
  prefs.end();
}

void LoadPreferences() {
  if (!prefs.begin(pref_namespace, true)) {
    Serial.println("Error: Failed to open preferences storage!");
    return;
  }
  if (!prefs.isKey("currentUser")) {
    Serial.println("No user exists in storage. Using defaults.");
    prefs.end();
    return;
  }
  prefs.getString("currentUser", currentUser);
  maxBrightness = prefs.getFloat("maxBrightness", 100);
  minBrightness = prefs.getFloat("minBrightness", 0);
  maxVolume = prefs.getFloat("maxVolume", 100);
  minVolume = prefs.getFloat("minVolume", 0);
  prefs.end();
}
