/*
This example will receive multiple universes via Art-Net and control a strip of
WS2812 LEDs via the FastLED library: https://github.com/FastLED/FastLED
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/
#include <WiFi.h>
#include <WiFIAP.h>
#include <ArtnetWifi.h>
#include <Arduino.h>
#include <FastLED.h>

// Wifi settings
const char* ssid     = "mobelebobele";
const char* password = "R?J#4?4yNdmVgUiA=A";

// LED settings
const int numLeds = 40; // CHANGE FOR YOUR SETUP

const byte dataPin_A = 36;
const byte dataPin_B = 37;
CRGB leds_A[numLeds];
CRGB leds_B[numLeds];
CRGB* targetLeds = nullptr;

// Art-Net settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = 2;
bool universesReceived[2];

// connect to wifi – returns true if successful or false if not
void initWifiAP()
{
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void initWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{

  uint8_t index = universe - startUniverse;

  // Store which universe has got in
  universesReceived[index] = true;


  if (universe == 0) targetLeds = leds_A;
  if (universe == 1) targetLeds = leds_B;

  if (targetLeds) {
    for (int i = 0; i < min(numLeds, length / 3); i++) {
      targetLeds[i] = CRGB(
        data[i * 3],
        data[i * 3 + 1],
        data[i * 3 + 2]
      );
    }
  }

  Serial.print(" universe: ");
  Serial.print(universe);
  Serial.print(" length: ");
  Serial.println(length);
  
  FastLED.show();
  // Reset universeReceived to 0
  memset(universesReceived, 0, maxUniverses);
  
}

void setup()
{
  Serial.begin(115200);
  initWifi();

  artnet.begin();
  FastLED.addLeds<WS2812, dataPin_A, GRB>(leds_A, numLeds);
  FastLED.addLeds<WS2812, dataPin_B, GRB>(leds_B, numLeds);

  memset(universesReceived, 0, maxUniverses);
  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}
