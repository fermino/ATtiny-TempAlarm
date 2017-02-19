#ifndef TempAlarmModule_h
#define TempAlarmModule_h

	#include <LiquidCrystal_I2C.h>
	#include <OneWireSwitches.h>

	//#include "Configuration.cpp"

	class TempAlarmModule
	{
		protected: 
			LiquidCrystal_I2C* LCD;
			OneWireSwitches* Switches;

		public:
			// Can't we mark this as final
			TempAlarmModule(LiquidCrystal_I2C* L, OneWireSwitches* S);

			virtual void init() = 0;
			virtual void loop() = 0;

			virtual bool isAlarmOn() = 0;
	};

#endif