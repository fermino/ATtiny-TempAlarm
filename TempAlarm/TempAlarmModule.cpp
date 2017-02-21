#include "TempAlarmModule.h"

	TempAlarmModule::TempAlarmModule(LiquidCrystal_I2C* L, OneWireSwitches<SWITCHES_AMOUNT>* S)
	{
		LCD = L;
		Switches = S;
	}

	void TempAlarmModule::printZerofill(uint8_t Number)
	{
		if(Number < 10)
			LCD->print('0');

		LCD->print(Number);
	}