/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
 * Add DHT22 (environment temperature)
 * Compile modules only when enabled
 */

	// LCD
	#include <TinyWireM.h> // I2C lib
	#include <LiquidCrystal_I2C.h>

	// Using one analog input for read multiple pushbutons
	#include <OneWireSwitches.h>

	// Configuration file
	#include "Configuration.cpp"

	// LCD library
	LiquidCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

	// OneWireSwitches
	OneWireSwitches Switches(SWITCHES_INPUT_PIN, SWITCHES_AMOUNT, SwitchesR1, SWITCHES_R2, SWITCHES_RESISTOR_TOLERANCE);

	// Including shared variables
	extern bool TemperatureAlarmOn;
	extern bool TimerAlarmOn;

	void setup()
	{
		// Configure Buzzer Pin
		pinMode(BUZZER_PIN, OUTPUT);

		// Init LCD

		LCD.init();

		#ifdef LCD_BACKLIGHT_ON
			LCD.backlight();
		#else
			LCD.noBacklight();
		#endif

		// Print some info
		// Can be commented if you need more flash for improvements

		/*LCD.home();
		LCD.print(" TempAlarm v0.1");
		LCD.setCursor(0, 1);
		LCD.print("  By @fermino");

		delay(LCD_AFTER_WELCOME_DELAY);*/

		// Init Modules

		InitTemperature();
		InitTimer();
	}

	void loop()
	{
		TemperatureLoop();
		TimerLoop();

		digitalWrite(BUZZER_PIN, TemperatureAlarmOn || TimerAlarmOn);
	}