# ATtiny-TempAlarm
A programable temperature alarm with ATtiny85, DS18B20 temperature sensor and Grove Serial LCD

## Fuses

Low fuse: 0xf1  
High fuse: 0xd4  
Extended fuse: 0xff  

Avrdude arguments: -U lfuse:w:0xf1:m -U hfuse:w:0xd4:m -U efuse:w:0xff:m

## Known bugs
If the sensor is working with parasite power, the VCC pin should be tied to ground, but, because the Greek Gods hate me, it returns 85 °C (that seems to be an error code). 
Maybe we are scheduling more conversions that the module can handle when working with parasite power. I remember some datasheet saying that while converting in parasite power mode, no communication should be made. 

https://forums.adafruit.com/viewtopic.php?f=19&t=34569#p171152
