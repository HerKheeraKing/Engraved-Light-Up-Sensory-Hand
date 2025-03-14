#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <Bounce2.h>

#define VS1053_RESET -1  // VS1053 reset pin (not used!)

#define VS1053_CS 32    // VS1053 chip select pin (output)
#define VS1053_DCS 33   // VS1053 Data/command select pin (output)
#define CARDCS 14       // Card chip select pin
#define VS1053_DREQ 15  // VS1053 Data request, ideally an Interrupt pin

#define BOUNCE_PIN1 12

// INSTANTIATE A Bounce and FilePlayer OBJECT
Bounce bounce = Bounce();
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
bool isDone;

void setup() {
  Serial.begin(115200);
  // if you're using Bluefruit or LoRa/RFM Feather, disable the radio module
  pinMode(BOUNCE_PIN1, INPUT_PULLUP);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

  bounce.attach(BOUNCE_PIN1, INPUT_PULLUP);  // USE INTERNAL PULL-UP
  bounce.interval(5);                        // interval in ms

  if (!musicPlayer.begin()) {  // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1)
      ;
  }

  Serial.println(F("VS1053 found"));

  //musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1)
      ;  // don't do anything more
  }
  Serial.println("SD OK!");

  // list files
  printDirectory(SD.open("/"), 0);

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10, 10);

  // If DREQ is on an interrupt pin we can do background
  // audio playing
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Play a file in the background, REQUIRES interrupts!
  // musicPlayer.playFullFile("/Pig_01.wav");

  //Serial.println(F("Playing track"));
  musicPlayer.playFullFile("/Yippee.ogg");
  //musicPlayer.startPlayingFile("/Yippee.ogg");
}

void loop() {
  bounce.update();
  if (bounce.fell()) {
    Serial.println("Button press");
    if (musicPlayer.stopped()) {
      Serial.println("play");
      musicPlayer.playFullFile("/Pig_01.wav");
    } else {
      Serial.println("Wait for music to stop.");
    }
  }
  //if(bounce.fell()){
  //   int btn = digitalRead(BOUNCE_PIN1);
  //   Serial.println(btn);
  //   if(btn == LOW){
  //   Serial.println("play");
  // musicPlayer.playFullFile("/Pig_01.wav");
  // //musicPlayer.startPlayingFile("/Pig_01.wav");
  // delay(500);
  //   }
    // musicPlayer.playFullFile("/Pig_01.wav");
    // delay(500);
  //musicPlayer.startPlayingFile("/Yippee.ogg");
  //}
  //Serial.print(".");
  // File is playing in the background
  // if (musicPlayer.stopped()) {
  //   Serial.println("Done playing music");
  //   while (1) {
  //     delay(10);  // we're done! do nothing...
  //   }
  //}
  // if (Serial.available()) {
  //   char c = Serial.read();

  //   // if we get an 's' on the serial console, stop!
  //   if (c == 's') {
  //     musicPlayer.stopPlaying();
  //   }
  //   if (c == 'm') {
  //     musicPlayer.startPlayingFile("/Pig_01.wav");
  //   }
  //   // if we get an 'p' on the serial console, pause/unpause!
  //   if (c == 'p') {
  //     if (! musicPlayer.paused()) {
  //       Serial.println("Paused");
  //       musicPlayer.pausePlaying(true);
  //     } else {
  //       Serial.println("Resumed");
  //       musicPlayer.pausePlaying(false);
  //     }
  //   }
  // }
  // delay(100);
}



/// File listing helper
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
