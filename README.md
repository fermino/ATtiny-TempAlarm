# ATtiny-TempAlarm
A programable temperature alarm with ATtiny85, DS18B20 temperature sensor and Grove Serial LCD

## Hardware

I made a schematic and a PCB of the project using DipTrace; you can find it inside hardware/.

## Fuses

The Grove Serial LCD only works with the MCU running at 16MHz, so, the fuses are: 

Low fuse: 0xf1  
High fuse: 0xd4  
Extended fuse: 0xff  

Avrdude arguments: -U lfuse:w:0xf1:m -U hfuse:w:0xd4:m -U efuse:w:0xff:m

## Libraries

The Arduino IDE used to compile the project is 1.6.13. 

Every required library is in libraries/, even SoftwareSerial, which is shipped with the Arduino IDE by default. 

## Datasheets

Like the libraries, the datasheets are inside datasheets/

## Known bugs
If the sensor is working with parasite power, the VCC pin should be tied to ground, but, because the Greek Gods hate me, it returns 85 °C (that seems to be an error code). 
Maybe we are scheduling more conversions that the module can handle when working with parasite power. I remember some datasheet saying that while converting in parasite power mode, no communication should be made. 

https://forums.adafruit.com/viewtopic.php?f=19&t=34569#p171152
