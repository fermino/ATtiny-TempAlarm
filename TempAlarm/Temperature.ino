	// EEPROM
	#include <avr/eeprom.h>

	// DS18B20 (DS18S20 and DS1820 should work too)
	#include <OneWire.h>
	#include <DallasTemperature.h>

	// This lib executes a callback every x ms
	#include <SimpleCallbackTimer.h>

	// OneWire and DallasTemperature
	OneWire OneWireBus(ONE_WIRE_BUS);
	DallasTemperature Sensors(&OneWireBus);

	// Timers
	SimpleCallbackTimer T_UpdateTemperature;
	SimpleCallbackTimer T_UpdateAlarmStatus;

	// Other variables

	int8_t TemperatureThreshold;

	bool TemperatureAlarmEnabled = false;
	bool TemperatureAlarmDirection = 0;

	bool TemperatureAlarmOn = false;

	void InitTemperature()
	{
		// Print the template in the LCD
		PrintTemperatureTemplate();

		// Init the OneWire bus and DallasTemperature
		Sensors.begin();
		// We need to perform other things while the sensor making the conversion
		Sensors.setWaitForConversion(false);
		// Start a conversion, we want to have a stable value as soon as possible
		// Note: Sensors.requestTemperaturesByIndex(0) uses around 200 more bytes of flash
		Sensors.requestTemperatures();

		// Get last data from EEPROM

		TemperatureThreshold = eeprom_read_byte(EEPROM_TEMPERATURE_ADDRESS);

		if(TemperatureThreshold < LOWEST_TEMPERATURE || TemperatureThreshold > HIGHEST_TEMPERATURE)
		{
			TemperatureThreshold = START_TEMPERATURE;
			//eeprom_update_byte(EEPROM_TEMPERATURE_ADDRESS, TemperatureThreshold);
			// If the user starts the alarm, it will be saved, but if not, 
		}

		TemperatureAlarmDirection = eeprom_read_byte(EEPROM_DIRECTION_ADDRESS);

		if(TemperatureAlarmDirection != false && TemperatureAlarmDirection != true)
		{
			TemperatureAlarmDirection = false;
			//eeprom_update_byte(EEPROM_MODE_ADDRESS, AlarmReverse);
		}

		// Setup the timers, this will update the temperature every 1 second

		T_UpdateTemperature.setCallback(F_UpdateTemperature);
		T_UpdateTemperature.setInterval(500);
		T_UpdateTemperature.start(true);

		// This, will update the character that indicates if the alarm is enabled or not

		T_UpdateAlarmStatus.setCallback(F_UpdateAlarmStatus);
		T_UpdateAlarmStatus.setInterval(BLINKING_CHARACTER_DELAY);
		T_UpdateAlarmStatus.start(true);
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
	}

	void TemperatureLoop()
	{
		T_UpdateTemperature.run();
		T_UpdateAlarmStatus.run();

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(ReadPulse(CHANGE_DIR_ID, CHANGE_DIR_BUTTON_THRESHOLD) >= CHANGE_DIR_BUTTON_THRESHOLD) // If the user pressed ChangeDir
		{
			TemperatureAlarmDirection = !TemperatureAlarmDirection;

			delay(CHANGE_DIR_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_MINUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp--
		{
			// Limit temperature
			if(TemperatureThreshold > LOWEST_TEMPERATURE)
				TemperatureThreshold--;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_PLUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp++
		{
			// Limit temperature
			if(TemperatureThreshold < HIGHEST_TEMPERATURE)
				TemperatureThreshold++;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(START_STOP_ID, START_STOP_BUTTON_THRESHOLD) >= START_STOP_BUTTON_THRESHOLD) // If the user pressed Start/Stop
		{
			TemperatureAlarmEnabled = !TemperatureAlarmEnabled;

			TemperatureAlarmOn = false;

			// Store the temperature and the direction in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(EEPROM_TEMPERATURE_ADDRESS, TemperatureThreshold);
			eeprom_update_byte(EEPROM_DIRECTION_ADDRESS, TemperatureAlarmDirection);

			delay(START_STOP_BUTTON_DELAY);
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
		float Temperature = Sensors.getTempCByIndex(0);

		LCD.setCursor(6, 1);
		
		if(Temperature != DEVICE_DISCONNECTED_C)
		{
			LCD.print(Temperature, 1);
			LCD.print(' ');

			// If the temperature is above or below the given temperature, activate the alarm
			if(TemperatureAlarmEnabled)
			{
				if(TemperatureAlarmDirection == 0 && Temperature >= TemperatureThreshold)
					TemperatureAlarmOn = true;

				if(TemperatureAlarmDirection == 1 && Temperature <= TemperatureThreshold)
					TemperatureAlarmOn = true;
			}
		}
		else
			LCD.print("-----");

		// Schedule another conversion
		Sensors.requestTemperatures();
	}

	uint8_t TemperatureStateCharacter = 1;
	void F_UpdateAlarmStatus()
	{
		LCD.setCursor(15, 0);
		
		if(TemperatureAlarmEnabled)
		{
			LCD.print(TemperatureStateCharacter ? LCD_ALARM_ENABLED_0 : LCD_ALARM_ENABLED_1);

			TemperatureStateCharacter = !TemperatureStateCharacter;
		}
		else
			LCD.print(LCD_ALARM_DISABLED);

		LCD.setCursor(15, 1);
		LCD.print(TemperatureAlarmDirection ? LCD_ALARM_DIRECTION_1 : LCD_ALARM_DIRECTION_0);
	}

	bool isTemperatureAlarmOn()
	{ return TemperatureAlarmOn; }