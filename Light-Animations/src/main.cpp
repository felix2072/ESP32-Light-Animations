#include <light.h>
#include <Timer.h>

Light light = Light();

  // TIMER
Timer neoPixelTimer;
int neoPixelInterval = 5000;

int commandCounter = 1;
char command = '1';

int potValue1 = 0;
int potValue2 = 0;
int potpin1 = 7;

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
  potValue1 = analogRead(potpin1);
  potValue1 = potValue1/16;
  Serial.println(potValue1);

 /*  potValue2 = analogRead(potpin2);
  Serial.println(potValue2/16); */
  
  if(neoPixelTimer.read() > neoPixelInterval){
    neoPixelTimer.stop();
    command = char(48 + commandCounter);
    Serial.print("Timer command: ");
    Serial.println(command);
    if(commandCounter == 2){
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

  switch ('2') {
    case '1':
      light.updateRainbowColor(potValue1);
      break;
    case '2':
      light.updateNoiseColor(0,potValue1);
      break;
    case '3':
      light.updateNoiseColor(1,potValue1);
      break;
    case '4':
      light.updateNoiseColor(2,potValue1);
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