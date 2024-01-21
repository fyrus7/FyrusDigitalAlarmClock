# Fyrus Digital Alarm Clock
* 12 Hour Clock Display [ AM / PM ]
* Edit Alarm with button (2 Editable Alarm)
* Edit Time, Date with button
* 5 Minute Snooze Time
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
- connect Display & DS3231 I2C ( VCC to VCC / GND to GND / SDA to A4 / SCL to A5 ) 
- connect Pin 2 to SQW pin  (D2 / Interrupt - INT0)
- connect Pin 3 to button 1 (D3 / Interrupt - INT1)
- connect Pin 4 to button 2
- connect Pin 5 to Buzzer positive.

# HOW TO USE
2 Button operation:
- Button 1 to select
- Button 2 to scroll down
- Press both Button to enter Menu

Alarm Triggered condition:
- Button 1 to Snooze / add 5 Minute snooze.
- Button 2 to Clear  / stop alarm until next 24 hours if not changing the alarm time.
- DO NOTHING         / alarm clear after 5 minute and stop until next 24 hour if not changing the alarm time.


# Schematic (Wiring Diagram)
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Schematic_.jpg)

# Module Mod
these are modification I made on DS3231 RTC, remove LED because it's useless anyway, remove the charging resistor to use normal CR2032 coin cell battery. If your module come with a rechargeble LIR2032 you dont need to remove charging resistor.
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/DS3231%20MOD.jpg)
