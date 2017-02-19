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

	/**
	 * OneWireSwitches configuration
	 */

		#define TEMPERATURE_SWITCHES_AMOUNT 3

		#define TEMPERATURE_SWITCHES_R1 \
			220000,	// 220k	\
			68000,	// 68k	\
			47000	// 47k	

		#define TEMPERATURE_BUTTONID_MINUS 0
		#define TEMPERATURE_BUTTONID_PLUS 1
		#define TEMPERATURE_BUTTONID_STARTSTOP 2

	/**
	 * EEPROM
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