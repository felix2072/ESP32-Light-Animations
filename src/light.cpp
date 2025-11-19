#include <light.h>

Light::Light()
{
}

void Light::initNeoPixel()
{   
    /* for(int r = 0; r < LEDROWS; r++){
        if(r % 2 == 0){
            for(int c = 0; c < LEDCOLS; c++){
                int ledIndex = r + LEDROWS * c;
                int ledID = c + LEDCOLS * r;
                ledIDs[ledID] = ledIndex;
                Serial.println("LED ID[" + String(ledID) + "] = " + String(ledIndex));
            }
        }else{
            for(int c = LEDCOLS -1; c >=0; c--){
                int ledIndex = r + LEDROWS * c;
                int ledID = c + LEDCOLS * r;
                ledIDs[ledID] = ledIndex;
                Serial.println("LED ID[" + String(ledID) + "] = " + String(ledIndex));
            }

        }
    } */
   
    // Mapping berechnen
    int index_1d = 0;
    for (int r = 0; r < LEDROWS; r++) {
        for (int c = 0; c < LEDCOLS; c++) {

            int index;

            if (r % 2 == 0) {
                // Gerade Reihe → normal
                index = r * LEDCOLS + c;
            } else {
                // Ungerade Reihe → reverse
                index = r * LEDCOLS + (LEDCOLS - 1 - c);
            }

            LED_MATRIX[r][c] = index;
            LED_1D[index_1d] = index;
            Serial.println("LED ID[" + String(index_1d) + "] = " + String(index) + " (Row: " + String(r) + ", Col: " + String(c) + ")");
            index_1d++;
        }
    }

    pixels.begin();
    pixels.setBrightness(255);
}

void Light::setLEDColor(int red, int green, int blue){
    if(pixels.getPixelColor(0) != pixels.Color(red, green, blue)){
        for (int r = 0; r < LEDROWS; r++) {
            for (int c = 0; c < LEDCOLS; c++) {
                pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(red, green, blue));
            }
        }
        pixels.show();
        
        if(SERIAL_DEBUG){
            Serial.print("setLEDRingColor: " + String(red));
            Serial.print(", " + String(green));
            Serial.println(", " + String(blue));
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

uint32_t Light::ColorHSVWheel(byte pos) {
    if(pos < 85) {
        return pixels.ColorHSV(pos, 0, 0);
    }
    return pixels.ColorHSV(0, 0, 0);
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

void Light::updateNoiseColor(int mode) {
    
    float time = millis() *.1; // Zeit in Sekunden
    for (int r = 0; r < LEDROWS; r++) {
        for (int c = 0; c < LEDCOLS; c++) {
            // P<erlin-Rauschwert für die LED-Position und die Zeit abrufen
            float noiseValue = inoise8(r*80,c*40, time);
            

            uint32_t col = colorWheel(noiseValue);

            // Farbe extrahieren (da deine Funktion r,g,b erwartet)
            byte red = (col >> 16) & 0xFF;
            byte green = (col >>  8) & 0xFF;
            byte blue =  col        & 0xFF;


            //Serial.println("Noise Value [" + String(r) + "][" + String(c) + "] = col "+ String(col) + "/" + String(red) + "," + String(green) + "," + String(blue));
            if(mode == 0){
                
                if(pixels.gamma8(green)>10 && pixels.gamma8(green)<40){
                    pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(40, 0, 40));
                }else{
                    pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0,0,0));
                }
            }
            if(mode == 1){
                float noiseValue2 = inoise8(r*60,c*30, time+5000);
                uint32_t col2 = colorWheel(noiseValue2);
                byte red2 = (col2 >> 16) & 0xFF;
                byte green2 = (col2 >>  8) & 0xFF;
                byte blue2=  col2        & 0xFF;
                
                pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0,0,0));
                if(pixels.gamma8(green)>10 && pixels.gamma8(green)<40){
                    pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0, 150, 0));
                }
                if (pixels.gamma8(red2)>2 && pixels.gamma8(red2)<50)
                {
                    pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(150, 100, 0));
                }
            }
            if(mode == 2){
                pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(pixels.gamma8(red), pixels.gamma8(green*0.7), 0));
            }
        }
    }
    pixels.show();
}

void Light::serverAnimationUpdate(int mode) {
    float time = millis() *.1;
    if (mode == 0)
    {
        for (int r = 0; r < LEDROWS; r++) {
            for (int c = 0; c < LEDCOLS; c++) {
                float noiseValue = inoise8(r*80,c*40, time);

                if(counter % 7 < 3 && noiseValue < 100 + (counter*0.5)){
                    if(r % 2 == counter % 2 == 0){
                        if(c % 4 < 1){
                            pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(255, 255, 0));
                        }else{
                            pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0,0,0));
                        }
                    }else{
                        if(c % 5 < counter % 5){
                            pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0, 255, 0));
                        }else{
                            pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0,0,0));
                        }
                    }
                }
            }
        }
    }
    
    if(mode == 1){
        for(float i = 0; i < 3; i++){
            float noiseValue = inoise8(i*400, time*10);
            if(counter + i * 14 < MAXLED){
                pixels.setPixelColor(LED_1D[counter + i * 18], pixels.Color(noiseValue, noiseValue, noiseValue));
                pixels.setPixelColor(LED_1D[counter + i * 16], pixels.Color(noiseValue, noiseValue, 0));
                pixels.setPixelColor(LED_1D[counter + i * 14], pixels.Color(noiseValue, 0, 0));
            }

        }
    }

    pixels.show();
    counter++;
    if(counter >= MAXLED) {
        counter = 0;
    }
    delay(10); // Kurze Pause
}

void Light::onePixelTest() {
    for (int r = 0; r < LEDROWS; r++) {
        for (int c = 0; c < LEDCOLS; c++) {
            pixels.setPixelColor(LED_MATRIX[r][c], pixels.Color(0, 0, 0));
        }
    }
    pixels.setPixelColor(LED_1D[counter], pixels.Color(255, 0, 0)); // Eine LED rot einschalten
    pixels.show();

    counter++;
    if(counter >= MAXLED) {
        counter = 0;
    }
    delay(100); // Kurze Pause
}
