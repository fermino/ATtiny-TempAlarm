#ifndef TempAlarmModule_h
#define TempAlarmModule_h

	#include <LiquidCrystal_I2C.h>
	#include <OneWireSwitches.h>

	#include "Configuration.h"

	class TempAlarmModule
	{
		protected: 
			LiquidCrystal_I2C* LCD;
			OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* Switches;

			// Some utils

			void printZerofill(uint8_t Number);

		public:
			// Can't we mark this as final
			TempAlarmModule(LiquidCrystal_I2C* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S);

			virtual void init() = 0;
			virtual void loop() = 0;

			virtual bool isAlarmOn() = 0;
	};

#endif