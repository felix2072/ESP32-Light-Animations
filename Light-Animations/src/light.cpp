#include <light.h>

Light::Light()
{
}

void Light::initNeoPixel()
{   
    /* for(int r = 0; r < LEDROWS_1; r++){
        if(r % 2 == 0){
            for(int c = 0; c < LEDCOLS_1; c++){
                int ledIndex = r + LEDROWS_1 * c;
                int ledID = c + LEDCOLS_1 * r;
                ledIDs[ledID] = ledIndex;
                Serial.println("LED ID[" + String(ledID) + "] = " + String(ledIndex));
            }
        }else{
            for(int c = LEDCOLS_1 -1; c >=0; c--){
                int ledIndex = r + LEDROWS_1 * c;
                int ledID = c + LEDCOLS_1 * r;
                ledIDs[ledID] = ledIndex;
                Serial.println("LED ID[" + String(ledID) + "] = " + String(ledIndex));
            }

        }
    } */
   
    // Mapping berechnen
    int index_1d = 0;
    for (int r = 0; r < LEDROWS_1; r++) {
        for (int c = 0; c < LEDCOLS_1; c++) {

            int index;

            if (r % 2 == 0) {
                // Gerade Reihe → normal
                index = r * LEDCOLS_1 + c;
            } else {
                // Ungerade Reihe → reverse
                index = r * LEDCOLS_1 + (LEDCOLS_1 - 1 - c);
            }

            LED_MATRIX_1[r][c] = index;
            LED_1D_1[index_1d] = index;
            Serial.println("LED ID[" + String(index_1d) + "] = " + String(index) + " (Row: " + String(r) + ", Col: " + String(c) + ")");
            index_1d++;
        }
    }

    pixels_1.begin();
    pixels_1.setBrightness(255);
}

void Light::setLEDColor(int red, int green, int blue){
    if(pixels_1.getPixelColor(0) != pixels_1.Color(red, green, blue)){
        for (int r = 0; r < LEDROWS_1; r++) {
            for (int c = 0; c < LEDCOLS_1; c++) {
                pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(red, green, blue));
                pixels_2.setPixelColor(LED_MATRIX_1[r][c], pixels_2.Color(red, green, blue));
                pixels_3.setPixelColor(LED_MATRIX_1[r][c], pixels_3.Color(red, green, blue));
                pixels_4.setPixelColor(LED_MATRIX_1[r][c], pixels_4.Color(red, green, blue));
                pixels_5.setPixelColor(LED_MATRIX_1[r][c], pixels_5.Color(red, green, blue));
                pixels_6.setPixelColor(LED_MATRIX_1[r][c], pixels_6.Color(red, green, blue));
                pixels_7.setPixelColor(LED_MATRIX_1[r][c], pixels_7.Color(red, green, blue));
                pixels_8.setPixelColor(LED_MATRIX_1[r][c], pixels_8.Color(red, green, blue));
                pixels_9.setPixelColor(LED_MATRIX_1[r][c], pixels_9.Color(red, green, blue));
                pixels_10.setPixelColor(LED_MATRIX_1[r][c], pixels_10.Color(red, green, blue));
                pixels_11.setPixelColor(LED_MATRIX_1[r][c], pixels_11.Color(red, green, blue));
                pixels_12.setPixelColor(LED_MATRIX_1[r][c], pixels_12.Color(red, green, blue));
            }
        }
        pixels_1.show();
        pixels_2.show();
        pixels_3.show();
        pixels_4.show();
        pixels_5.show();
        pixels_6.show();
        pixels_7.show();
        pixels_8.show();
        pixels_9.show();
        pixels_10.show();
        pixels_11.show();
        pixels_12.show();
        
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
        return pixels_1.Color(255 - pos * 3, 0, pos * 3);
    }
    if(pos < 170) {
        pos -= 85;
        return pixels_1.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return pixels_1.Color(pos * 3, 255 - pos * 3, 0);
}

uint32_t Light::ColorHSVWheel(byte pos) {
    if(pos < 85) {
        return pixels_1.ColorHSV(pos, 0, 0);
    }
    return pixels_1.ColorHSV(0, 0, 0);
}

