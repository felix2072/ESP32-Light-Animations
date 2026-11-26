#include <Arduino.h>
#include <FastLED.h>

const byte trigger_pin = 32;
#if defined(ARDUINO_ESP32S3_DEV)
const uint8_t dataPin_A = 48;
#else
const uint8_t dataPin_A = 25;
#endif
const int numLeds = 7;

CRGB leds[numLeds];

const unsigned long triggerIntervalMs = 2000;
const unsigned long triggerLowDurationMs = 500;
const unsigned long animationStepMs = 20;

unsigned long lastTriggerStartMs = 0;
unsigned long lastAnimationStepMs = 0;
uint8_t hue = 0;

void setup()
{
    pinMode(trigger_pin, OUTPUT);
    digitalWrite(trigger_pin, HIGH);

    FastLED.addLeds<NEOPIXEL, dataPin_A>(leds, numLeds);
    FastLED.setBrightness(180);
}

void loop()
{
    const unsigned long now = millis();

    // Trigger every 2s: LOW for 500ms, then HIGH again.
    if (now - lastTriggerStartMs >= triggerIntervalMs)
    {
        lastTriggerStartMs = now;
    }

    if (now - lastTriggerStartMs < triggerLowDurationMs)
    {
        digitalWrite(trigger_pin, LOW);
    }
    else
    {
        digitalWrite(trigger_pin, HIGH);
    }

    // Smoothly fade through the color wheel using HSV hue.
    if (now - lastAnimationStepMs >= animationStepMs)
    {
        lastAnimationStepMs = now;
        fill_solid(leds, numLeds, CHSV(hue++, 255, 255));
        FastLED.show();
    }
}
