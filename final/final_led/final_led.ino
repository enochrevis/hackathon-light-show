
#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

#define LED_PIN    6

// If its too bright, lower this value. Do not go above 100
#define BRIGHTNESS         100

File myLedFile;

unsigned int sampling_period_us;
unsigned long microseconds;

uint8_t row[41];
long start_time;
long target_time;
#define LED_DELAY 41667

#define LED_COUNT 60

#define ROW_SIZE 61

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Open serial communications and wait for port to open:
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  

 Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myLedFile = SD.open("ui.bin");

  pinMode(2, INPUT);
  delay(50);
  while(! digitalRead(2));

//  while (myLedFile.read(row, 40) == 40) {
//    for (int i = 0; i < 40; i++){
//      Serial.print(row[i]);
//      Serial.print(",");
//    }
//    Serial.println();
//  }
//  myLedFile.seek(0);
  
  target_time = micros() + LED_DELAY;
}

void loop() {
  if (!digitalRead(2)) {
    myLedFile.seek(0);
    while (!digitalRead(2));
    target_time = micros() + LED_DELAY;
  }
  //Serial.println(1);
  Serial.println(target_time - micros());
  while (micros() < target_time);
  //Serial.println(2);

  int bytes_read = myLedFile.read(row, ROW_SIZE);
  //Serial.println(3);

//  if (bytes_read != ROW_SIZE) {
//    while (true);
//  }
  if (bytes_read != ROW_SIZE) return;

  //Serial.println(4);
  for (int i = 0; i < ROW_SIZE; i++) {
    if (row[0]) {
      strip.setPixelColor(i, strip.Color(200, 0, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 200, 0));
    }
//    if (row[0]) {
//      strip.setPixelColor(i, strip.Color(row[i+1], 0, 0));
//    } else {
//      strip.setPixelColor(i, strip.Color(0, row[i+1], 0));
//    }
  }
  strip.show();
//  Serial.print(".");
  //Serial.println(5);

//  Serial.print(millis());
//  Serial.print(", ");
//  Serial.println(target_time);
  
  target_time += LED_DELAY;
}
