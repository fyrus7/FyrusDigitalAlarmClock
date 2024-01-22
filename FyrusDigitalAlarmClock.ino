/*
*
*  Digital Alarm Clock by Fyrus [ https://iamfyrus.com ]
*
*  2 Button operation:
*  - Button 1 to select
*  - Button 2 to scroll down
*  - Press both Button to enter Menu
*
*  Alarm Triggered condition:
*  - Button 1 to Snooze / add 5 minute snooze
*  - Button 2 to Clear  / clear for 24 hours
*
*  Things:
*  - Arduino Pro Mini 3.3v 8Mhz
*  - DS3231 RTC
*  - 128x64 0'96 Oled Display SSD1306
*  - Active Buzzer
*  - 2 mini Button
*  - Small On/Off Switch
*  - 3.7v Lithium Battery
*  - TP4056 Battery Charger
*  - Small Electrical Box
*
*/

// 'Select_Buttom', 5x3px
const unsigned char Select_Buttom [] PROGMEM = {
  0xf8, 0x70, 0x20
};

#include "RTClib.h"
#include <TimerOne.h>
#include <LowPower.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FyrusClockFontL.h>
#include <Fonts/FyrusClockFontS2.h>


RTC_DS3231 rtc;
Adafruit_SSD1306 display(128, 64, &Wire, -1);


byte _day, _month, _hour, _hour12, _minute, _second, _dtw;
int _year;
char st[2];
char _nameoftheDay[9];
char _monthsName[3];

const char daysOfTheWeek_0[] PROGMEM = "Sun";   
const char daysOfTheWeek_1[] PROGMEM = "Mon";
const char daysOfTheWeek_2[] PROGMEM = "Tue";
const char daysOfTheWeek_3[] PROGMEM = "Wed";
const char daysOfTheWeek_4[] PROGMEM = "Thu";
const char daysOfTheWeek_5[] PROGMEM = "Fri";
const char daysOfTheWeek_6[] PROGMEM = "Sat";

const char* const daysOfTheWeek_table[] PROGMEM = {
  daysOfTheWeek_0,
  daysOfTheWeek_1,
  daysOfTheWeek_2,
  daysOfTheWeek_3,
  daysOfTheWeek_4,
  daysOfTheWeek_5,
  daysOfTheWeek_6};

const char months_name_0[] PROGMEM = "Jan";   
const char months_name_1[] PROGMEM = "Feb";
const char months_name_2[] PROGMEM = "Mar";
const char months_name_3[] PROGMEM = "Apr";
const char months_name_4[] PROGMEM = "May";
const char months_name_5[] PROGMEM = "Jun";
const char months_name_6[] PROGMEM = "Jul";
const char months_name_7[] PROGMEM = "Aug";
const char months_name_8[] PROGMEM = "Sep";
const char months_name_9[] PROGMEM = "Oct";
const char months_name_10[] PROGMEM = "Nov";
const char months_name_11[] PROGMEM = "Dec";

const char* const months_name_table[] PROGMEM = {
  months_name_0, months_name_1, months_name_2, months_name_3,
  months_name_4, months_name_5, months_name_6, months_name_7,
  months_name_8, months_name_9, months_name_10, months_name_11,};


byte b_GOTO, b_DOWN;
byte Buzzer = 5;

// Menus
bool Menu_Stat = false;
byte Menu_Cnt = 0;
bool Menu_Set = false;
byte Menu_Set_cnt;
byte Menu_Set_TimeDate = 0;

// Time & Date
bool set_Hour = false;
bool set_Minute = false;
bool set_Day = false;
bool set_Month = false;
bool set_Year = false;

bool blink_cursor = false;
int _delay = 100;

// Alarm 1
byte Alarm_Stat = 0;
bool Set_hour_alarm = false;
bool Set_minute_alarm = false;
byte Menu_Set_Alarm = 0;

// Alarm 2
byte Alarm_Stat2 = 0;
bool Set_hour_alarm2 = false;
bool Set_minute_alarm2 = false;
byte Menu_Set_Alarm2 = 0;

// Alarm Start & Sound
bool Alarm_Sound = false;
bool Alarm_Start = false;
byte Alarm_Duration = 0;


