# Fyrus Digital Alarm Clock
* 12 Hour Clock Display [ AM / PM ]
* Edit Alarm with button (2 Editable Alarm)
* Edit Time, Date with button
* Snooze Alarm for 5 minutes
* Low Power sleep mode
* DS3231 Interrupt (SQW pin)
* Save Alarm to DS3231 AT24C32 Memory
* 
* The Clock use LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF).
* so it's only can wakw when interrupt get's LOW om INT0 & INT1 (pin D2 & D3 on Pro Mini / Nano).
* in Sleep Mode, the display is turn Off (blank), without Alarm triggered LOW on pin 2, use Button 1 which attach to pin 3 to wake the display.
* I made this clock for portable use powered by Lipo 3.7v. Power saving is a must.

# Library
* RTClib    - https://github.com/adafruit/RTClib
* Low Power - https://github.com/rocketscream/Low-Power
* TimerOne  - https://github.com/PaulStoffregen/TimerOne
* Adafruit  - https://github.com/adafruit/Adafruit_SSD1306

# What Tou Need?
1. DS3231 RTC
2. Arduino Pro Mini or Arduino Nano (ATmega328P 3.3v or 5v).
3. Oled Display 0'96 SSD1306.
4. 2 Button.
5. Passive Buzzer.
6. On/Off Switch.
7. (Optional) LiPo 3.7v Battery (or AA Battery).
8. (Optional) TP4056 Module to charge the LiPo.

ALSO, the 2 Fonts provided, paste it inside Adafruit GFX library folder.
* ( Adafuirt_GFX_Library / Fonts / HERE ).

# What to connect where?
 1. connect display and DS3231 to I2C pin as usual.
 2. connect SQW pin from DS3231 to pin 2 of Arduino.
 3. connect button 1 to pin 3 (button interrupt), connect button 2 to pin 4.
 4. connect Buzzer to pin 5.

# HOW TO USE
* Press Button 1 & Button 2 simultaneously to enter Menu.
* Press Button 1 to wake from Low Power Sleep Mode (turn display ON).
* Press Button 1 to STOP Alarm when it's fired..
* Press Button 2 to SNOOZE Alarm when it's fired.

# Schematic (Wiring Diagram)
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Schematic.jpg)
