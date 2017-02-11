/**
 * Thanks for helping me to optimize the code (more then 1.3K flash saved!) to
 * 
 * https://edwardmallon.wordpress.com/2014/03/12/using-a-ds18b20-temp-sensor-without-a-dedicated-library/
 * http://bildr.org/2011/07/ds18b20-arduino/
 */

	// Using this instead of Arduino's make the code lighter
	#include <avr/eeprom.h>

	// OneWire library from PJRC
	#include <OneWire.h>

	// This lib executes a callback every x ms
	#include <SimpleCallbackTimer.h>

	// OneWire and other sensor related variables
	OneWire OneWireBus(ONE_WIRE_BUS);
	byte TemperatureSensorAddress[8];

	// Timers
	// The temperature conversion takes about 750ms, so, I think this is pretty reasonable
	SimpleCallbackTimer T_UpdateTemperature(775, F_UpdateTemperature);
	SimpleCallbackTimer T_UpdateTemperatureAlarmStatus(BLINKING_CHARACTER_DELAY, F_UpdateTemperatureAlarmStatus);

	// Other variables

	int8_t TemperatureThreshold;

	bool TemperatureAlarmEnabled = false;
	byte TemperatureAlarmDirection = 0;

	bool TemperatureAlarmOn = false;

	void InitTemperature()
	{
		// Print the template in the LCD
		PrintTemperatureTemplate();

		// Init the OneWire bus and find the sensor's address
		OneWireBus.search(TemperatureSensorAddress);

		// Get last used data from EEPROM

		TemperatureThreshold = eeprom_read_byte(EEPROM_ADDRESS_TEMP_THRESHOLD);
		if(TemperatureThreshold < TEMP_LOWEST_THRESHOLD || TemperatureThreshold > TEMP_HIGHEST_THRESHOLD)
			TemperatureThreshold = TEMP_DEFAULT_THRESHOLD;

		TemperatureAlarmDirection = eeprom_read_byte(EEPROM_ADDRESS_TEMP_DIRECTION);
		if(TemperatureAlarmDirection != 0 && TemperatureAlarmDirection != 1)
			TemperatureAlarmDirection = 0;

		// Start the timers, this will update the temperature every 1 second
		T_UpdateTemperature.start();

		// This, will update the character that indicates if the alarm is enabled or not
		T_UpdateTemperatureAlarmStatus.start();
	}

	void PrintTemperatureTemplate()
	{
		ClearLCDRow(0);
		ClearLCDRow(1);

		LCD.setCursor(0, 0);
		LCD.print("Alarm: ");
		LCD.setCursor(12, 0);
		LCD.print(LCD_DEGREE_CHAR);
		LCD.print('C');

		LCD.setCursor(0, 1);
		LCD.print("Temp: ");
		LCD.setCursor(12, 1);
		LCD.print(LCD_DEGREE_CHAR);
		LCD.print('C');

		UpdateTemperatureAlarmDirectionChar();
	}

	void TemperatureLoop()
	{
		T_UpdateTemperature.run();
		T_UpdateTemperatureAlarmStatus.run();

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(ReadPulse(TEMP_CHANGE_DIR_BUTTON_ID, TEMP_CHANGE_DIR_BUTTON_THRESHOLD) >= TEMP_CHANGE_DIR_BUTTON_THRESHOLD) // If the user pressed ChangeDir
		{
			TemperatureAlarmDirection = !TemperatureAlarmDirection;

			UpdateTemperatureAlarmDirectionChar();

			delay(TEMP_CHANGE_DIR_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_MINUS_BUTTON_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp--
		{
			// Limit temperature
			if(TemperatureThreshold > TEMP_LOWEST_THRESHOLD)
				TemperatureThreshold--;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_PLUS_BUTTON_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp++
		{
			// Limit temperature
			if(TemperatureThreshold < TEMP_HIGHEST_THRESHOLD)
				TemperatureThreshold++;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_START_STOP_BUTTON_ID, TEMP_START_STOP_BUTTON_THRESHOLD) >= TEMP_START_STOP_BUTTON_THRESHOLD) // If the user pressed Start/Stop
		{
			TemperatureAlarmEnabled = !TemperatureAlarmEnabled;

			TemperatureAlarmOn = false;

			// Store the temperature and the direction in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(EEPROM_ADDRESS_TEMP_THRESHOLD, TemperatureThreshold);
			eeprom_update_byte(EEPROM_ADDRESS_TEMP_DIRECTION, TemperatureAlarmDirection);

			delay(TEMP_START_STOP_BUTTON_DELAY);
		}

		// Show TemperatureThreshold

		LCD.setCursor(7, 0);
		if(TemperatureThreshold >= 0 && TemperatureThreshold < 100)
			LCD.print(' ');
		LCD.print(TemperatureThreshold);
		LCD.print(' ');
	}

	void F_UpdateTemperature()
	{
		LCD.setCursor(6, 1);

		float Temperature = NULL;

		// If the found (or not) address is a recognized one
		if(TemperatureSensorAddress[0] == 0x10 || TemperatureSensorAddress[0] == 0x28)
		{
			if(OneWireBus.reset())
			{
				// This reads the temperature from the scratchpad (sensor's memory)
				OneWireBus.select(TemperatureSensorAddress);
				OneWireBus.write(0xBE); // Read Scratchpad [BEh] command

				// Read and process the temperature
				byte LSB = OneWireBus.read();
				Temperature = ((OneWireBus.read() << 8) | LSB);
				Temperature = Temperature / 16;

				// Start another conversion; will be used in the next function call
				OneWireBus.reset();
				OneWireBus.select(TemperatureSensorAddress);
				OneWireBus.write(0x44); // Convert T [44h] command
			}
		}

		if(Temperature != NULL)
		{
			// Print it :P
			LCD.print(Temperature, 1);
			LCD.print(' ');

			// If the temperature is above or below the given temperature, activate the alarm
			if(TemperatureAlarmEnabled && ((TemperatureAlarmDirection == 0 && Temperature >= TemperatureThreshold) || (TemperatureAlarmDirection == 1 && Temperature <= TemperatureThreshold)))
				TemperatureAlarmOn = true;
		}
		else
			LCD.print("-----");
	}

	uint8_t TemperatureStateCharacter = 1;
	void F_UpdateTemperatureAlarmStatus()
	{
		LCD.setCursor(15, 0);
		
		if(TemperatureAlarmEnabled)
		{
			LCD.print(TemperatureStateCharacter ? LCD_ALARM_ENABLED_0 : LCD_ALARM_ENABLED_1);

			TemperatureStateCharacter = !TemperatureStateCharacter;
		}
		else
			LCD.print(LCD_ALARM_DISABLED);
	}

	void UpdateTemperatureAlarmDirectionChar()
	{
		LCD.setCursor(15, 1);
		LCD.print(TemperatureAlarmDirection ? LCD_TEMP_ALARM_DIRECTION_1 : LCD_TEMP_ALARM_DIRECTION_0);
	}