unsigned long previousGetTimeDate = 0;    // will store last time was updated
const long intervalGetTimeDate = 1000;    // interval (milliseconds) 

const unsigned long sleepTime = 15000;    // 15 second to sleep mode
volatile unsigned long lastActivity = 0;  // Detect last button activity and start timer
const int CRT_display = 300;              // CRT Off Animation duration in milliseconds


void wakeUp(){
  // let this empty
}

void timerISR() {
  // let this empty
}


void setup() {

  pinMode(2, INPUT_PULLUP); // interrupt INT0
  pinMode(3, INPUT_PULLUP); // interrupt INT1
  pinMode(4, INPUT_PULLUP);

  pinMode(Buzzer, OUTPUT);

  rtc.begin();

  rtc.disable32K(); // disable 32K Pin

  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);

  rtc.writeSqwPinMode(DS3231_OFF); // disable SquareWave = Enable INTerrupt

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print(F("Fyrus DAC : v1.0_S"));

  display.display();
  delay(3000);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(timerISR);

}


void loop() {

     unsigned long currentGetTimeDate = millis();
     if (currentGetTimeDate - previousGetTimeDate >= intervalGetTimeDate){
        previousGetTimeDate = currentGetTimeDate;

    // Display Time and Date if Menu Status = False
    if (Menu_Stat == false) {

      GetDateTime();
 
      display.clearDisplay();

      if (Alarm_Stat == 1 && Alarm_Stat2 == 1){
        ALARM1();
        ALARM2();

        } else if (Alarm_Stat2 == 1){
          ALARM2();
          
        } else if (Alarm_Stat == 1) {
          ALARM1();
            
        } else {
        display.setTextSize(1);
        display.setCursor(2, 0);
        display.print(F("ALARM OFF"));
      }


        // Alarm Triggered
        if (rtc.alarmFired(1) == true || rtc.alarmFired(2) == true){
          Alarm_Start = true;
          Alarm_Sound = !Alarm_Sound;
          if (Alarm_Sound == true) {
            Button_Sound(1);
            } else {
              Button_Sound(0);
              }

        Alarm_Duration++;
        // 299 = 5 min Alarm Duration
        if (Alarm_Duration > 299) {
          rtc.clearAlarm(1);
          rtc.clearAlarm(2);
          Button_Sound(0);
          Alarm_Duration = 0;
          Alarm_Start = false;
        }
        Alarm_Notify();
       }

        else {
          // Default Clock Loop
          Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);
          }
        display.display();
        }
      }


  read_button();


  // Stop Alarm 1
  if (Alarm_Start == true && b_DOWN == LOW && rtc.alarmFired(1) == true) {
    rtc.clearAlarm(1);
    Button_Sound(0);
    Alarm_Start = false;
    Alarm_Duration = 0;
    Clear();
    // Snooze Alarm 1 for 5 minute
    } else if (Alarm_Start == true && b_GOTO == LOW && rtc.alarmFired(1) == true){
      rtc.clearAlarm(1);
      rtc.disableAlarm(1);
      Button_Sound(0);
      Alarm_Start = false;
      Alarm_Duration = 0;
      rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 5, 0), DS3231_A1_Minute);
      Snooze();
      }

  // Stop Alarm 2
  if (Alarm_Start == true && b_DOWN == LOW && rtc.alarmFired(2) == true) {
    rtc.clearAlarm(2);
    Button_Sound(0);
    Alarm_Start = false;
    Alarm_Duration = 0;
    Clear();
    // Snooze Alarm 2 for 5 minute
    } else if (Alarm_Start == true && b_GOTO == LOW && rtc.alarmFired(2) == true){
      rtc.clearAlarm(2);
      rtc.disableAlarm(2);
      Button_Sound(0);
      Alarm_Start = false;
      Alarm_Duration = 0;
      rtc.setAlarm2(rtc.now() + TimeSpan(0, 0, 5, 0), DS3231_A2_Minute);
      Snooze();
      }


  // Press both Button = go to Display Menu
  if (b_DOWN == LOW && b_GOTO == LOW && Menu_Stat == false) {
    delay(500);
    Menu_Cnt = 1;
    Menu_Stat = true;
  }

  
  // display Menu
  while (Menu_Stat == true) {
    read_button();
    // for selecting Setup Menu
    if (b_DOWN == LOW) {
      delay(100);
      Menu_Cnt++;
      if (Menu_Cnt > 4) Menu_Cnt = 1;
    }

    if (b_GOTO == LOW && Menu_Cnt == 1) {
      delay(500);
      Menu_Set = true;
      Menu_Set_cnt = 1;
      Menu_Set_Alarm = 1;      
    }
  
    if (b_GOTO == LOW && Menu_Cnt == 2) {
      delay(500);
      Menu_Set = true;
      Menu_Set_cnt = 2;
      Menu_Set_Alarm2 = 1;
    }

    if (b_GOTO == LOW && Menu_Cnt == 3) {
      delay(500);
      Menu_Set = true;
      Menu_Set_cnt = 3;
      Menu_Set_TimeDate = 1;
    }

    if (b_DOWN == LOW && Menu_Cnt == 4) {
      delay(500);
      Menu_Stat = false;
    }


    // Menu contents
    if (Menu_Cnt == 1) {
      Menu_Display(Menu_Cnt);
    } else if (Menu_Cnt == 2) {
      Menu_Display(Menu_Cnt);
    } else if (Menu_Cnt == 3) {
      Menu_Display(Menu_Cnt);
    }


    // Time and Date Setting Menu
    while (Menu_Set == true && Menu_Set_cnt == 1) {
      Set_Alarm();
      delay(50);
    }

    // Alarm Settings Menu
    while (Menu_Set == true && Menu_Set_cnt == 2) {
      Set_Alarm2();
      delay(50);
    }

    while (Menu_Set == true && Menu_Set_cnt == 3) {
      Set_Time_Date();
      delay(50);
    }
    delay(50);
  }

  // Check if it's time to sleep when no button is pressed.
  if (millis() - lastActivity > sleepTime) {
    displayOff();
    Timer1.stop();
    sleep();
    Timer1.start();
  }
}


