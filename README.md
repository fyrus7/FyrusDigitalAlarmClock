# Fyrus Digital Alarm Clock
* 12 Hour Clock Display [ AM / PM ]
* Edit Alarm with button (2 Editable Alarm)
* Edit Time, Date with button
* Low Power sleep mode
* DS3231 Interrupt (SQW pin)
* Save Alarm to DS3231 AT24C32 Memory
* 
* The Clock use LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF).
* so it's only can wake when interrupt get's LOW om INT0 & INT1 (pin D2 & D3 on Pro Mini / Nano).
* in Sleep Mode, the display is turn Off (blank).
* I made this clock for portable use powered by Lipo 3.7v. Power saving is a must.

# Library
* RTClib    - https://github.com/adafruit/RTClib
* Low Power - https://github.com/rocketscream/Low-Power
* TimerOne  - https://github.com/PaulStoffregen/TimerOne
* Adafruit  - https://github.com/adafruit/Adafruit_SSD1306

# What Tou Need?
- Arduino Pro Mini 3.3v 8Mhz
- DS3231 RTC
- 128x64 0'96 Oled Display SSD1306
- Active Buzzer
- 2 mini Button
- Small On/Off Switch
- 3.7v Lithium Battery
- TP4056 Battery Charger
- Small Electrical Box

ALSO, the 2 Fonts provided, paste it inside Adafruit GFX library folder.
- ( Adafuirt_GFX_Library / Fonts / HERE ).

# What to connect where?
- connect display and DS3231 to I2C pin as usual.
- connect SQW pin from DS3231 to pin 2 of Arduino.
- connect button 1 to pin 3 (button interrupt), connect button 2 to pin 4.
- connect Buzzer to pin 5.

# HOW TO USE
2 Button operation:
- Button 1 to select
- Button 2 to scroll down
- Press both Button to enter Menu

Alarm Triggered condition:
- Button 1 to Clear   / trigger again after 24 hours
- Button 2 to Disable / remove alarm from triggering again


# Schematic (Wiring Diagram)
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Schematic.jpg)

# Module Mod
these are modification I made on DS3231 RTC, remove LED because it's useless anyway, remove the charging resistor to use normal CR2032 coin cell battery. If your module come with a rechargeble LIR2032 you dont need to remove charging resistor.
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/DS3231%20MOD.jpg)
