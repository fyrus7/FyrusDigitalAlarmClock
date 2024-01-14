# Fyrus Digital Alarm Clock
* 12 Hour Clock Display
* Edit Alarm with button (2 Editable Alarm)
* Edit Time, Date with button
* Low Power sleep mode ([Low Power Library](https://github.com/rocketscream/Low-Power))
* SQW Interrupt
* Save Alarm to DS3231 AT24C32 Memory ([RTClib](https://github.com/adafruit/RTClib))
*  
* use interrupt pin when alarm trigered so the Arduino can use Low Power Sleep mode to save energy and save the oled from continuosly turn on. I made it to for portable use powered by Lipo 3.7v with TP4056 rechargeable modul.
* DS3231 SQW pin has to be connected to Arduino interrupt capable pin (D2 or D3 on Pro Mini/Nano). Without these pin alarm will not triggered.

# What You Need?
1. DS3231 RTC
2. Arduino Pro Mini or Arduino Nano (ATmega328P 3.3v or 5v)
3. SSD1306 0'96 Oled Display
4. 2 buttons
5. Passive Buzzer
6. On/Off Switch
7. the 2 Custom Fonts provided ( paste it inside Adafruit_GFX_Library/FOnts )

8. (Optional) Lipo 3.7v Battery (can use 2 AA battery with a holder)
9. (Optional) TP4056 Charging modul (if you use rechargeable battery)

# What to connect where?
 1. connect display and DS3231 to I2C pin as usual.
 2. connect SQW pin from DS3231 to pin 2 of Arduino.
 3. connect button 1 to pin 3 (button interrupt), connect button 2 to pin 4.
 4. connect Buzzer to pin 5. 

# HOW TO USE
* Press Button 1 & Button 2 simultaneously to enter Menu.
* Press Button 1 to wake from Low Power Sleep Mode (turn display ON).
* Press any button to stop alarm.

# Schematic (Wiring Diagram)
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Schematic.jpg)