void displayOff() { // turn display off when sleep.
  unsigned long Start = millis();
  while (millis() - Start <= CRT_display) {
    int width = map(millis() - Start, 0, CRT_display, 128, 0);
    display.clearDisplay();
    display.fillRect((128 - width) / 2, 64 / 2, width, 2, WHITE);   
    display.display();
  }
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void sleep() { // sleep forever until interrupt
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), wakeUp, FALLING);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  display.ssd1306_command(SSD1306_DISPLAYON);
}


void read_button() {
  b_GOTO = digitalRead(3);
  b_DOWN = digitalRead(4);

  if (b_GOTO == LOW || b_DOWN == LOW || digitalRead(2) == LOW){
    lastActivity = millis();
  }
}


void GetDateTime(){
  DateTime now = rtc.now();
  _day    = now.day();
  _month  = now.month();
  _year   = now.year();
  _hour   = now.hour();
  _minute = now.minute();
  _second = now.second();
  _dtw    = now.dayOfTheWeek();

  if (_hour<12) {
    strcpy(st, "AM");
  }
  else {
    strcpy(st, "PM");
  }
}



// display Clock and Date
void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr) {

  if (Menu_Stat == false) {
  display.drawLine(0, 12, 128, 12, WHITE);
  display.drawLine(0, 50, 128, 50, WHITE);
  }

  display.setTextColor(WHITE);

  int colon   = s;
  int hour12_ = h;
  bool isPM_  = false;

  if (hour12_ >= 12) {
         isPM_ = true;
   if (hour12_ > 12) {
      hour12_ -= 12;
      }
  } else if (hour12_ == 0) {
       hour12_ = 12;
  }

  display.setTextSize(1);
  display.setFont(&FyrusClockFontL);
  display.setCursor(1, 41);
  if (hour12_ < 10) {
     display.print(F("0"));
   }
     display.print(hour12_);
  if (colon % 2 == 0) {
    display.print(F(" "));
  } else {
    display.print(F(":"));
  }
  display.setCursor(51, 41);
  if (m < 10) {
    display.print(F("0"));
  }
    display.print(m);
  if (colon % 2 == 0) {
    display.print(F(" "));
  } else {
    display.print(F(":"));
  }
  display.setFont();
  
  display.setFont(&FyrusClockFontS2);
  display.setCursor(102, 41);
  if (s < 10) {
    display.print(F("0"));
    display.print(s);
  } else {
    display.print(s);
  }
  display.setFont();

  display.setCursor(98, 25);
  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);
  display.print(isPM_ ? F(" pm") : F(" am"));
  display.setFont();

    // display Day Name
    strcpy_P(_nameoftheDay,(char*)pgm_read_word(&(daysOfTheWeek_table[dtw])));
    display.setTextSize(1);
    display.setCursor(18, 56);
    display.print(_nameoftheDay);
    display.print(F(", "));

    // display Date
    strcpy_P(_monthsName,(char*)pgm_read_word(&(months_name_table[mt-1])));
    if (dy < 10) {
    display.print(F("0"));
    display.print(dy);
  } else {
    display.print(dy);
  }
    display.print(F(" "));
    display.print(_monthsName);
    display.print(F(" "));
    display.print(yr);
}


