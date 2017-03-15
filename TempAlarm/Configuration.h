	#include "config/RTC.h"
	#include "config/Temperature.h"
	#include "config/Timer.h"

	/**
	 * TempAlarm v1.0
	 * ==============
	 * 
	 * __ ADD INFO
	 */

		// To do: PWM output?
		// Refer to the datasheet or tiny's pinout if you want to change it. 
		// PB1 is digital output 1
		#define BUZZER_DDR	DDRB
		#define BUZZER_PORT	PORTB
		#define BUZZER_BIT	PB1

	/**
	 * LCD
	 * 
	 * Pinout: 
	 * SDA <=> ATtiny's pin 5 (Digital IO #0)
	 * SCL <=> ATtiny's pin 7 (Digital IO #2)
	 * 
	 * Here you must put the LCD config and features, as the I2C address, columns
	 * and rows.
	 * 
	 * Also, in config/* files are defined some characters that are widely used
	 * in the firmware, you can change them if you want :)
	 */

		#define LCD_I2C_ADDRESS 0x27

		#define LCD_COLUMNS 20
		#define LCD_ROWS 4

		// Comment to disable+
		#define LCD_BACKLIGHT_ON

	/**
	 * OneWireSwitches configuration
	 * This lib allows us to read a lot of switches using only one analog input. 
	 * It uses a simple voltage divider multiplexed with every switch. 
	 * 
	 * Then: 
	 * 1. Put a resistor (R2) between the analog pin and GND. 
	 * 2. Put a resistor (R1s) between the output of every
	 *    switch and the analog pin. 
	 *    The other side of the switch should be tied to VCC. 
	 * 3. Configure it all here, including the resistor tolerance, 
	 *    multiplied by 10 (usually 50 (5%))
	 * 
	 * The library will make the calculations and now you can read all those
	 * switches by using readKey(R1Index). R1 index is the number of resistor
	 * configured here, starting off by 0. 
	 * 
	 * See https://learn.sparkfun.com/tutorials/voltage-dividers if you want
	 * to understand how this all works!
	 */

		#define SWITCHES_INPUT_PIN A2

		#define SWITCHES_AMOUNT 9

		// Values, over 10bit reading (102.4 => 10% over total reading)
		#define SWITCHES_READ_TOLERANCE 25

		#define SWITCHES_R1 {220000 /* 220k */, 68000 /* 68k */, 47000 /* 47k */, 33000 /* 33k */, 22000 /* 22k */, 18000 /* 18k */, 12000 /* 12k */, 6800 /* 6k8 */, 0 /* 0 ohms, yeah, just a cable :P */}
		#define SWITCHES_R2 20000 // 20k