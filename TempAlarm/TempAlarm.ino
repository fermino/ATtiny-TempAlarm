/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
 * Add DHT22 (environment temperature)
 * Compile modules only when enabled
 */
	
	#ifndef cbi
		#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
	#endif
	#ifndef sbi
		#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
	#endif

	#include <avr/io.h>

	#include "Configuration.h"

 	// I2C and LCD libs
 	#include <TinyWireM.h>
	#include <LiquidCrystal_I2C.h>

	// One analog input, a lot of switches :P
	#include <OneWireSwitches.h>
	
	// Modules
	#include "Module_RTC.h"
	#include "Module_Temperature.h"
	#include "Module_Timer.h"

	// LCD
	LiquidCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

	// OneWireSwitches
	static const uint32_t SwitchesR1[SWITCHES_AMOUNT] SWITCHES_R1;
	OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN> Switches(SwitchesR1, SWITCHES_R2, SWITCHES_READ_TOLERANCE);

	// Modules

	RTCAlarm M_RTC(&LCD, &Switches);
	TemperatureAlarm M_Temperature(&LCD, &Switches);
	TimerAlarm M_Timer(&LCD, &Switches);

	void setup()
	{
		// I2C interface and LCD init

		LCD.init();

		#ifdef LCD_BACKLIGHT_ON
			LCD.backlight();
		#else
			LCD.noBacklight();
		#endif

		// Set buzzer pin as output
		// pinMode(BUZZER_PIN, OUTPUT);
		sbi(BUZZER_DDR, BUZZER_BIT);

		// Init modules

		M_RTC.init();
		M_Temperature.init();
		M_Timer.init();

		for(;;)
		{
			M_RTC.loop();
			M_Temperature.loop();
			M_Timer.loop();

			setAlarmStatus(/*M_RTC.isAlarmOn() || */ M_Temperature.isAlarmOn() || M_Timer.isAlarmOn());
		}
	}

	void setAlarmStatus(bool Enabled)
	{
		// digitalWrite(BUZZER_PIN, Enabled);

		if(Enabled)
			sbi(BUZZER_PORT, BUZZER_BIT);
		else
			cbi(BUZZER_PORT, BUZZER_BIT);
	}

	void loop()
	{}