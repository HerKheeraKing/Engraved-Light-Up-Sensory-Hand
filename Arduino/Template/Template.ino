//Base Include
#include <PanelManager.h>

/*
All Constructor args: panelName, panelPrefix, channel, debug, baud

Mandatory args:
  panelName -> String, The name of the actual panel, name it something meaningful, it will be shown in WiFi/Bluetooth connections

Optional args:
  panelPrefix -> String, The prefix the esps will scan for, can be set per classroom, currently scans for 'Panel-'
  channel -> uint8_t, The channel the esps are running on, make sure the esps you wish to find are on the same channel
  debug -> bool, If you want debug logs to be shown
  baud -> unsigned long, Assuming debug is true, set the baud rate you wish to have the debug messages go to
*/ 


/*
NOTES: 
  * To include the library easily, go to Sketch->Include Library->Add .zip Library

  * If you have compiler issues, make sure you are using Huge App Partion Scheme
    -Tools->Partion Scheme->Huge APP(3MB No OTA/1MB SPIFFS)
  
  * When using analogReads, make sure to call `analogSetPinAttenuation(pin, attenuation)`
  
  * If you are using volume/brightness, please make sure to use the correct settings
      -panelManager.maxBrightness
      -panelManager.minBrightness
      -panelManager.maxVolume
      -panelManager.minVolume
*/


//Very Basic Setup, ideally this is all you need for this class, unless specified otherwise
//Make sure you include your prefix to look for in your panelName
PanelManager panelManager("Panel-Something");

void setup() {
  Serial.begin(115200);
  //Make sure to call this init, everything else is handled internally
  panelManager.InitManager();

  //Whatever else
}

void loop() {
  //Whatever else
  delay(100);
}