void Light::updateRainbowColor(int brightness) {

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

void Light::updateNoiseColor(int mode, int brightness) {

    /* float temp_speed = speed * 0002;
    temp_speed = temp_speed + 0.001; */
    float time = millis() * 0.1; // Zeit in Sekunden
    
    for (int r = 0; r < LEDROWS_1; r++) {
        for (int c = 0; c < LEDCOLS_1; c++) {
            // P<erlin-Rauschwert für die LED-Position und die Zeit abrufen
            float noiseValue1 = inoise8(r*80,c*40, time);
            float noiseValue2 = inoise8(r*100,c*70, time*0.5);

            uint32_t col1 = colorWheel(noiseValue1);
            uint32_t col2 = colorWheel(noiseValue2);

            // Farbe extrahieren (da deine Funktion r,g,b erwartet)
            byte red1 = (col1 >> 16) & 0xFF;
            byte green1 = (col1 >>  8) & 0xFF;
            byte blue1 =  col1        & 0xFF;

            // Farbe extrahieren (da deine Funktion r,g,b erwartet)
            byte red2 = (col2 >> 16) & 0xFF;
            byte green2 = (col2 >>  8) & 0xFF;
            byte blue2 =  col2        & 0xFF;


            //Serial.println("Noise Value [" + String(r) + "][" + String(c) + "] = col "+ String(col) + "/" + String(red) + "," + String(green) + "," + String(blue));
            if(mode == 0){
                
                if(pixels_1.gamma8(green1)>10 && pixels_1.gamma8(green1)<40){
                    pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(brightness, 0, brightness));
                    pixels_2.setPixelColor(LED_MATRIX_1[r][c], pixels_2.Color(brightness, 0, brightness));
                    pixels_3.setPixelColor(LED_MATRIX_1[r][c], pixels_3.Color(brightness, 0, brightness));
                    pixels_4.setPixelColor(LED_MATRIX_1[r][c], pixels_4.Color(brightness, 0, brightness));
                    pixels_5.setPixelColor(LED_MATRIX_1[r][c], pixels_5.Color(brightness, 0, brightness));
                    pixels_6.setPixelColor(LED_MATRIX_1[r][c], pixels_6.Color(brightness, 0, brightness));
                    pixels_7.setPixelColor(LED_MATRIX_1[r][c], pixels_7.Color(brightness, 0, brightness));
                    pixels_8.setPixelColor(LED_MATRIX_1[r][c], pixels_8.Color(brightness, 0, brightness));
                    pixels_9.setPixelColor(LED_MATRIX_1[r][c], pixels_9.Color(brightness, 0, brightness));
                    pixels_10.setPixelColor(LED_MATRIX_1[r][c], pixels_10.Color(brightness, 0, brightness));
                    pixels_11.setPixelColor(LED_MATRIX_1[r][c], pixels_11.Color(brightness, 0, brightness));
                    pixels_12.setPixelColor(LED_MATRIX_1[r][c], pixels_12.Color(brightness, 0, brightness));
                }
                else if(pixels_1.gamma8(blue2)>20 && pixels_1.gamma8(blue2)<30){
                    pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0, 0, brightness));
                    pixels_2.setPixelColor(LED_MATRIX_1[r][c], pixels_2.Color(0, 0, brightness));
                    pixels_3.setPixelColor(LED_MATRIX_1[r][c], pixels_3.Color(0, 0, brightness));
                    pixels_4.setPixelColor(LED_MATRIX_1[r][c], pixels_4.Color(0, 0, brightness));
                    pixels_5.setPixelColor(LED_MATRIX_1[r][c], pixels_5.Color(0, 0, brightness));
                    pixels_6.setPixelColor(LED_MATRIX_1[r][c], pixels_6.Color(0, 0, brightness));
                    pixels_7.setPixelColor(LED_MATRIX_1[r][c], pixels_7.Color(0, 0, brightness));
                    pixels_8.setPixelColor(LED_MATRIX_1[r][c], pixels_8.Color(0, 0, brightness));
                    pixels_9.setPixelColor(LED_MATRIX_1[r][c], pixels_9.Color(0, 0, brightness));
                    pixels_10.setPixelColor(LED_MATRIX_1[r][c], pixels_10.Color(0, 0, brightness));
                    pixels_11.setPixelColor(LED_MATRIX_1[r][c], pixels_11.Color(0, 0, brightness));
                    pixels_12.setPixelColor(LED_MATRIX_1[r][c], pixels_12.Color(0, 0, brightness));
                }else{
                    pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0,0,0));
                    pixels_2.setPixelColor(LED_MATRIX_1[r][c], pixels_2.Color(0,0,0));
                    pixels_3.setPixelColor(LED_MATRIX_1[r][c], pixels_3.Color(0,0,0));
                    pixels_4.setPixelColor(LED_MATRIX_1[r][c], pixels_4.Color(0,0,0));
                    pixels_5.setPixelColor(LED_MATRIX_1[r][c], pixels_5.Color(0,0,0));
                    pixels_6.setPixelColor(LED_MATRIX_1[r][c], pixels_6.Color(0,0,0));
                    pixels_7.setPixelColor(LED_MATRIX_1[r][c], pixels_7.Color(0,0,0));
                    pixels_8.setPixelColor(LED_MATRIX_1[r][c], pixels_8.Color(0,0,0));
                    pixels_9.setPixelColor(LED_MATRIX_1[r][c], pixels_9.Color(0,0,0));
                    pixels_10.setPixelColor(LED_MATRIX_1[r][c], pixels_10.Color(0,0,0));
                    pixels_11.setPixelColor(LED_MATRIX_1[r][c], pixels_11.Color(0,0,0));
                    pixels_12.setPixelColor(LED_MATRIX_1[r][c], pixels_12.Color(0,0,0));
                }
            }
            if(mode == 1){
                float noiseValue2 = inoise8(r*60,c*30, time+5000);
                uint32_t col2 = colorWheel(noiseValue2);
                byte red2 = (col2 >> 16) & 0xFF;
                byte green2 = (col2 >>  8) & 0xFF;
                byte blue2=  col2        & 0xFF;
                
                pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0,0,0));
                if(pixels_1.gamma8(green1)>10 && pixels_1.gamma8(green1)<40){
                    pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0, 150, 0));
                }
                if (pixels_1.gamma8(red2)>2 && pixels_1.gamma8(red2)<50)
                {
                    pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(150, 100, 0));
                }
            }
            if(mode == 2){
                pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(pixels_1.gamma8(red1), pixels_1.gamma8(green1*0.7), 0));
            }
        }
    }
    pixels_1.show();
    pixels_2.show();
    pixels_3.show();
    pixels_4.show();
    pixels_5.show();
    pixels_6.show();
    pixels_7.show();
    pixels_8.show();
    pixels_9.show();
    pixels_10.show();
    pixels_11.show();
    pixels_12.show();
}

