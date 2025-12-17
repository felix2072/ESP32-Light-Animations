#ifndef LIGHT_H
#define LIGHT_H


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include <array>
// MIDI

class Light
{
private:
  // ADAFRUIT NEOPIXEL
  // Which pin on the ESP32 is connected to the NeoPixels?

  #define MAXLED_1 208
  #define LEDROWS_1 8
  #define LEDCOLS_1 26

  #define SERIAL_DEBUG true

  // setting up the NeoPixel - pixels, pin, led type

  Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(MAXLED_1, 15, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(MAXLED_1, 16, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(MAXLED_1, 17, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_4 = Adafruit_NeoPixel(MAXLED_1, 18, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_5 = Adafruit_NeoPixel(MAXLED_1, 8, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_6 = Adafruit_NeoPixel(MAXLED_1, 3, NEO_GRB + NEO_KHZ800);
  
  Adafruit_NeoPixel pixels_7 = Adafruit_NeoPixel(MAXLED_1, 36, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_8 = Adafruit_NeoPixel(MAXLED_1, 37, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_9 = Adafruit_NeoPixel(MAXLED_1, 38, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_10 = Adafruit_NeoPixel(MAXLED_1, 39, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_11 = Adafruit_NeoPixel(MAXLED_1, 40, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel pixels_12 = Adafruit_NeoPixel(MAXLED_1, 41, NEO_GRB + NEO_KHZ800);


  //1D array for LED ids
  std::array<int, MAXLED_1> LED_1D_1;
  std::array<std::array<int, 26>, 8> LED_MATRIX_1{};

  uint16_t colorIndex = 0;
  
  uint16_t counter = 0;

public:
  Light();
  void initNeoPixel();
  void setLEDColor(int r, int g, int b);
  uint32_t colorWheel(byte pos);
  uint32_t ColorHSVWheel(byte pos);
  void updateRainbowColor(int brightness);
  void updateNoiseColor(int mode, int brightness);
  void serverAnimationUpdate(int mode);
  void onePixelTest();
};

#endif