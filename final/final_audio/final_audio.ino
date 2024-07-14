
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>
#include <stdint.h>

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

#define LED_PIN    6

// If its too bright, lower this value. Do not go above 100
#define BRIGHTNESS         100

TMRpcm audio;
File myFile;

unsigned int sampling_period_us;
unsigned long microseconds;

uint8_t row[40];
long start_time;
long target_time;
#define LED_DELAY 125

#define LED_COUNT 40


void setup() {
  audio.speakerPin = 9;
  // Open serial communications and wait for port to open:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  

 Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open("ui.wav");

  audio.setVolume(6);    //   0 to 7. Set volume level
  audio.quality(0);      //  Set 1 for 2x oversampling Set 0

  delay(500);
  digitalWrite(2, HIGH);
  audio.play("ui.wav");
}

void loop() {
}
