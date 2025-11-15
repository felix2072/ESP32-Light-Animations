#include <light.h>

Light::Light()
{
}

void Light::initNeoPixel()
{
    pixels.begin();
    pixels.setBrightness(255);
}

void Light::setLEDColor(int r, int g, int b){
    if(pixels.getPixelColor(0) != pixels.Color(r, g, b)){
        for (int i = 0; i < MAXLED; i++)
        {
            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }
        pixels.show();
        
        if(SERIAL_DEBUG){
            Serial.print("setLEDRingColor: " + String(r));
            Serial.print(", " + String(g));
            Serial.println(", " + String(b));
        }
    }
}

uint32_t Light::colorWheel(byte pos) {
    pos = 255 - pos;
    if(pos < 85) {
        return pixels.Color(255 - pos * 3, 0, pos * 3);
    }
    if(pos < 170) {
        pos -= 85;
        return pixels.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return pixels.Color(pos * 3, 255 - pos * 3, 0);
}

void Light::updateRainbowColor() {

    uint32_t col = colorWheel(colorIndex);

    // Farbe extrahieren (da deine Funktion r,g,b erwartet)
    byte r = (col >> 16) & 0xFF;
    byte g = (col >>  8) & 0xFF;
    byte b =  col        & 0xFF;

    setLEDColor(r, g, b);

    // Farbrad weiterdrehen
    colorIndex++;
    if(colorIndex >= 255) colorIndex = 0;
}

void Light::updateNoiseColor() {
    
    float time = millis(); // Zeit in Sekunden
    for (int i = 0; i < MAXLED; i++) {
        // Perlin-Rauschwert für die LED-Position und die Zeit abrufen
        float noiseValue = inoise8(i * 60, time);

        // Rauschwert in eine Farbe umwandeln (z.B. Blau bis Rot)
        byte r = 0;
        byte g = noiseValue;
        byte b = 255 - noiseValue;
        if(noiseValue > 128) {
            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }else{
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
    }
    pixels.show();
}

void Light::serverAnimationUpdate() {
    // Hier kannst du den Code für die Server-Animation einfügen
}
