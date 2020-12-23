// necessary libraries
#include <Adafruit_NeoPixel.h>
#include <stdio.h>

// pin assignment
#define LED_PIN   5   // LED data
#define LED_AMNT  10  // amount of LEDs
#define poti      A2  // potentiometer

// object initialization
Adafruit_NeoPixel led(LED_AMNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// color definition
int purple = led.Color(0, 128, 128);      // minute
int yellow = led.Color(0, 128, 0);        // hour
int blue = led.Color(0, 78, 81);          // minute and hour
int black = led.Color(0, 0, 0);
int scheme[] = {purple, yellow, blue, black};

// setup
void setup()
{  
  // LED configuration
  led.begin();
  led.clear();
  led.setBrightness(128);
}

// global variable
int cnt = 0;
int sec = 180;

void loop()
{
  led.clear();
  determineColor();
  led.setBrightness(map(analogRead(poti), 0, 1024, 0, 255));                                                                                                                                                                                                
  led.show();
  delay(sec * 1000);
}

void determineColor()
{
    for (int n = 1; n <= 5; n++)
    {
        int rand = 0;
        switch(n)
        {
        case 1:
            rand = random(0,3);
            led.setPixelColor(0, scheme[rand]);
            break;
        case 2:
            rand = random(0,3);
            led.setPixelColor(1, scheme[rand]);
            break;
        case 3:
            rand = random(0,3);
            led.setPixelColor(2, scheme[rand]);
            led.setPixelColor(3, scheme[rand]);
            break;
        case 4:
            rand = random(0,3);
            led.setPixelColor(4, scheme[rand]);
            led.setPixelColor(5, scheme[rand]);
            break;
        case 5:
            rand = random(0,3);
            led.setPixelColor(6, scheme[rand]);
            led.setPixelColor(7, scheme[rand]);
            led.setPixelColor(8, scheme[rand]); 
            led.setPixelColor(9, scheme[rand]);     
            break;
        }
    }
}