// Menu Display
void Menu_Display(byte slct) {
  display.clearDisplay();

  // Cursor position for Menu
  if (slct == 1) {
    display.setTextSize(1);
    display.setFont(&FyrusClockFontS2);

  display.setCursor(16, 38);
  display.print(F("ALARM 1"));

  } else if (slct == 2) {
    display.setTextSize(1);
    display.setFont(&FyrusClockFontS2);

  display.setCursor(16, 38);
  display.print(F("ALARM 2"));
    
  } else if (slct == 3) {
    display.setTextSize(1);
    display.setFont(&FyrusClockFontS2);

  display.setCursor(1, 38);
  display.print(F("TIME : DATE"));

  }
  display.setFont();
  display.display();
}



// displaying Time and Date settings
void Set_Time_Date() {

  read_button();

  display.clearDisplay();
  
  Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);


  if (b_DOWN == LOW) {
    delay(100);
    Menu_Set_TimeDate++;
    if (Menu_Set_TimeDate > 7) Menu_Set_TimeDate = 1;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 1) {
    delay(500);
    set_Hour = true;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 2) {
    delay(500);
    set_Minute = true;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 3) {
    delay(500);
    set_Day = true;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 4) {
    delay(500);
    set_Month = true;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 5) {
    delay(500);
    set_Year = true;
  }

  if (b_GOTO == LOW && Menu_Set_TimeDate == 6) {
    delay(500);
    rtc.adjust(DateTime(_year, _month, _day, _hour, _minute, 0));
    Display_to_save();
  }


  if (b_DOWN == LOW && Menu_Set_TimeDate == 7) {
    delay(100);
    Menu_Set_cnt = 0;
    Menu_Set = false;
    Menu_Stat = false; // remove to go back to Menu
  }


  // Cursor position for Time & Date settings
  if (Menu_Set_TimeDate == 1) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 2) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 3) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 4) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 5) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 6) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }

  display.display();

  // set Hour
  while (set_Hour == true) {
    read_button();
    display.clearDisplay();

    Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);

    if (b_DOWN == LOW) {
      _delay = 50;
      delay(_delay);
      _hour++;
      if (_hour > 23) _hour = 0;
    }

    if (b_GOTO == LOW) {
      delay(500);
      set_Hour = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(3, 15, 40, 40, BLACK);
    }
    
    display.display();
    delay(_delay);
  }


  // set Minute
  while (set_Minute == true) {
    read_button();
    display.clearDisplay();

    Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);

    if (b_DOWN == LOW) {
      _delay = 50;
      delay(_delay);
      _minute++;
      if (_minute > 59) _minute = 0;
    }

    if (b_GOTO == LOW) {
      delay(500);
      set_Minute = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(52, 15, 40, 40, BLACK);
    }
    
    display.display();
    delay(_delay);
  }


  // set Day / Date
  while (set_Day == true) {
    read_button();
    display.clearDisplay();

    Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);

    if (b_DOWN == LOW) {
      _delay = 50;
      delay(_delay);
      _day++;
      if (_day > 31) _day = 1;
    }

    if (b_GOTO == LOW) {
      delay(500);
      set_Day = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(48, 55, 12, 8, BLACK);
    }
    
    display.display();
    delay(_delay);
  }


  // set Month
  while (set_Month == true) {
    read_button();
    display.clearDisplay();

    Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);

    if (b_DOWN == LOW) {
      _delay = 50;
      delay(_delay);
      _month++;
      if (_month > 12) _month = 1;
    }

    if (b_GOTO == LOW) {
      delay(500);
      set_Month = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(66, 55, 20, 8, BLACK);
    }
    
    display.display();
    delay(_delay);
  }


  // set Year
  while (set_Year == true) {
    read_button();
    display.clearDisplay();

    Digital_Clock_and_Date(_hour, _minute, _second, _dtw, _day, _month ,_year);

    if (b_DOWN == LOW) {
      _delay = 50;
      delay(_delay);
      _year++;
      if (_year > 2100) _year = 2000;
    }

    if (b_GOTO == LOW) {
      delay(500);
      set_Year = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(90, 55, 27, 8, BLACK);
    }
    
    display.display();
    delay(_delay);
  }
}

