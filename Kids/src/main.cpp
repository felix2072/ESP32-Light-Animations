#include <light.h>
#include <Adafruit_NeoPixel.h>
#include <Timer.h>

#define LED_BUILDIN 8
#define MAXLED 40

int ar = 50;
int ag = 50;
int ab = 50;
int br = 50;
int bg = 50;
int bb = 50;

int16_t position = 200;
u_int8_t speed[6] = {3,2,10,1,10,0};
boolean left = false;
boolean right = false;
boolean locked = false;
boolean next = false;
int mode = 2;
Timer modeTimer;
uint32_t modeInterval = 250;

Timer colorWheelTimer;
uint32_t colorWheelInterval = 50;

Light light = Light(10,4);

Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(MAXLED, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(MAXLED, 10, NEO_GRB + NEO_KHZ800);
uint16_t colorIndex_1 = 0;
uint16_t colorIndex_2 = 60;
uint8_t active_1[MAXLED] = {0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0}; //inside
uint8_t active_2[MAXLED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,0,0,0}; //outside
u_int8_t maxBrightness[6] = {200,200,255,90,255,0};


void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);
  while(!Serial);
  Serial.println("setup");

  light.initNeoPixel(pixels_1, true);
  light.initNeoPixel(pixels_2, false);
                   
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}


uint32_t Color(byte red, byte green, byte blue){
  return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}


void modes(){
  
  switch (mode)
  {
  case 0:
    if(colorWheelTimer.state() == STOPPED){
      colorWheelTimer.start();
      light.updateRainbowColor(pixels_1, true, colorIndex_1);
      light.updateRainbowColor(pixels_2, false, colorIndex_2);
    }
    if(colorWheelTimer.read() >= position){
      colorWheelTimer.stop();
    }
    light.setBrightness(pixels_1, position, 205);
    light.setBrightness(pixels_2, position, 205);
    break;
  case 1:
    light.paintMode(pixels_1, Color(0,255,0), Color(143,0,26), active_1);
    light.paintMode(pixels_2, Color(235, 0, 28), Color(0, 85, 163), active_2);
    light.setBrightness(pixels_1, position, 210);
    light.setBrightness(pixels_2, position, 210);
    break;
  case 2:
    light.LerpMode(pixels_1, Color(20,255,70), Color(255,20,20),position);
    light.LerpMode(pixels_2, Color(255,20,20), Color(30,60,255),position);
    light.setBrightness(pixels_1, 150, 160);
    light.setBrightness(pixels_2, 150, 160);
    break;
  case 3:
    
    light.setLEDColor(pixels_1, 255,255,255,position);
    light.setLEDColor(pixels_2, 255,255,255,position);
    light.setBrightness(pixels_1, position, 90);
    light.setBrightness(pixels_2, position, 90);
    break;
  case 4:
    light.LerpMode(pixels_1, Color(180,200,255), Color(255,180,100),position);
    light.LerpMode(pixels_2, Color(180,200,255), Color(255,180,100),position);
    light.setBrightness(pixels_1, 150, 160);
    light.setBrightness(pixels_2, 150, 160);
    break;
  default:
    light.setLEDColor(pixels_1, 0,0,0);
    light.setLEDColor(pixels_2, 0,0,0);
    break;
  }
}

void rotaryEncoder(){
  
  left = digitalRead(5);
  right = digitalRead(6);
  next = !digitalRead(7);
  if(next && modeTimer.state() == STOPPED){
    modeTimer.start();
    mode++;
    if(mode == 6){
      mode = 0;
    }
    position = maxBrightness[mode];
    Serial.println(mode);
  }
  if(modeTimer.read() >= modeInterval){
    modeTimer.stop();
  }

  if(left || right){
    
    if (!left && !locked && position < maxBrightness[mode])
    {
      position += speed[mode];
      
      if(position > 255){
        position = 255;
      }
      Serial.print("position ");
      Serial.println(position);
      digitalWrite(LED_BUILDIN, HIGH);
      locked = true;
    }
    else if (!right && !locked)
    {
      position -= speed[mode];
      if(position < 0){
        position = 0;
      }
      Serial.print("position ");
      Serial.println(position);
      digitalWrite(LED_BUILDIN, LOW);
      locked = true;
    }
    
  }
  
  if(left && right){
    locked = false;
  }
}

void loop()
{
  rotaryEncoder();
  modes();
}

