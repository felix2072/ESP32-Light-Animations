#include <light.h>
#include <Timer.h>

Light light = Light();

  // TIMER
Timer neoPixelTimer;
int neoPixelInterval = 5000;

int commandCounter = 1;
char command = '1';

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
  neoPixelTimer.start();
}

void loop()
{
  
  if(neoPixelTimer.read() > neoPixelInterval){
    neoPixelTimer.stop();
    command = char(48 + commandCounter);
    Serial.print("Timer command: ");
    Serial.println(command);
    if(commandCounter == 6){
      commandCounter = 1;
    }else{
      commandCounter++;
    }
    neoPixelTimer.start();
  }

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
      light.updateNoiseColor(2);
      break;
    case '5':
      light.serverAnimationUpdate(0);
      break;
    case '6':
      light.serverAnimationUpdate(1);
      break;
    case '0':
      light.setLEDColor(0, 0, 0);
      break;
    default:
      break;
  }
}