// Subroutines for displaying a cursor to select settings
void Set_Time_Date_Display(byte slc) {
   display.setTextSize(1);
   display.setTextColor(WHITE);
    
  if (slc == 1) {
    //Set Hour
    display.drawBitmap(18, 10, Select_Buttom, 5, 3, WHITE);
  } else if (slc == 2) {
    //Set Minute
    display.drawBitmap(70, 10, Select_Buttom, 5, 3, WHITE);
  } else if (slc == 3) {
    //Set Day
    display.drawBitmap(51, 49, Select_Buttom, 5, 3, WHITE);
  } else if (slc == 4) {
    //Set Month
    display.drawBitmap(74, 49, Select_Buttom, 5, 3, WHITE);
  } else if (slc == 5) {
    //Set Year
    display.drawBitmap(99, 49, Select_Buttom, 5, 3, WHITE);
  } else if (slc == 6) {
    //Save Settings
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(&FyrusClockFontS2);
    display.setCursor(35, 38);
    display.print(F("SAVE"));
    display.setFont();
    display.setCursor(15, 47);
    display.print(F("or next to Exit >"));
  }
}


// Save Alarm 1 in DS3231 memory
void setAlarmTime(int hour, int minute) {
  rtc.disableAlarm(1);
  rtc.clearAlarm(1);

  DateTime now = rtc.now();
  DateTime alarmTime(now.year(), now.month(), now.day(), hour, minute, 0);

  if (alarmTime < now) {
    // If the alarm time is in the past, set it for the next day
    alarmTime = alarmTime + TimeSpan(1, 0, 0, 0);
  }
  rtc.setAlarm1(alarmTime, DS3231_A1_Hour);
}


// Alarm 1 Settings display
void Set_Alarm() {
  read_button();
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  Set_Alarm_Display();

  if (b_DOWN == LOW) {
    delay(100);
    Menu_Set_Alarm++;
    if (Menu_Set_Alarm > 5) Menu_Set_Alarm = 1;
  }

  if (b_GOTO == LOW && Menu_Set_Alarm == 1) {
    delay(250);
    Set_hour_alarm = true;
  }

  if (b_GOTO == LOW && Menu_Set_Alarm == 2) {
    delay(250);
    Set_minute_alarm = true;
  }

  // Enable Alarm 1
  if (b_GOTO == LOW && Menu_Set_Alarm == 3) {
    delay(500);
   
    Display_to_save();

    Alarm_Stat = 1;
    delay(100);
  }


  // Disable Alarm 1
  if (b_GOTO == LOW && Menu_Set_Alarm == 4) {
    delay(500);

    rtc.disableAlarm(1);
    rtc.clearAlarm(1);
    
    Display_to_save();
    
    Alarm_Stat = 0;
    delay(100);
  }


  if (b_GOTO == LOW && Menu_Set_Alarm == 5) {
    delay(500);
    Menu_Set_Alarm = 0;
    Menu_Stat = false; // remove to go back to Menu
    Menu_Set = false;
  }


  // Cursor position on the Alarm 1 Settings
  if (Menu_Set_Alarm == 1) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 2) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 3) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 4) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 5) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }


  // set Hour Alarm 1
  while (Set_hour_alarm == true && Menu_Set_Alarm == 1) {
    read_button();
    
    display.clearDisplay();

    if (b_DOWN == LOW) {
      DateTime alarmTime = rtc.getAlarm1();
      _delay = 50;
      delay(_delay);
      int newHour = (alarmTime.hour() + 1) % 24;
      setAlarmTime(newHour, alarmTime.minute());
    }

    if (b_GOTO == LOW) {
      delay(500);
      Set_hour_alarm = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(22, 8, 25, 20, BLACK);
    }

    display.display();
    delay(_delay);
  }


  // set Minute Alarm 1
  while (Set_minute_alarm == true && Menu_Set_Alarm == 2) {
    read_button();
    
    display.clearDisplay();

    if (b_DOWN == LOW) {
      DateTime alarmTime = rtc.getAlarm1();
      _delay = 50;
      delay(_delay);
      int newMinute = (alarmTime.minute() + 1) % 60;
      setAlarmTime(alarmTime.hour(), newMinute);
    }

    if (b_GOTO == LOW) {
      delay(500);
      Set_minute_alarm = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(60, 8, 25, 20, BLACK);
    }

    display.display();
    delay(_delay);
  }
  
  display.display();
}


