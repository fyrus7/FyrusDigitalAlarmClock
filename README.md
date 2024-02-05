# About
- I search online but looks like no one is making an button editable alarm with DS3231 RTC that use the module memory. But averyone is using the MCU eeprom to save the alarm set.
- if the alarm is save into MCU memory, when sleep mode, there is no way to wake the clock without tapping on external interrupt button.
- when the alarm is save into MCU eeprom, DS3231 cannot wake the board with its Interrupt pin (SQW).
- the only way to make the Interrupt pin usable is to save the alarm time into DS3231 memory.
- 
- So, here it is...I just made an alarm clock with interrupt enable so I can turn the oled display off and turn it on only when i want to see time or when the sets Alarm is triggered :)

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
- MCP1700 Chip (optional)

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
- Button 1 Stop and Clear Alarm until next 24 hour
- Button 2 Stop and Remove Alarm settings
- DO NOTHING / alarm auto stop after 5 minute of non stop beeping.


# Schematic (Wiring Diagram)
![](https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Schematic_.jpg)

# Module Mod
these are modification I made on DS3231 RTC, remove LED because it's useless anyway, remove the charging resistor to use normal CR2032 coin cell battery. If your module come with a rechargeble LIR2032 you dont need to remove charging resistor.

I also remove the power led and on-board regulator and replace it by MCP1700 sot-23 for more power saving.

<img src="https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/DS3231%20MOD.jpg" width="48%"> <img src="https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Pro%20Mini%20%26%20MCP1700.jpg" width="50%">

Incase you wondering how I solder the small SMD chip, or you can buy adapter for that. Why I'm not using the sot-92? because it's not working by just soldered directly to the MCU, it need 1uF capacitor on Vin/Vout (based on datasheet). So I dont have the capacitor but I have 2 version of MCP1700 on hands.

<img src="https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/IMG20240124193754.jpg" width="45%"> <img src="https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/Sot23toDIP.jpg" width="45%">

After using Sot-23 to DIP adapter, I decided not to solder Vin to RAW pin because it's useless anyway. Battery positive goes to center pin on the adapter.

<img src="https://github.com/fyrus7/FyrusDigitalAlarmClock/blob/main/MCP1700_SOT23toDIP.jpg" width="45%">
