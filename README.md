# Fyrus Digital Alarm Clock
* 12 Hour Clock Display
* 2 Editable Alarm
* Edit Time, Date with button
* Low Power sleep mode
* SQW Interrupt
* use DS3231 AT24C32 Memory to save Alarm (not arduino eeprom)
*
*  
* use interrupt pin when alarm trigered so the Arduino can use Low Power Sleep mode to save energy and save the oled from continuosly turn on. I made it to for portable use powered by Lipo 3.7v with TP4056 rechargeable modul.
* DS3231 SQW pin has to be connected to Arduino interrupt capable pin (D2 or D3 on Pro Mini/Nano). Without these pin alarm will not triggered.

# What You Need?
1. DS3231 RTC
2. Arduino Pro Mini or Nano
3. SSD1306 0'96 Oled Display
4. 2 buttons
5. Passive Buzzer
6. the 2 Custom Fonts provided ( paste it inside Adafruit_GFX_Library/FOnts )

7. (Optional) Lipo 3.7v Battery (can use 2 AA battery with a holder)
8. (Optional) TP4056 Charging modul (if you use rechargeable battery)

# What to connect where?
 1. connect display and DS3231 to I2C pin as usual.
 2. connect SQW pin from DS3231 to pin 2 of Arduino.
 3. connect button 1 to pin 3 (button interrupt), connect button 2 to pin 4.
 4. connect Buzzer to pin 5. 
