#ifndef LIGHT_H
#define LIGHT_H


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
// MIDI

class Light
{
private:
  // ADAFRUIT NEOPIXEL
  // Which pin on the ESP32 is connected to the NeoPixels?


  #define SERIAL_DEBUG false

  //1D array for LED ids
  
  uint8_t LED_Rows;
  uint8_t LED_Cols;
  uint8_t* LED_1D;
  uint8_t** LED_Matrix;
  uint16_t time;

  
  uint16_t counter = 0;

public:
  Light(uint8_t rows, uint8_t cols);
  void initNeoPixel(Adafruit_NeoPixel& _pixels, boolean mapping);
  void setLEDColor(Adafruit_NeoPixel& _pixels, uint8_t r, uint8_t g, uint8_t b);
  void setLEDColor(Adafruit_NeoPixel& _pixels, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
  uint32_t colorWheel(Adafruit_NeoPixel& _pixels, byte pos);
  uint32_t ColorHSVWheel(Adafruit_NeoPixel& _pixels, byte pos);
  void updateRainbowColor(Adafruit_NeoPixel& _pixels, boolean increment, uint16_t& colorIndex);
  void paintMode(Adafruit_NeoPixel& _pixels, uint32_t color_1, uint32_t color_2, uint8_t* active_1);
  void LerpMode(Adafruit_NeoPixel& _pixels, uint32_t color_1, uint32_t color_2, uint8_t mix);
  void setBrightness(Adafruit_NeoPixel& _pixels, uint8_t brightness, uint8_t max);
  ~Light();
};

#endif