void Light::serverAnimationUpdate(int mode) {
    float time = millis() *.1;
    if (mode == 0)
    {
        for (int r = 0; r < LEDROWS_1; r++) {
            for (int c = 0; c < LEDCOLS_1; c++) {
                float noiseValue = inoise8(r*80,c*40, time);

                if(counter % 7 < 3 && noiseValue < 100 + (counter*0.5)){
                    if(r % 2 == counter % 2 == 0){
                        if(c % 4 < 1){
                            pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(255, 255, 0));
                        }else{
                            pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0,0,0));
                        }
                    }else{
                        if(c % 5 < counter % 5){
                            pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0, 255, 0));
                        }else{
                            pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0,0,0));
                        }
                    }
                }
            }
        }
    }
    
    if(mode == 1){
        for(float i = 0; i < 3; i++){
            float noiseValue = inoise8(i*400, time*10);
            if(counter + i * 14 < MAXLED_1){
                pixels_1.setPixelColor(LED_1D_1[counter + i * 18], pixels_1.Color(noiseValue, noiseValue, noiseValue));
                pixels_1.setPixelColor(LED_1D_1[counter + i * 16], pixels_1.Color(noiseValue, noiseValue, 0));
                pixels_1.setPixelColor(LED_1D_1[counter + i * 14], pixels_1.Color(noiseValue, 0, 0));
            }

        }
    }

    pixels_1.show();
    counter++;
    if(counter >= MAXLED_1) {
        counter = 0;
    }
    delay(10); // Kurze Pause
}

void Light::onePixelTest() {
    for (int r = 0; r < LEDROWS_1; r++) {
        for (int c = 0; c < LEDCOLS_1; c++) {
            pixels_1.setPixelColor(LED_MATRIX_1[r][c], pixels_1.Color(0, 0, 0));
        }
    }
    pixels_1.setPixelColor(LED_1D_1[counter], pixels_1.Color(255, 0, 0)); // Eine LED rot einschalten
    pixels_1.show();

    counter++;
    if(counter >= MAXLED_1) {
        counter = 0;
    }
    delay(100); // Kurze Pause
}
