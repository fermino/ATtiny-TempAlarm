/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
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

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		/*if(ReadPulse(TEMP_PLUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD)
		{
			// If the user pressed Temp++

			// Limit temperature
			if(AlarmTemperature < HIGHEST_TEMPERATURE)
				AlarmTemperature++;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_MINUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD)
		{
			// If the user pressed Temp--

			// Limit temperature
			if(AlarmTemperature > LOWEST_TEMPERATURE)
				AlarmTemperature--;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(START_STOP_ID, START_STOP_BUTTON_THRESHOLD) >= START_STOP_BUTTON_THRESHOLD)
		{
			// If the user pressed Start/Stop

			if(AlarmEnabled)
			{
				// If the alarm is enabled we disable it and drive buzzer LOW (even if the alarm was not triggered)
				// Then, we write LCD_ALARM_DISABLED in the display, as alarm enable flag

				digitalWrite(BUZZER_PIN, LOW);

				ClearBlinkingCharacter();

				AlarmEnabled = false;
			}
			else
				AlarmEnabled = true; // If the alarm is not enabled, we'll make it happen

			// And, we store the temperature and the mode in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(EEPROM_TEMPERATURE_ADDRESS, AlarmTemperature);
			eeprom_update_byte(EEPROM_MODE_ADDRESS, AlarmReverse);

			delay(START_STOP_BUTTON_DELAY);
		}
		else if(ReadPulse(CHANGE_MODE_ID, CHANGE_MODE_BUTTON_THRESHOLD) >= CHANGE_MODE_BUTTON_THRESHOLD)
		{
			// If the user pressed ChangeMode

			AlarmReverse = !AlarmReverse;

			UpdateModeCharacter();

			delay(CHANGE_MODE_BUTTON_DELAY);
		}

		// Show AlarmTemperature

		LCD.setCursor(7, 0);
		LCD.print(AlarmTemperature);
		LCD.print(' ');

		// Show current temperature at sensor

		float Temperature = Sensors.getTempCByIndex(0);

		if(Temperature != DEVICE_DISCONNECTED_C)
		{
			LCD.setCursor(6, 1);
			LCD.print(Temperature, 1);
			LCD.print("  ");
		}
		else
		{
			// If the sensor gets disconnected, we'll print it

			LCD.clear();
			LCD.home();
			LCD.print("Sensor");
			LCD.setCursor(4, 1);
			LCD.print("disconnected");

			while(1);
		}

		// Do some alarm things (?

		if(AlarmEnabled)
		{
			// Write some status info in the LCD

			UpdateBlinkingCharacter();

			// If the threshold is obove or below the setted temperature, activate the alarm

			if(!AlarmReverse && Temperature >= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
			
			if(AlarmReverse && Temperature <= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
		}

		// Perform another reading

		Sensors.requestTemperatures();*/
	}

	// Reads a pulse from a button and returns the time since the press
	// Returns 0 if the button is not pressed
	uint16_t ReadPulse(uint8_t KeyIndex, uint16_t Timeout)
	{
		uint16_t Time = 0;

		while(Switches.readKey(KeyIndex) == HIGH && Time < Timeout)
		{
			Time++;
			delay(1);
		}

		return Time;
	}

	void UpdateModeCharacter()
	{
		LCD.setCursor(15, 1);
		LCD.print(AlarmReverse ? LCD_REVERSE_ALARM_0 : LCD_REVERSE_ALARM_1);
	}