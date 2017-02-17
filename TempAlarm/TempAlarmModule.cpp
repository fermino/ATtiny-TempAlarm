#include "TempAlarmModule.h"

	TempAlarmModule::TempAlarmModule(LiquidCrystal_I2C* L, OneWireSwitches* S)
	{
		LCD = L;
		Switches = S;
	}