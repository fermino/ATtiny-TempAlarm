#ifndef TemperatureAlarm_h
#define TemperatureAlarm_h

	#if ARDUINO >= 100
		#include <Arduino.h>
	#else
		#include <WProgram.h>
	#endif
	
	#include "TempAlarmModule.h"

	#include <avr/eeprom.h>
	#include <OneWire.h>

	#include "Configuration.cpp"
	#include "config/Temperature.h"

	class TemperatureAlarm : public TempAlarmModule
	{
		//using TempAlarmModule::TempAlarmModule;

		private:
			OneWire OW = OneWire(TEMPERATURE_ONE_WIRE_BUS);
			byte SensorAddress[8];

			bool Enabled = false;
			bool DirectionUp = true;

			int8_t Threshold;

			bool AlarmOn = false;

			// Timers

			uint32_t UpdateTemperature_StartedAt = 0;
			void UpdateTemperature();

			uint8_t CurrentStatusCharacter = 1;
			uint32_t UpdateStatus_StartedAt = 0;
			void UpdateStatus();

			void UpdateDirection();

		public:
			TemperatureAlarm(LiquidCrystal_I2C* L, OneWireSwitches<SWITCHES_AMOUNT>* S);
			void init();
			void loop();

			bool isAlarmOn();
			void F_UpdateTemperature();
	};

#endif