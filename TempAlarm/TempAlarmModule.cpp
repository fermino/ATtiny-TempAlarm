#include "TempAlarmModule.h"

	TempAlarmModule::TempAlarmModule(LiquidCrystal_I2C* L, OneWireSwitches<SWITCHES_AMOUNT>* S)
	{
		LCD = L;
		Switches = S;
	}