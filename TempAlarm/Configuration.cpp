	#include <inttypes.h>

	/**
	 * BEGIN CONFIG
	 * 
	 * To do: 
	 * Add some defines, as the time between sensor's reads
	 */

	/**
	 * LCD
	 * 
	 * Pinout: 
	 * SDA <=> ATtiny's pin 5 (Digital IO #0)
	 * SCL <=> ATtiny's pin 7 (Digital IO #2)
	 * 
	 * Here you must put the LCD config and features, as the I2C address, columns
	 * and rows. If it have more than 2 rows, the Timer module will be compiled. 
	 * 
	 * Also, here are defined some characters that are used widely in the firmware, 
	 * you can change them if you want :)
	 * 
	 * The alarm's direction means if the sensor should be heated or cooled for
	 * the alarm to be activated. 
	 */

		#define LCD_I2C_ADDRESS 0x27

		#define LCD_COLUMNS 20
		#define LCD_ROWS 4

		// Comment it to disable
		#define LCD_BACKLIGHT_ON

		#define LCD_ALARM_ENABLED_0 '!'
		#define LCD_ALARM_ENABLED_1 (char) 255 // a character full of black dots C:

		#define LCD_ALARM_DISABLED '-'

		#define LCD_TEMP_ALARM_DIRECTION_0 '>'
		#define LCD_TEMP_ALARM_DIRECTION_1 '<'

		#define LCD_DEGREE_CHAR (char) 223

		#define LCD_TIMER_STOPWATCH_CLOCK_CHAR_INDEX 0
		const uint8_t LCD_Timer_StopwatchClockChar[]
		{
			0b01110,
			0b00100,
			0b01110,
			0b10011,
			0b10101,
			0b10001,
			0b01110,
			0b00000
		};

		#define LCD_TIMER_COUNTDOWN_CLOCK_CHAR_INDEX 1
		const uint8_t LCD_Timer_CountdownClockChar[]
		{
			0b11111,
			0b10001,
			0b01010,
			0b00100,
			0b01010,
			0b10001,
			0b11111,
			0b00000
		};

		#define LCD_TIMER_ENABLED_CHAR ' '
		#define LCD_TIMER_FINISHED_CHAR '!'

	/**
	 * Switches
	 * 
	 * ID: The index of the resistor configured in OneWireSwitches, which
	 * is tied to the switch we want to control. 
	 * THRESHOLD: The shortest pulse detected as a press by the firmware. 
	 * DELAY: Time (ms) to wait after the press without reading any button. 
	 */

		#define TEMP_CHANGE_DIR_BUTTON_ID 0
		#define TEMP_CHANGE_DIR_BUTTON_THRESHOLD 25
		#define TEMP_CHANGE_DIR_BUTTON_DELAY 500
		
		#define TEMP_MINUS_BUTTON_ID 1
		#define TEMP_PLUS_BUTTON_ID 2
		#define TEMP_BUTTON_THRESHOLD 25
		#define TEMP_BUTTON_DELAY 50

		#define TEMP_START_STOP_BUTTON_ID 3
		#define TEMP_START_STOP_BUTTON_THRESHOLD 25
		#define TEMP_START_STOP_BUTTON_DELAY 500

		#define TIMER_ADD_HOUR_BUTTON_ID 4
		#define TIMER_ADD_MINUTE_BUTTON_ID 5
		#define TIMER_ADD_SECOND_BUTTON_ID 6
		#define TIMER_ADD_BUTTON_THRESHOLD 30
		#define TIMER_ADD_BUTTON_DELAY 30

		// This switch has multiple functions
		#define TIMER_CONTROL_BUTTON_ID 7

		#define TIMER_CONTROL_RESET_BUTTON_THRESHOLD 500
		#define TIMER_CONTROL_RESET_BUTTON_DELAY 500

		#define TIMER_CONTROL_SWITCH_BUTTON_THRESHOLD 250
		#define TIMER_CONTROL_SWITCH_BUTTON_DELAY 250

		#define TIMER_CONTROL_START_STOP_BUTTON_THRESHOLD 50
		#define TIMER_CONTROL_START_STOP_BUTTON_DELAY 50

		// This must match the previous settings
		#define TIMER_CONTROL_BUTTON_MAX_THRESHOLD 500

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

		#define SWITCHES_AMOUNT 8

		#define SWITCHES_RESISTOR_TOLERANCE 50

		#define SWITCHES_R2 20000 // 20k

		const uint32_t SwitchesR1[SWITCHES_AMOUNT]
		{
			100000,	// 100k
			68000,	// 68k
			33000,	// 33k
			20000,	// 20k
			10000,	// 10k
			6800,	// 6k8
			3300,	// 3k3
			0		// 0 ohms (switch output tied directly to the input)
		};

	/**
	 * EEPROM
	 * 
	 * The EEPROM can store information for a loong time. 
	 * Here, we define in which direction will we store the 
	 * last used temperature and alarm direction. 
	 */

		#define EEPROM_ADDRESS_TEMP_THRESHOLD 0
		#define EEPROM_ADDRESS_TEMP_DIRECTION 1

	/**
	 * Alarm
	 * 
	 * Here is the configuration of the alarm in itself. 
	 */

		// How many time the info screen will be showed at startup
		#define LCD_AFTER_WELCOME_DELAY 500

		// To do: PWM output
		#define BUZZER_PIN 1

	/**
	 * MODULE: Temperature
	 * 
	 * This module is a temperature-triggered alarm, it can alert you
	 * when something reachs the given temperature. 
	 * 
	 * The sensor is a DS18B20, but the DS18S20 and the DS1820 should work too. 
	 * It is connected through a OneWire bus. 
	 */

		// It disturbs the SPI signals when programming if tied to some SPI pin
		#define ONE_WIRE_BUS 3

		// Between -127 and 127, as TemperatureAlarmThreshold is an 8-bit integer
		#define TEMP_HIGHEST_THRESHOLD 100
		#define TEMP_LOWEST_THRESHOLD -20

		// Default at startup if the value stored in the EEPROM is not valid
		// Also between -127 and 127
		#define TEMP_DEFAULT_THRESHOLD 25

		// Some characters blink on the screen on certain situations. 
		// What should be the delay between everyone of them?
		#define BLINKING_CHARACTER_DELAY 500

	/**
	 * MODULE: Timer
	 * 
	 * 
	 */

		// This will be modified by software according to the available space on the LCD
		#define MAX_TIMERS 2

		// The time limit for a countdown timer, in seconds: 99:59:59 => 359999
		#define TIMER_LIMIT 359999

	/**
	 * MODULE: RTC
	 * 
	 * 
	 */

		//#define RTC_HW DS3231
		#define RTC_I2C_ADDRESS 0x68

	/**
	 * END CONFIG
	 */