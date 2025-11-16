#include <light.h>

Light light = Light();
char command = '0';

void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("setup");

  light.initNeoPixel();
  light.setLEDColor(40, 0, 0);
  delay(500);
  light.setLEDColor(0, 0, 0);
  delay(500);
  light.setLEDColor(40, 0, 0);
  delay(500);
  light.setLEDColor(0, 0, 0);
}

void loop()
{
  // serial.read();
  if (Serial.available()) {
    command = Serial.read();
    Serial.print("Received command: ");
    Serial.println(command);
  }
  switch (command) {
    case '1':
      light.updateRainbowColor();
      break;
    case '2':
      light.updateNoiseColor(0);
      break;
    case '3':
      light.updateNoiseColor(1);
      break;
    case '4':
      light.serverAnimationUpdate();
      break;
    case '5':
      light.onePixelTest();
      break;
    case '0':
      light.setLEDColor(0, 0, 0);
      break;
    default:
      break;
  }
}