// Alarm 1 Settings display
void Set_Alarm_Display() {

  if (Alarm_Stat == 0) {
    display.setTextSize(1);
    display.setCursor(90, 35);
    display.print(F("Status"));
    display.setFont(&FyrusClockFontS2);
    display.setCursor(88, 60);
    display.print(F("OFF"));
    display.setFont();
  } else {
    display.setTextSize(1);
    display.setCursor(90, 35);
    display.print(F("Status"));
    display.setFont(&FyrusClockFontS2);
    display.setCursor(94, 60);
    display.print(F("ON"));
    display.setFont();
  }

  DateTime alarmTime = rtc.getAlarm1();

  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);

        display.setCursor(22, 22);
        int hour12_ = alarmTime.hour();
        bool isPM_  = false;
        
        if (hour12_ >= 12) {
              isPM_  = true;
        if (hour12_ > 12) {
            hour12_ -= 12;
            }
        } else if (hour12_ == 0) {
            hour12_ = 12;
        }
        if (hour12_ < 10) {
          display.print(F("0"));
        }
          display.print(hour12_, DEC);

          display.setCursor(52, 22);
          display.print(F(":"));

        display.setCursor(60, 22);
        if (alarmTime.minute() < 10) {
          display.print(F("0"));
        }
          display.print(alarmTime.minute(), DEC);

          display.setCursor(90, 25);
          display.print(isPM_ ? F(" pm") : F(" am"));

          display.setFont();
}


// Save Alarm 2 in DS3231 memory
void setAlarmTime2(int hour, int minute) {
  rtc.disableAlarm(2);
  rtc.clearAlarm(2);

  DateTime now = rtc.now();
  DateTime alarmTime(now.year(), now.month(), now.day(), hour, minute, 0);

  if (alarmTime < now) {
    // If the alarm time is in the past, set it for the next day
    alarmTime = alarmTime + TimeSpan(1, 0, 0, 0);
  }
  rtc.setAlarm2(alarmTime, DS3231_A2_Hour);
}


