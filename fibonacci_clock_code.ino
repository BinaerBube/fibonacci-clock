// necessary libraries
#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <TimeLib.h>
#include <DS1302.h>

// pin assignment
#define LED_PIN   5   // LED data
#define LED_AMNT  10  // amount of LEDs
#define kCePin    8   // RTC enable
#define kIoPin    7   // RTC data
#define kSclkPin  6   // serial clock
#define poti      A0  // potentiometer
#define sUp       2   // switch up -> configure mode
#define sDown     3   // switch down -> brightness mode
#define but       4   // button

// object initialization
Adafruit_NeoPixel led(LED_AMNT, LED_PIN, NEO_GRB + NEO_KHZ800);
DS1302 rtc(kCePin, kIoPin, kSclkPin);

// color definition
int purple = led.Color(0, 128, 128);      // minute
int yellow = led.Color(0, 128, 0);        // hour
int blue = led.Color(0, 78, 81);          // minute and hour
int scheme[] = {purple, yellow, blue};

// displaying day
String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

// setup
void setup()
{
  // adjusting pinmodes
  pinMode(poti, INPUT);
  pinMode(sUp, INPUT);
  pinMode(sDown, INPUT);
  pinMode(but, INPUT);
  
  // LED configuration
  led.begin();
  led.clear();
  led.setBrightness(128);
  
  // RTC configuration
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2020, 10, 21, 16, 50, 5, Time::kWednesday); // launch time of the project
  rtc.time(t);

  // Serial monitor configuration
  Serial.begin(9600);
}

// global variable
int cnt = 0;

void loop()
{
  Serial.print("Schalter Unten: ");
  Serial.println(digitalRead(sDown));
  
  Serial.print("Schalter Oben: ");
  Serial.println(digitalRead(sUp));

  Serial.print("Counter: ");
  Serial.println(cnt);
  
  if(digitalRead(sDown))
  {
    if(digitalRead(but))
    {
      cnt++;
      delay(250);
    }

    int new_min = (cnt*5)%720;
    int new_hr = new_min / 60;
    new_min = new_min - (new_hr*60);

    Time new_t(2020, 10, 21, new_hr, new_min, 5, Time::kWednesday);
    rtc.time(new_t);
  }
  else if(digitalRead(sUp))
  {
    cnt = (rtc.time().hr % 12) * 12 + (rtc.time().min)/5;
    led.setBrightness(map(analogRead(poti), 0, 1024, 0, 255));
  }
  
  led.clear();
  determineColor();                                                                                                                                                                                                     
  led.show();
  printTime();
}

void determineColor()
{
  int fields[] = {5, 3, 2, 1, 10};
  
  Time z = rtc.time();
  int m = z.min/5;
  int h = z.hr % 12;

  for(int i = 0; i < 5; i++)
  {
    bool minb = false;
    bool hrb = false;

    if(m-fields[i] >= 0) 
    {
      minb = true;
      m -= fields[i];
    }

    if(h-fields[i] >= 0)
    {
      hrb = true;
      h -= fields[i];
    }
    
    setColor(5-i, col(minb, hrb));
  }
  Serial.println("-------");
}

void setColor(int n, int col)
{
  if(col == 0) return;
  
  col = col - 1;

  switch(n)
  {
    case 1:
      if(rtc.time().hr % 2 == 0) led.setPixelColor(1, scheme[col]);
      else led.setPixelColor(0, scheme[col]);
      break;
    case 2:
      if(rtc.time().hr % 2 == 1) led.setPixelColor(0, scheme[col]);
      else led.setPixelColor(1, scheme[col]);
      break;
    case 3:
      led.setPixelColor(2, scheme[col]);
      led.setPixelColor(3, scheme[col]);
      break;
    case 4:
      led.setPixelColor(4, scheme[col]);
      led.setPixelColor(5, scheme[col]);
      break;
    case 5:
      led.setPixelColor(6, scheme[col]);
      led.setPixelColor(7, scheme[col]);
      led.setPixelColor(8, scheme[col]); 
      led.setPixelColor(9, scheme[col]);     
      break;
  }
}

int col(bool minb, bool hrb)
{
  int col = 0; // 1 == minute, 2 == hour, 3 == both
  if(minb) col +=1;
  if(hrb) col +=2;
  return col;
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
}
