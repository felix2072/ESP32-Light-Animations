#include <Arduino.h>
#include <light.h>

Light::Light(uint8_t rows, uint8_t cols)
{
    LED_Rows = rows;
    LED_Cols = cols;
    LED_1D = new uint8_t[rows * cols];
    LED_Matrix = new uint8_t*[rows];
    for (uint8_t r = 0; r < rows; r++) {
        LED_Matrix[r] = new uint8_t[cols];
    }
}


void Light::initNeoPixel(Adafruit_NeoPixel& _pixels, boolean mapping)
{   
    if (mapping)
    {
        // Mapping berechnen
        uint8_t index_1d = 0;
        for (int r = 0; r < LED_Rows; r++) {
            for (int c = 0; c < LED_Cols; c++) {

                uint8_t index;

                if (r % 2 == 0) {
                    // Gerade Reihe → normal
                    index = r * LED_Cols + c;
                } else {
                    // Ungerade Reihe → reverse
                    index = r * LED_Cols + (LED_Cols - 1 - c);
                }

                LED_Matrix[r][c] = index;
                LED_1D[index_1d] = index;
                Serial.println("LED ID[" + String(index_1d) + "] = " + String(index) + " (Row: " + String(r) + ", Col: " + String(c) + ")");
                index_1d++;
            }
        }
    }

    _pixels.begin();
    _pixels.setBrightness(255);
    
    setLEDColor(_pixels,10, 0, 0);
    delay(200);

    setLEDColor(_pixels,0, 10, 0);
    delay(200);

    setLEDColor(_pixels,0, 0, 10);
    delay(200);

    setLEDColor(_pixels,0, 0, 0);
}

void Light::setLEDColor(Adafruit_NeoPixel& _pixels, uint8_t red, uint8_t green, uint8_t blue){
    if(_pixels.getPixelColor(0) != _pixels.Color(red, green, blue)){
        for (int r = 0; r < LED_Rows; r++) {
            for (int c = 0; c < LED_Cols; c++) {
                _pixels.setPixelColor(LED_Matrix[r][c], _pixels.Color(red, green, blue));
            }
        }
        _pixels.show();
    }
}

void Light::setLEDColor(Adafruit_NeoPixel& _pixels, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
    if(_pixels.getPixelColor(0) != _pixels.Color(red, green, blue)){
        for (int r = 0; r < LED_Rows; r++) {
            for (int c = 0; c < LED_Cols; c++) {
                _pixels.setPixelColor(LED_Matrix[r][c], _pixels.Color(red, green, blue));
            }
        }
        _pixels.setBrightness(brightness);
        _pixels.show();
    }
}

uint32_t Light::colorWheel(Adafruit_NeoPixel& _pixels, byte pos) {
    pos = 255 - pos;
    if(pos < 85) {
        return _pixels.Color(255 - pos * 3, 0, pos * 3);
    }
    if(pos < 170) {
        pos -= 85;
        return _pixels.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return _pixels.Color(pos * 3, 255 - pos * 3, 0);
}

uint32_t Light::ColorHSVWheel(Adafruit_NeoPixel& _pixels, byte pos) {
    if(pos < 255) {
        return _pixels.ColorHSV(pos, 0, 0);
    }
    return _pixels.ColorHSV(0, 0, 0);
}

void Light::updateRainbowColor(Adafruit_NeoPixel& _pixels,boolean increment, uint16_t& colorIndex) {

    uint32_t col = colorWheel(_pixels, colorIndex);

    // Farbe extrahieren (da deine Funktion r,g,b erwartet)
    byte r = (col >> 16) & 0xFF;
    byte g = (col >>  8) & 0xFF;
    byte b =  col        & 0xFF;
    setLEDColor(_pixels,r, g, b);

    // Farbrad weiterdrehen
    increment ? colorIndex++ : colorIndex--;
    if(colorIndex >= 255) colorIndex = 0;
    if(colorIndex < 0) colorIndex = 255;
}


void Light::paintMode(Adafruit_NeoPixel& _pixels, uint32_t color_1, uint32_t color_2, uint8_t* active_1){
    byte red1 = (color_1 >> 16) & 0xFF;
    byte green1 = (color_1 >>  8) & 0xFF;
    byte blue1 =  color_1        & 0xFF;

    byte red2 = (color_2 >> 16) & 0xFF;
    byte green2 = (color_2 >>  8) & 0xFF;
    byte blue2 =  color_2        & 0xFF;

    for(int i = 0; i < LED_Rows*LED_Cols; i++){
        if(active_1[i] == 1){
        _pixels.setPixelColor(i, _pixels.Color(red1, green1, blue1));
        }else{
        _pixels.setPixelColor(i, _pixels.Color(red2, green2, blue2));
        }
    }

  _pixels.show();
}

void Light::LerpMode(Adafruit_NeoPixel& _pixels, uint32_t color_1, uint32_t color_2, uint8_t mix){
    byte r1 = (color_1 >> 16) & 0xFF;
    byte g1 = (color_1 >>  8) & 0xFF;
    byte b1 =  color_1        & 0xFF;

    byte r2 = (color_2 >> 16) & 0xFF;
    byte g2 = (color_2 >>  8) & 0xFF;
    byte b2 =  color_2        & 0xFF;

    for(int i = 0; i < LED_Rows * LED_Cols; i++){
        uint8_t r = r1 + ((int16_t)(r2 - r1) * mix) / 255;
        uint8_t g = g1 + ((int16_t)(g2 - g1) * mix) / 255;
        uint8_t b = b1 + ((int16_t)(b2 - b1) * mix) / 255;

        _pixels.setPixelColor(i, _pixels.Color(r, g, b));
    }

    _pixels.show();
}

void Light::setBrightness(Adafruit_NeoPixel& _pixels, uint8_t brightness, uint8_t max){
    if(brightness < max){
        _pixels.setBrightness(brightness);
    }
}


Light::~Light() {
    delete[] LED_1D;
    for (uint8_t r = 0; r < LED_Rows; r++) {
        delete[] LED_Matrix[r];
    }
    delete[] LED_Matrix;
}
