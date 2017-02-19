/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
 * Add DHT22 (environment temperature)
 * Compile modules only when enabled
 */

	#include "Configuration.cpp"

 	// I2C and LCD libs
 	#include <TinyWireM.h>
	#include <LiquidCrystal_I2C.h>

	// One analog input, a lot of switches :P
	#include <OneWireSwitches.h>
	
	// Modules
	#include "Module_RTC.h"
	#include "Module_Temperature.h"

	void setup()
	{
		// I2C interface and LCD init

		LiquidCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
		LCD.init();

		#ifdef LCD_BACKLIGHT_ON
			LCD.backlight();
		#else
			LCD.noBacklight();
		#endif

		// OneWireSwitches

		const uint32_t SwitchesR1[SWITCHES_AMOUNT] SWITCHES_R1;

		OneWireSwitches Switches(SWITCHES_INPUT_PIN, SWITCHES_AMOUNT, SwitchesR1, SWITCHES_R2, SWITCHES_RESISTOR_TOLERANCE);

		// Modules

		//RTCAlarm M_RTC(&LCD, &Switches);
		TemperatureAlarm M_Temperature(&LCD, &Switches);

		//M_RTC.init();
		M_Temperature.init();

		while(1)
		{
			//M_RTC.loop();
			M_Temperature.loop();
		}
	}

	void loop()
	{}