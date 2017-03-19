/**
 * Thanks for helping me to optimize the code (more then 1.3K flash saved!) to
 * 
 * https://edwardmallon.wordpress.com/2014/03/12/using-a-ds18b20-temp-sensor-without-a-dedicated-library/
 * http://bildr.org/2011/07/ds18b20-arduino/
 */

	#include "Module_Temperature.h"

	TemperatureAlarm::TemperatureAlarm(LiquidCrystal_I2C* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S)
	: TempAlarmModule(L, S)
	{}

	void TemperatureAlarm::init()
	{
		// Create characters
		LCD->createChar(TEMPERATURE_DIRECTION_UP_CHAR_INDEX, Temperature_DirectionUpChar);
		LCD->createChar(TEMPERATURE_DIRECTION_DOWN_CHAR_INDEX, Temperature_DirectionDownChar);

		// Print template
		LCD->setCursor(0, 1);
		LCD->print(F("Sens:"));

		UpdateDirection();

		// Get last used data from EEPROM

		Threshold = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_THRESHOLD);
		if(Threshold < TEMPERATURE_LOWEST_THRESHOLD || Threshold > TEMPERATURE_HIGHEST_THRESHOLD)
			Threshold = TEMPERATURE_DEFAULT_THRESHOLD;

		DirectionUp = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_DIRECTION);
		if(DirectionUp != false && DirectionUp != true)
			DirectionUp = true;
	}

	void TemperatureAlarm::loop()
	{
		// Timers

		if((millis() - UpdateTemperature_StartedAt) >= TEMPERATURE_CONVERSION_TIME)
		{
			UpdateTemperature_StartedAt = millis();

			UpdateTemperature();
		}
		if((millis() - UpdateStatus_StartedAt) >= TEMPERATURE_STATUS_UPDATE_DELAY)
		{
			UpdateStatus_StartedAt = millis();

			UpdateStatus();
		}

		// Buttons

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(Switches->readKeyPulse(TEMPERATURE_BUTTON_DIRECTION_ID, TEMPERATURE_BUTTON_DIRECTION_THRESHOLD) >= TEMPERATURE_BUTTON_DIRECTION_THRESHOLD)
		{
			DirectionUp = !DirectionUp;
			UpdateDirection();

			delay(TEMPERATURE_BUTTON_DIRECTION_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_MINUS_ID, TEMPERATURE_BUTTON_MINUS_THRESHOLD) >= TEMPERATURE_BUTTON_MINUS_THRESHOLD)
		{
			if(Threshold > TEMPERATURE_LOWEST_THRESHOLD)
				Threshold--;

			delay(TEMPERATURE_BUTTON_MINUS_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_PLUS_ID, TEMPERATURE_BUTTON_PLUS_THRESHOLD) >= TEMPERATURE_BUTTON_PLUS_THRESHOLD)
		{
			if(Threshold < TEMPERATURE_HIGHEST_THRESHOLD)
				Threshold++;

			delay(TEMPERATURE_BUTTON_PLUS_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_STARTSTOP_ID, TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD) >= TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD)
		{
			AlarmOn = false;

			Enabled = !Enabled;

			// Store the temperature and the direction in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(TEMPERATURE_EEPROM_ADDRESS_THRESHOLD, Threshold);
			eeprom_update_byte(TEMPERATURE_EEPROM_ADDRESS_DIRECTION, DirectionUp);

			delay(TEMPERATURE_BUTTON_STARTSTOP_DELAY);
		}

		// Show Threshold temperature

		LCD->setCursor(14, 1);
		if(Threshold >= 0 && Threshold < 100)
			LCD->print(' ');
		LCD->print(Threshold);
		LCD->print(TEMPERATURE_DEGREE_CHAR);
		LCD->print(' ');
	}

	void TemperatureAlarm::UpdateTemperature()
	{
		LCD->setCursor(5, 1);

		int8_t Temperature = -127;
		uint8_t DecimalPart = 0;
		
		if(OW.reset())
		{
			// This reads the temperature from the scratchpad (sensor's memory)
			OW.skip(); // Since there's only one device in the bus, we can skip the address
			OW.write(0xBE); // Read Scratchpad [BEh] command

			// Read and process the temperature
			uint8_t LSB = OW.read();

			Temperature = (OW.read() << 4) | (LSB >> 4);
			DecimalPart = (LSB & 0b00001111) * 0.625f;

			if(OW.reset())
			{
				// Start another conversion; will be used in the next function call
				OW.skip();
				OW.write(0x44); // Convert T [44h] command
			}
		}

		if(Temperature != -127)
		{
			// Print it :P
			if(Temperature >= 0 && Temperature < 100)
				LCD->print(' ');
			LCD->print(Temperature);
			LCD->print('.');
			LCD->print(DecimalPart);
			LCD->print(TEMPERATURE_DEGREE_CHAR);
			LCD->print(' ');

			// If the temperature is above or below the given temperature, activate the alarm
			if(Enabled && ((DirectionUp == 0 && Temperature <= Threshold) || (DirectionUp == 1 && Temperature >= Threshold)))
				AlarmOn = true;
			else
				AlarmOn = false;
		}
		else
		{
			LCD->print(F(" ----- "));

			if(Enabled)
				AlarmOn = true;
		}
	}

	void TemperatureAlarm::UpdateStatus()
	{
		LCD->setCursor(19, 1);
		
		if(Enabled)
		{
			LCD->print(CurrentStatusCharacter ? TEMPERATURE_ALARM_ENABLED_0_CHAR : TEMPERATURE_ALARM_ENABLED_1_CHAR);

			CurrentStatusCharacter = !CurrentStatusCharacter;
		}
		else
			LCD->print(TEMPERATURE_ALARM_DISABLED_CHAR);
	}

	void TemperatureAlarm::UpdateDirection()
	{
		LCD->setCursor(12, 1);
		LCD->write(DirectionUp ? TEMPERATURE_DIRECTION_UP_CHAR_INDEX : TEMPERATURE_DIRECTION_DOWN_CHAR_INDEX);
	}

	bool TemperatureAlarm::isAlarmOn()
	{ return AlarmOn; }