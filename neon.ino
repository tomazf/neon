// Arduino neon sign simulation
//
// Ferbi

#include "src/RGBLed.h"           // https://github.com/wilmouths/RGBLed

// onboard activity
#define DATA_PIN 13

// RGB - PWM pins
#define R_PIN 9
#define G_PIN 11
#define B_PIN 10

// disable neon pin
#define BUTTON_PIN 2

// debug?
#define DEBUG 0

// use flickerTable
#define FLICKER 1

// some other variables
uint8_t count = 0;                // counter for the flicker table
const uint8_t velikost = 100;     // size of flickerTable
byte flickerTable[velikost];

// set RGB color
byte RGB[] = {230, 255 , 10};

// create object
RGBLed led(R_PIN, G_PIN, B_PIN, COMMON_CATHODE);

/*  // for fixed dimmer
  byte flickerTable[] = { 10, 10, 20, 30, 250, 250, 140, 10, 10, 255 };
*/

void setup() {

  // set pin state
  pinMode(DATA_PIN, OUTPUT);        // declare out
  pinMode(BUTTON_PIN, INPUT);       // declare in

  // start serial for debugging
  if (DEBUG) {
    Serial.begin(115200);
    Serial.print("Setup done!");
  }

  // set strip color
  led.setColor( RGB[0] , RGB[1] , RGB[2]);
}

void loop() {

  // if pin GND-ed
  if (!digitalRead(BUTTON_PIN)) {                       // check if the input is HIGH
    digitalWrite(DATA_PIN, LOW);
    led.off();
    if (DEBUG) {
      Serial.println("OFF by HW switch!");
    }
    delay(1000);
  }
  else {
    mode_1();     // random or flicker
    mode_2();     // just big random
  }
}

void mode_1() {
  delay(30);
  if (FLICKER) {
    setFlicker();

    for ( int i = 0; i <= velikost; i++) {                // This for loop runs untill the flicker table finishes
      analogWrite(DATA_PIN, flickerTable[i]);
      led.brightness(RGB[0] , RGB[1] , RGB[2], flickerTable[i] * 100 / 255);

      if (count == 3 && flickerTable[i] > 200) {
        digitalWrite(DATA_PIN, HIGH);
        led.brightness(RGB[0] , RGB[1] , RGB[2], 100);
        delay(random(3000, 6000));
        count = 0;
      }
      if (flickerTable[i] > 200) {
        delay(random(200, 400));
        count++;
      }
      if (flickerTable[i] < 20) {
        delay(random(50, 80));
      }
      else {
        delay(random(10, 20));
        count = 0;
      }
    }
  }
  else {
    for ( int i = 0; i <= velikost / 2 ; i++) {
      analogWrite(DATA_PIN, 255);
      led.brightness(RGB[0] , RGB[1] , RGB[2], 255);
      delay(random(200, 800));

      analogWrite(DATA_PIN, 0);
      led.off();
      delay(random(10, 80 + count));

      analogWrite(DATA_PIN, 255);
      led.brightness(RGB[0] , RGB[1] , RGB[2], 255);
      delay(random(10, 50));

      analogWrite(DATA_PIN, 0);
      led.off();
      delay(random(10, 20 + count));

      analogWrite(DATA_PIN, 255);
      led.brightness(RGB[0] , RGB[1] , RGB[2], 255);
      count++;
    }
  }

  digitalWrite(DATA_PIN, LOW);          // turn off strip
  led.off();
}

void mode_2() {
  if (DEBUG) {
    Serial.print("mode2...");
  }

  digitalWrite(DATA_PIN, HIGH);
  led.brightness(RGB[0] , RGB[1] , RGB[2], 100);
  delay(random(5000, 10000));
  digitalWrite(DATA_PIN, LOW);
  led.off();

  if (DEBUG) {
    Serial.println("off");
  }
}

void setFlicker() {
  for (int i = 0; i < velikost; i++) {
    flickerTable[i] = random(0, 255);
  }
}
