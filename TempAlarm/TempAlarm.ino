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
	#include "Module_Timer.h"

	static const uint32_t SwitchesR1[SWITCHES_AMOUNT] SWITCHES_R1;

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

		OneWireSwitches<SWITCHES_AMOUNT> Switches(SWITCHES_INPUT_PIN, SwitchesR1, SWITCHES_R2, SWITCHES_READ_TOLERANCE);

		// Buzzer output
		pinMode(BUZZER_PIN, OUTPUT);

		// Modules

		RTCAlarm M_RTC(&LCD, &Switches);
		TemperatureAlarm M_Temperature(&LCD, &Switches);
		TimerAlarm M_Timer(&LCD, &Switches);

		M_RTC.init();
		M_Temperature.init();
		M_Timer.init();

		for(;;)
		{
			M_RTC.loop();
			M_Temperature.loop();
			M_Timer.loop();

			changeAlarmStatus(/*M_RTC.isAlarmOn() ||*/ M_Temperature.isAlarmOn() || M_Timer.isAlarmOn());
		}
	}

	void changeAlarmStatus(bool Enabled)
	{
		digitalWrite(BUZZER_PIN, Enabled);
	}

	void loop()
	{}