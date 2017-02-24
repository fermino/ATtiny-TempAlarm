#ifndef TempAlarm_Timer_Configuration_h
#define TempAlarm_Timer_Configuration_h
	#include <inttypes.h>

	/**
	 * Module: Timer
	 * ===================
	 * 
	 */

		// How many timers? (be carefull with LCD's remaining space!)
		#define TIMER_AMOUNT 2

		// 99 hours, 59 minutes, 59 seconds
		#define TIMER_TIME_LIMIT 359999

	/**
	 * LCD configuration
	 */

		#define TIMER_SELECTED_TIMER_CHAR '>'

		// This two chars will blink alternating with the mode character (below)
		#define TIMER_ENABLED_CHAR ' '
		#define TIMER_FINISHED_CHAR '!'

		#define TIMER_STOPWATCH_CHAR_INDEX 2
		const uint8_t Timer_StopwatchChar[]
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
	
		#define TIMER_COUNTDOWN_CHAR_INDEX 3
		const uint8_t Timer_CountdownChar[]
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

		// Some characters blink on the screen on certain situations
		#define TIMER_STATUS_UPDATE_DELAY 500

	/**
	 * OneWireSwitches configuration
	 * 
	 * ID: The index of the resistor configured in OneWireSwitches, which
	 * is tied to the switch we want to control. 
	 * THRESHOLD: The shortest pulse detected as a press by the firmware. 
	 * DELAY: Time (ms) to wait after the press without reading any button. 
	 */

		#define TIMER_BUTTON_ADD_HOUR_ID 3
		#define TIMER_BUTTON_ADD_MINUTE_ID 4
		#define TIMER_BUTTON_ADD_SECOND_ID 5

		#define TIMER_BUTTON_ADD_THRESHOLD 50


		#define TIMER_BUTTON_CONTROL_ID 6

		#define TIMER_BUTTON_CONTROL_RESET_THRESHOLD 500
		#define TIMER_BUTTON_CONTROL_RESET_DELAY 500

		#define TIMER_BUTTON_CONTROL_SWITCH_THRESHOLD 250
		#define TIMER_BUTTON_CONTROL_SWITCH_DELAY 250

		#define TIMER_BUTTON_CONTROL_STARTSTOP_THRESHOLD 50
		#define TIMER_BUTTON_CONTROL_STARTSTOP_DELAY 50

#endif