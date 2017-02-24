# ATtiny-TempAlarm
A programable temperature alarm with ATtiny85, DS18B20 temperature sensor and an I2C Liquid Crystal Display (20x4)

## Hardware

I made a schematic and a PCB of the project using DipTrace; you can find it inside hardware/.

The LCD and RTC is connected to the I2C USI port (SDA <=> Pin 5, SCL <=> Pin 7).  
The temperature sensor (DS18B20, OneWire; DS18S20 and DS1820 should work too) is connected in the digital i/o #3 (Pin 2).  
The buzzer is connected to digital output #1 (Pin 6).  
The input used by OneWireSwitches lib is A2 (Pin 3).  


The DS18B20 can ork in two different modes: 

* Parasite mode (2 lines)
	VCC and GND in the sensor tied to GND, data line tied to MCU pin, and pulled up via a 4k7 resistor. 
	This mode is not recommended when you're working with high temperatures (such as boiling water :P)

* Non parasite (3 lines)
	VCC tied to VCC, GND to GND, data line same as above (WITH the resistor). 

## Fuses

The LCD with the ATtiny running at 8MHz works very well. 
This fuses enable preserve EEPROM (avoid clearing up on every code flash) and bronout detection at 2.7V (can be powered by a LiPo battery, assuming your display works with theese voltages). 

Low fuse: 0xe2  
High fuse: 0xd5  
Extended fuse: 0xff  

Avrdude arguments: -U lfuse:w:0xe2:m -U hfuse:w:0xd5:m -U efuse:w:0xff:m

## Libraries

The Arduino IDE used to compile the project is 1.6.13. 

Every required library is in libraries/. 

## Datasheets

Like the libraries, the datasheets are inside datasheets/

## Known bugs

None AFAIK :)
