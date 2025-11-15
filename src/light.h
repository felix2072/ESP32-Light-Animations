#ifndef LIGHT_H
#define LIGHT_H


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include <Timer.h>
// MIDI

class Light
{
private:
  // ADAFRUIT NEOPIXEL
  // Which pin on the ESP32 is connected to the NeoPixels?

  #define LEDPIN 23
  #define MAXLED 126
  #define SERIAL_DEBUG true

  // setting up the NeoPixel - pixels, pin, led type

  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(MAXLED, LEDPIN, NEO_GRB + NEO_KHZ800);


  // TIMER
  Timer neoPixelTimer;
  int neoPixelInterval = 100;

  uint16_t colorIndex = 0; 

public:
  Light();
  void initNeoPixel();
  void setLEDColor(int r, int g, int b);
  uint32_t colorWheel(byte pos);
  void updateRainbowColor();
  void updateNoiseColor();
  void serverAnimationUpdate();
};

#endif