// Alarm 2 Settings
void Set_Alarm2() {
  read_button();
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  Set_Alarm_Display2();

  if (b_DOWN == LOW) {
    delay(100);
    Menu_Set_Alarm2++;
    if (Menu_Set_Alarm2 > 5) Menu_Set_Alarm2 = 1;
  }

  if (b_GOTO == LOW && Menu_Set_Alarm2 == 1) {
    delay(250);
    Set_hour_alarm2 = true;
  }

  if (b_GOTO == LOW && Menu_Set_Alarm2 == 2) {
    delay(250);
    Set_minute_alarm2 = true;
  }

  // Enable Alarm 2
  if (b_GOTO == LOW && Menu_Set_Alarm2 == 3) {
    delay(500);
   
    Display_to_save();

    Alarm_Stat2 = 1;
    delay(100);
  }


  // Disable Alarm 2
  if (b_GOTO == LOW && Menu_Set_Alarm2 == 4) {
    delay(500);
    // Disable and clear alarm
    rtc.disableAlarm(2);
    rtc.clearAlarm(2);
    
    Display_to_save();
    
    Alarm_Stat2 = 0;
    delay(100);
  }


  if (b_GOTO == LOW && Menu_Set_Alarm2 == 5) {
    delay(500);
    Menu_Set_Alarm2 = 0;
    Menu_Stat = false; // remove to go back to Menu
    Menu_Set = false;
  }


  // Cursor position on the Alarm 2 Settings
  if (Menu_Set_Alarm2 == 1) {
    Set_Alarm_Cursor(Menu_Set_Alarm2);
  } else if (Menu_Set_Alarm2 == 2) {
    Set_Alarm_Cursor(Menu_Set_Alarm2);
  } else if (Menu_Set_Alarm2 == 3) {
    Set_Alarm_Cursor(Menu_Set_Alarm2);
  } else if (Menu_Set_Alarm2 == 4) {
    Set_Alarm_Cursor(Menu_Set_Alarm2);
  } else if (Menu_Set_Alarm2 == 5) {
    Set_Alarm_Cursor(Menu_Set_Alarm2);
  }


  // set Hour Alarm 2
  while (Set_hour_alarm2 == true && Menu_Set_Alarm2 == 1) {
    read_button();
    
    display.clearDisplay();

    if (b_DOWN == LOW) {
      DateTime alarmTime = rtc.getAlarm2();
      _delay = 50;
      delay(_delay);
      int newHour = (alarmTime.hour() + 1) % 24;
      setAlarmTime2(newHour, alarmTime.minute());
    }

    if (b_GOTO == LOW) {
      delay(500);
      Set_hour_alarm2 = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    Set_Alarm_Display2();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(22, 8, 25, 20, BLACK);
    }

    display.display();
    delay(_delay);
  }


  // set Minute Alarm 2
  while (Set_minute_alarm2 == true && Menu_Set_Alarm2 == 2) {
    read_button();
    
    display.clearDisplay();

    if (b_DOWN == LOW) {
      DateTime alarmTime = rtc.getAlarm2();
      _delay = 50;
      delay(_delay);
      int newMinute = (alarmTime.minute() + 1) % 60;
      setAlarmTime2(alarmTime.hour(), newMinute);
    }

    if (b_GOTO == LOW) {
      delay(500);
      Set_minute_alarm2 = false;
    }

    if (b_DOWN == HIGH && b_GOTO == HIGH) {
      _delay = 100;
    }

    Set_Alarm_Display2();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.fillRect(60, 8, 25, 20, BLACK);
    }

    display.display();
    delay(_delay);
  }

  display.display();
}



// Alarm 2 Settings display
void Set_Alarm_Display2() {

  if (Alarm_Stat2 == 0) {
    display.setTextSize(1);
    display.setCursor(90, 35);
    display.print(F("Status"));
    display.setFont(&FyrusClockFontS2);
    display.setCursor(88, 60);
    display.print(F("OFF"));
    display.setFont();
  } else {
    display.setTextSize(1);
    display.setCursor(90, 35);
    display.print(F("Status"));
    display.setFont(&FyrusClockFontS2);
    display.setCursor(94, 60);
    display.print(F("ON"));
    display.setFont();
  }

  DateTime alarmTime = rtc.getAlarm2();

  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);

        display.setCursor(22, 22);
        int hour12_ = alarmTime.hour();
        bool isPM_  = false;
        
        if (hour12_ >= 12) {
              isPM_  = true;
        if (hour12_ > 12) {
            hour12_ -= 12;
            }
        } else if (hour12_ == 0) {
            hour12_ = 12;
        }
        if (hour12_ < 10) {
          display.print(F("0"));
        }
          display.print(hour12_, DEC);

          display.setCursor(52, 22);
          display.print(F(":"));

          display.setCursor(60, 22);
        if (alarmTime.minute() < 10) {
          display.print(F("0"));
        }
          display.print(alarmTime.minute(), DEC);

          display.setCursor(90, 25);
          display.print(isPM_ ? F(" pm") : F(" am"));

          display.setFont();
}



