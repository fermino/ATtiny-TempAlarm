	// DS18B20 (DS18S20 and DS1820 should work too)
	#include <OneWire.h>
	#include <DallasTemperature.h>

	#include <SimpleCallbackTimer.h>

	// OneWire and DallasTemperature
	OneWire OneWireBus(ONE_WIRE_BUS);
	DallasTemperature Sensors(&OneWireBus);

	// Timers
	SimpleCallbackTimer T_UpdateTemperature;
	SimpleCallbackTimer T_UpdateAlarmStateChar;

	// Other variables

	int8_t TemperatureThreshold;

	bool TemperatureAlarmEnabled = false;
	bool TemperatureAlarmDirection = 0;

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

		TemperatureAlarmDirection = eeprom_read_byte(EEPROM_MODE_ADDRESS);

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

		T_UpdateAlarmStateChar.setCallback(F_UpdateAlarmStateChar);
		T_UpdateAlarmStateChar.setInterval(BLINKING_CHARACTER_DELAY);
		T_UpdateAlarmStateChar.start(true);
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

		//ClearBlinkingCharacter();
		//UpdateModeCharacter();
	}

	void TemperatureLoop()
	{
		T_UpdateTemperature.run();
		T_UpdateAlarmStateChar.run();
	}

	void F_UpdateTemperature()
	{
		float Temperature = Sensors.getTempCByIndex(0);

		LCD.setCursor(6, 1);
		
		if(Temperature != DEVICE_DISCONNECTED_C)
		{
			LCD.print(Temperature, 1);
			LCD.print(' ');
		}
		else
			LCD.print("-----");

		Sensors.requestTemperatures();
	}

	uint8_t TemperatureStateCharacter = 1;
	void F_UpdateAlarmStateChar()
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

