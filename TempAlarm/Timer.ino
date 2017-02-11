	// This lib executes a callback every x ms
	//#include <SimpleCallbackTimer.h>

	// A set of timer utilities for the kitchen :P
	#include <KitchenTimer.h>

	// This is the desired amount of timers enabled
	#define _TIMERS (MAX_TIMERS < LCD_ROWS - 2 ? MAX_TIMERS : LCD_ROWS - 2)
	const uint8_t _Timers = _TIMERS;
	
	// Those timers
	KitchenTimer KitchenTimers[_TIMERS];
	uint8_t SelectedTimer = 0;

	// Internal timers
	//SimpleCallbackTimer T_UpdateTimerAlarmStatus(1000, F_UpdateTimerAlarmStatus);

	// Other variables

	bool TimerAlarmOn = false;

	void InitTimer()
	{
		PrintTimerTemplate();

		// This timer will update the data of the kitchen timers on the screen
		//T_UpdateTimerAlarmStatus.start();
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
		//T_UpdateTimerAlarmStatus.run();

		if(ReadPulse(TIMER_ADD_HOUR_BUTTON_ID, TIMER_ADD_BUTTON_THRESHOLD) >= TIMER_ADD_BUTTON_THRESHOLD && KitchenTimers[SelectedTimer].getTime() <= TIMER_LIMIT - 3600)
		{
			KitchenTimers[SelectedTimer].addHours();
			delay(TIMER_ADD_BUTTON_DELAY);
		}
		else if(ReadPulse(TIMER_ADD_MINUTE_BUTTON_ID, TIMER_ADD_BUTTON_THRESHOLD) >= TIMER_ADD_BUTTON_THRESHOLD && KitchenTimers[SelectedTimer].getTime() <= TIMER_LIMIT - 60)
		{
			KitchenTimers[SelectedTimer].addMinutes();
			delay(TIMER_ADD_BUTTON_DELAY);
		}
		else if(ReadPulse(TIMER_ADD_SECOND_BUTTON_ID, TIMER_ADD_BUTTON_THRESHOLD) >= TIMER_ADD_BUTTON_THRESHOLD && KitchenTimers[SelectedTimer].getTime() <= TIMER_LIMIT - 1)
		{
			KitchenTimers[SelectedTimer].addSeconds();
			delay(TIMER_ADD_BUTTON_DELAY);
		}
		else
		{
			uint16_t Pulse = ReadPulse(TIMER_CONTROL_BUTTON_ID, TIMER_CONTROL_BUTTON_MAX_THRESHOLD);

			if(Pulse >= TIMER_CONTROL_RESET_BUTTON_THRESHOLD)
			{
				KitchenTimers[SelectedTimer].reset();
				delay(TIMER_CONTROL_RESET_BUTTON_DELAY);
			}
			else if(Pulse >= TIMER_CONTROL_SWITCH_BUTTON_THRESHOLD)
			{
				if(SelectedTimer < _Timers - 1)
					SelectedTimer++;
				else
					SelectedTimer = 0;

				delay(TIMER_CONTROL_SWITCH_BUTTON_DELAY);
			}
			else if(Pulse >= TIMER_CONTROL_START_STOP_BUTTON_THRESHOLD)
			{
				KitchenTimers[SelectedTimer].toggle();

				delay(TIMER_CONTROL_START_STOP_BUTTON_DELAY);
			}
		}

		UpdateTimerAlarmStatus();
	}

	void UpdateTimerAlarmStatus()
	{
		for(uint8_t i = 0; i < _Timers; i++)
		{
			LCD.setCursor(0, i + 2);
			LCD.print(i == SelectedTimer ? '>' : ' ');

			LCD.setCursor(2, i + 2);
			PrintZerofill(KitchenTimers[i].getHours());
			LCD.setCursor(5, i + 2);
			PrintZerofill(KitchenTimers[i].getMinutes());
			LCD.setCursor(8, i + 2);
			PrintZerofill(KitchenTimers[i].getSeconds());
		}
	}

	void PrintZerofill(uint8_t Number)
	{
		if(Number < 10)
			LCD.print('0');

		LCD.print(Number);
	}