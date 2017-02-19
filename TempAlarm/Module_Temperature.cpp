/**
 * Thanks for helping me to optimize the code (more then 1.3K flash saved!) to
 * 
 * https://edwardmallon.wordpress.com/2014/03/12/using-a-ds18b20-temp-sensor-without-a-dedicated-library/
 * http://bildr.org/2011/07/ds18b20-arduino/
 */

	#include "Module_Temperature.h"

	TemperatureAlarm::TemperatureAlarm(LiquidCrystal_I2C* L, OneWireSwitches* S)
	: TempAlarmModule(L, S)
	{}

	void TemperatureAlarm::init()
	{
		// Print template
		LCD->setCursor(0, 1);
		LCD->print(TEMPERATURE_DEGREE_CHAR);
		LCD->print("C:");

		LCD->setCursor(10, 1);
		LCD->print("Al:");

		// Find sensor's address
		OW.search(SensorAddress);

		// Get last used data from EEPROM

		Threshold = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_THRESHOLD);
		if(Threshold < TEMPERATURE_LOWEST_THRESHOLD || Threshold > TEMPERATURE_HIGHEST_THRESHOLD)
			Threshold = TEMPERATURE_DEFAULT_THRESHOLD;

		Direction = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_DIRECTION);
		if(Direction != 0 && Direction != 1)
			Direction = 0;


		//UpdateTemperatureAlarmDirectionChar();
	}

	void TemperatureAlarm::loop()
	{
		// Timers
		if(millis() >= (UpdateTemperature_StartedAt + TEMPERATURE_CONVERSION_TIME))
		{
			UpdateTemperature_StartedAt = millis();

			UpdateTemperature();
		}

		// Show Threshold temperature

		LCD->setCursor(14, 1);
		if(Threshold >= 0 && Threshold < 100)
			LCD->print(' ');
		LCD->print(Threshold);
		LCD->print(' ');
	}

	void TemperatureAlarm::UpdateTemperature()
	{
		LCD->setCursor(4, 1);

		float Temperature = NULL;

		// If the found (or not) address is a recognized one
		if(SensorAddress[0] == 0x10 || SensorAddress[0] == 0x28)
		{
			if(OW.reset())
			{
				// This reads the temperature from the scratchpad (sensor's memory)
				OW.select(SensorAddress);
				OW.write(0xBE); // Read Scratchpad [BEh] command

				// Read and process the temperature
				byte LSB = OW.read();
				Temperature = ((OW.read() << 8) | LSB);
				Temperature = Temperature / 16;

				// Start another conversion; will be used in the next function call
				OW.reset();
				OW.select(SensorAddress);
				OW.write(0x44); // Convert T [44h] command
			}
		}

		if(Temperature != NULL)
		{
			// Print it :P
			LCD->print(Temperature, 1);
			LCD->print(' ');

			// If the temperature is above or below the given temperature, activate the alarm
			if(Enabled && ((Direction == 0 && Temperature >= Threshold) || (Direction == 1 && Temperature <= Threshold)))
				AlarmOn = true;
		}
		else
			LCD->print("-----");
	}

	bool TemperatureAlarm::isAlarmOn()
	{ return AlarmOn; }

	void TemperatureAlarm::F_UpdateTemperature()
	{

	}