	// This lib executes a callback every x ms
	#include <SimpleCallbackTimer.h>

	// This library is just a simple countdown timer :P
	#include <KitchenTimer_Countdown.h>

	// This is the desired amount of timers enabled
	#define _TIMERS (MAX_TIMERS < LCD_ROWS - 2 ? MAX_TIMERS : LCD_ROWS - 2)
	
	// Those timers
	KitchenTimer_Countdown KitchenTimers[_TIMERS];

	// Internal timers
	SimpleCallbackTimer T_UpdateTimerStatus(1000, F_UpdateTimerStatus);

	// Other variables

	bool TimerAlarmOn = false;

	void InitTimer()
	{
		PrintTimerTemplate();

		// This timer will update the data of the kitchen timers on the screen
		T_UpdateTimerStatus.start();
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
		T_UpdateTimerStatus.run();
	}

	void F_UpdateTimerStatus()
	{
		for(int i = 2; i - 2 < _TIMERS; i++)
		{
			LCD.setCursor(2, i);
			PrintZerofill(KitchenTimers[i - 2].getRemainingHours());
			LCD.setCursor(5, i);
			PrintZerofill(KitchenTimers[i - 2].getRemainingMinutes());
			LCD.setCursor(8, i);
			PrintZerofill(KitchenTimers[i - 2].getRemainingSeconds());
		}
	}

	void PrintZerofill(uint8_t Number)
	{
		if(Number < 10)
			LCD.print('0');

		LCD.print(Number);
	}