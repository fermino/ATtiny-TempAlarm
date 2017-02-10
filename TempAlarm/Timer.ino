	// This lib executes a callback every x ms
	#include <SimpleCallbackTimer.h>

	// This library is just a simple countdown timer :P
	#include <KitchenTimer_Countdown.h>

	// This is the desired amount of timers enabled
	#define _TIMERS (MAX_TIMERS < LCD_ROWS - 2 ? MAX_TIMERS : LCD_ROWS - 2)
	const uint8_t _Timers = _TIMERS;
	
	// Those timers
	KitchenTimer_Countdown KitchenTimers[_TIMERS];
	uint8_t SelectedTimer = 0;

	// Internal timers
	SimpleCallbackTimer T_UpdateTimerAlarmStatus(1000, F_UpdateTimerAlarmStatus);

	// Other variables

	bool TimerAlarmOn = false;

	void InitTimer()
	{
		PrintTimerTemplate();

		// This timer will update the data of the kitchen timers on the screen
		T_UpdateTimerAlarmStatus.start();
	}

	void PrintTimerTemplate()
	{
		for(int i = 2; i - 2 < _TIMERS; i++)
		{
			ClearLCDRow(i);

			LCD.setCursor(4, i);
			LCD.print(':');
			LCD.setCursor(7, i);
			LCD.print(':');
		}
	}

	void TimerLoop()
	{
		T_UpdateTimerAlarmStatus.run();
	}

	void F_UpdateTimerAlarmStatus()
	{
		for(uint8_t i = 0; i < _Timers; i++)
		{
			LCD.setCursor(0, i + 2);
			LCD.print(i == SelectedTimer ? '>' : ' ');

			LCD.setCursor(2, i + 2);
			PrintZerofill(KitchenTimers[i].getRemainingHours());
			LCD.setCursor(5, i + 2);
			PrintZerofill(KitchenTimers[i].getRemainingMinutes());
			LCD.setCursor(8, i + 2);
			PrintZerofill(KitchenTimers[i].getRemainingSeconds());
		}
	}

	void PrintZerofill(uint8_t Number)
	{
		if(Number < 10)
			LCD.print('0');

		LCD.print(Number);
	}