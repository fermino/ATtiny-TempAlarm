#ifndef TEMPALARM_CONFIGURATION_h
#define TEMPALARM_CONFIGURATION_h	
	#include <inttypes.h>

	/**
	 * Module: Temperature
	 * ===================
	 * 
	 * This module is a temperature-triggered alarm, it can alert you
	 * when something reachs the given temperature. 
	 * 
	 * The sensor is a DS18B20, but the DS18S20 and the DS1820 should work too. 
	 * It is connected through a OneWire bus. 
	 */
		
		// Between -127 and 127 (as Threshold is an 8-bit integer)
		#define TEMPERATURE_HIGHEST_THRESHOLD 127
		#define TEMPERATURE_LOWEST_THRESHOLD -20
		// Default threshold if the one stored in the EEPROM is not valid
		#define TEMPERATURE_DEFAULT_THRESHOLD 20

	/**
	 * LCD configuration
	 */

		#define TEMPERATURE_DEGREE_CHAR ((char) 223)

		#define TEMPERATURE_ALARM_ENABLED_0_CHAR '!'
		#define TEMPERATURE_ALARM_ENABLED_1_CHAR ((char) 255) // A character full of black dots C:
		
		#define TEMPERATURE_ALARM_DISABLED_CHAR '-'
	
		#define TEMPERATURE_DIRECTION_UP_CHAR_INDEX 0
		const uint8_t Temperature_DirectionUpChar[8]
		{
			0b00100,
			0b01110,
			0b11111,
			0b11111,
			0b00100,
			0b00100,
			0b00100,
			0b00100
		};

		#define TEMPERATURE_DIRECTION_DOWN_CHAR_INDEX 1
		const uint8_t Temperature_DirectionDownChar[8]
		{
			0b00100,
			0b00100,
			0b00100,
			0b00100,
			0b11111,
			0b11111,
			0b01110,
			0b00100
		};

		// Some characters blink on the screen on certain situations
		#define TEMPERATURE_STATUS_UPDATE_DELAY 500

	/**
	 * OneWireSwitches configuration
	 */

		#define TEMPERATURE_SWITCHES_AMOUNT 3

		#define TEMPERATURE_SWITCHES_R1 220000 /* 220k */, 68000 /* 68k */, 47000 /* 47k */

		#define TEMPERATURE_BUTTON_MINUS_ID 0
		#define TEMPERATURE_BUTTON_MINUS_THRESHOLD 25
		#define TEMPERATURE_BUTTON_MINUS_DELAY 50

		#define TEMPERATURE_BUTTON_PLUS_ID 1
		#define TEMPERATURE_BUTTON_PLUS_THRESHOLD 25
		#define TEMPERATURE_BUTTON_PLUS_DELAY 50

		#define TEMPERATURE_BUTTON_STARTSTOPDIRECTION_ID 2

		#define TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD 25
		#define TEMPERATURE_BUTTON_STARTSTOP_DELAY 500

		#define TEMPERATURE_BUTTON_DIRECTION_THRESHOLD 500
		#define TEMPERATURE_BUTTON_DIRECTION_DELAY 500
	/**
	 * EEPROM
	 * 
	 * The EEPROM can store information for a loong time. 
	 * Here, we define in which direction will we store the 
	 * last used temperature and alarm direction. 
	 */

		#define TEMPERATURE_EEPROM_ADDRESS_THRESHOLD 0
		#define TEMPERATURE_EEPROM_ADDRESS_DIRECTION 1

	/**
	 * DS18B20 / DS18S20 / DS1820
	 */

		// It disturbs the SPI signals when programming if tied to some SPI pin
		#define TEMPERATURE_ONE_WIRE_BUS 3

		// The temperature conversion takes about 750ms, so, I think this is pretty reasonable
		#define TEMPERATURE_CONVERSION_TIME 775
#endif