// Cursor position in Alarm Settings
void Set_Alarm_Cursor(byte slc) {
  if (slc == 1) {
    display.drawBitmap(32, 0, Select_Buttom, 5, 3, WHITE);

  } else if (slc == 2) {
    display.drawBitmap(70, 0, Select_Buttom, 5, 3, WHITE);

  } else if (slc == 3) {
    display.setTextSize(1);
    display.setCursor(7, 45);
    display.print(F("Turn ON  >"));

  } else if (slc == 4) {
    display.setTextSize(1);
    display.setCursor(7, 45);
    display.print(F("Turn OFF >"));

  } else if (slc == 5) {
    display.setTextSize(1);
    display.setCursor(7, 45);
    display.print(F("< Exit"));
  }
}


void Display_to_save() {
  display.clearDisplay(); 
  
  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);
  display.setTextColor(WHITE);
  display.setCursor(28, 38);
  display.print(F("SAVED"));

  display.setFont();
  
  display.display();
  delay(500);
}

void Clear() {
  display.clearDisplay(); 
  
  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);
  display.setTextColor(WHITE);
  display.setCursor(17, 38);
  display.print(F("CLEARED"));

  display.setFont();
  
  display.display();
  delay(500);
}

void Snooze() {
  display.clearDisplay(); 
  
  display.setTextSize(1);
  display.setFont(&FyrusClockFontS2);
  display.setTextColor(WHITE);
  display.setCursor(33, 38);
  display.print(F("SNOOZE"));

  display.setFont();
  
  display.display();
  delay(500);
}


// Buzzer on (1) or off (0)
void Button_Sound(byte snd) {
  digitalWrite(Buzzer, snd);
}


// ---- Top Bar & Alarm Triggered Display ---- //
void Alarm_Notify(){

        display.clearDisplay();
        char buff[] = "hh:mm";

        display.setCursor(80, 0);
        display.setTextSize(1);
        display.print(F("Snooze >"));
        display.setCursor(0, 0);
        display.print(F(" Alarm!"));
        display.setCursor(80, 56);
        display.print(F(" Clear >"));

        display.setTextSize(1);
        display.setFont(&FyrusClockFontL);
        display.setCursor(17, 42);
        DateTime now = rtc.now();
        display.print(now.toString(buff));
        blink_cursor = !blink_cursor;
          if (blink_cursor == true) {
            display.fillRect(15, 17, 97, 30, BLACK);
          }

        display.setFont();
}

void ALARM1(){
        DateTime alarmTime = rtc.getAlarm1();

        display.setCursor(2, 0);
        display.setTextSize(1);
        int hour12_ = alarmTime.hour();
        bool isPM_  = false;
        
        if (hour12_ >= 12) {
              isPM_  = true;
        if (hour12_ > 12) {
            hour12_ -= 12;
            }
        } else if (hour12_ == 0) {
            hour12_ = 12;
        }
        if (hour12_ < 10) {
          display.print(F("0"));
        }
          display.print(hour12_, DEC);
          display.print(F(":"));
          
        if (alarmTime.minute() < 10) {
          display.print(F("0"));
        }
          display.print(alarmTime.minute(), DEC);
          display.setCursor(34, 0);
          display.print(isPM_ ? F("PM") : F("AM"));
}

void ALARM2(){
        DateTime alarmTime = rtc.getAlarm2();

        display.setCursor(82, 0);
        display.setTextSize(1);
        int hour12_ = alarmTime.hour();
        bool isPM_  = false;
        
        if (hour12_ >= 12) {
              isPM_  = true;
        if (hour12_ > 12) {
            hour12_ -= 12;
            }
        } else if (hour12_ == 0) {
            hour12_ = 12;
        }
        if (hour12_ < 10) {
          display.print(F("0"));
        }
          display.print(hour12_, DEC);
          display.print(F(":"));
          
        if (alarmTime.minute() < 10) {
          display.print(F("0"));
        }
          display.print(alarmTime.minute(), DEC);
          display.setCursor(115, 0);
          display.print(isPM_ ? F("PM") : F("AM"));
}
