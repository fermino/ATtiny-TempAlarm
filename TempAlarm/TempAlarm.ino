/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
 * Add DHT22 (environment temperature)
 */

	// EEPROM
	#include <avr/eeprom.h>

	// LCD
	#include <TinyWireM.h> // I2C lib
	#include <LiquidCrystal_I2C.h>

	// Using one analog input for read multiple pushbutons
	#include <OneWireSwitches.h>

	// Countdown timer library
	#include <KitchenTimer_Countdown.h>

	// Configuration file
	#include "Configuration.cpp"

	// Objects

		// LCD library
		LiquidCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

		// OneWireSwitches
		OneWireSwitches Switches(SWITCHES_INPUT_PIN, SWITCHES_AMOUNT, SwitchesR1, SWITCHES_R2, SWITCHES_RESISTOR_TOLERANCE);

		// Timers
		KitchenTimer_Countdown Timers[TIMER_AMOUNT];

	uint8_t SelectedTimer = 0;
	void setup()
	{
		// Configure Buzzer Pin
		pinMode(BUZZER_PIN, OUTPUT);

		// Init LCD

		delay(100);
		LCD.init();

		#ifdef LCD_BACKLIGHT_ON
			LCD.backlight();
		#else
			LCD.noBacklight();
		#endif

		// Print some info
		// Can be commented if you need more flash for improvements

		LCD.home();
		LCD.print(" TempAlarm v0.1");
		LCD.setCursor(0, 1);
		LCD.print("  By @fermino");

		delay(AFTER_WELCOME_DELAY);

		// Init Modules

		InitTemperature();
	}

	void loop()
	{
		TemperatureLoop();

		/*// Do some alarm things (?

		if(AlarmEnabled)
		{
			// Write some status info in the LCD

			UpdateBlinkingCharacter();

			// If the threshold is obove or below the setted temperature, activate the alarm

			if(!AlarmReverse && Temperature >= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
			
			if(AlarmReverse && Temperature <= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
		}*/
	}