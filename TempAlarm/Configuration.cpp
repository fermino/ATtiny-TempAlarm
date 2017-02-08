	#include <inttypes.h>

	/**
	 * BEGIN CONFIG
	 */

	// Project's pinout

		/** LCD pinout
		 * SDA <=> ATtiny's pin 5 (Digital IO #0)
		 * SCL <=> ATtiny's pin 7 (Digital IO #2)
		 */

		// An active buzzer
		#define BUZZER_PIN 1

		// Data pin of temperature sensor
		// Disturbs the SPI signals while programming if connected to pin 2 (or other SPI pin) 
		#define ONE_WIRE_BUS 3

		// OneWireSwitches input pin
		#define SWITCHES_INPUT_PIN A2

	// OneWireSwitches configuration

		// This works by dividing voltages on only one analog input
		// See https://learn.sparkfun.com/tutorials/voltage-dividers

		// How many switches do we have?
		#define SWITCHES_AMOUNT 8

		// Define array indexes to make the code prettier :P
		#define CHANGE_DIR_ID 0
		#define TEMP_MINUS_ID 1
		#define TEMP_PLUS_ID 2
		#define START_STOP_ID 3

		#define CHANGE_START_STOP_TIMER_ID 4
		#define ADD_HOURS_ID 5
		#define ADD_MINUTES_ID 6
		#define ADD_SECONDS_ID 7

		// What is the resistor tolerance?
		// %, multiplied by 10, 5% should be 50
		#define SWITCHES_RESISTOR_TOLERANCE 50

		// Value of R2 in the voltage divider, the resistor tied from SWITCHES_INPUT_PIN to ground
		#define SWITCHES_R2 20000 // 20k

		// R1 values, resistors tied from SWITCHES_INPUT_PIN to every pushbutton
		// The button input then should be tied to VCC
		const uint32_t SwitchesR1[SWITCHES_AMOUNT]
		{
			100000,	// 100k
			68000,	// 68k
			33000,	// 33k
			20000,	// 20k
			10000,	// 10k
			6800,	// 6k8
			3300,	// 3k3
			0		// 0 ohms (switch tied to VCC without any resistor)
		};

	// Interface delays

		// How many time the info screen will be showed at startup
		#define AFTER_WELCOME_DELAY 500

		// Time to wait after every Temp++/-- button press WITHOUT READING
		#define TEMP_BUTTON_DELAY 50

		// Time to wait after every Start/Stop button press WITHOUT READING
		// This will avoid multiple presses with only one click
		#define START_STOP_BUTTON_DELAY 500
		
		// Time to wait after every ChangeDir button press WITHOUT READING
		// This will avoid multiple presses with only one click
		#define CHANGE_DIR_BUTTON_DELAY 500

		// How many time should be wait between every blinking char in the screen
		#define BLINKING_CHARACTER_DELAY 500

	// Button thresholds

		// How many time should the user press Temp++/-- in order to be detected as a press?
		#define TEMP_BUTTON_THRESHOLD 25

		// The same, with Start/Stop button
		#define START_STOP_BUTTON_THRESHOLD 25

		// Yeah, the same, you should know what button we're talking about..
		// OKAY, RIGHT, I'M TIRED OF USING CTRL+C/CTRL+V
		#define CHANGE_DIR_BUTTON_THRESHOLD 25

	// Temperature limits and default data

		// Temperature limits in the alarm config
		// Should be between -127 and 127, as AlarmTemperature is an 8-bit integer
		#define HIGHEST_TEMPERATURE 100
		#define LOWEST_TEMPERATURE -20

		// Default AlarmTemperature at startup if the value stored in the EEPROM is not valid
		// Also between -127 and 127
		#define START_TEMPERATURE 25

	// LCD configuration

		#define LCD_I2C_ADDRESS 0x27 // The address of the I2C Port Expander

		#define LCD_COLUMNS 20
		#define LCD_ROWS 4

		#define LCD_ALARM_ENABLED_0 '!'
		#define LCD_ALARM_ENABLED_1 (char) 255 // a character full of black dots C:

		#define LCD_ALARM_DISABLED '-'

		#define LCD_ALARM_DIRECTION_0 '>' // >
		#define LCD_ALARM_DIRECTION_1 '<' // <

		#define LCD_DEGREE_CHAR (char) 223

	// Timer configuration

		// The first 2 LCD rows are used by the temperature alarm
		#define TIMER_AMOUNT LCD_ROWS - 2

		// Timer limit in seconds
		#define TIMER_LIMIT 359999 // 99 hours, 59 minutes, 59 seconds

	// Other config

		// Comment it to disable
		#define LCD_BACKLIGHT_ON

		// Where is stored the last temperature?
		#define EEPROM_TEMPERATURE_ADDRESS 0

		// Where is stored the last alarm direction?
		#define EEPROM_DIRECTION_ADDRESS 1

	/**
	 * END CONFIG
	 */