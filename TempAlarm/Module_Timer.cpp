#include "Module_Timer.h"

	void TimerAlarm::init()
	{
		// Create characters
		LCD->createChar(TIMER_STOPWATCH_CHAR_INDEX, Timer_StopwatchChar);
		LCD->createChar(TIMER_COUNTDOWN_CHAR_INDEX, Timer_CountdownChar);
	}

	void TimerAlarm::loop()
	{
		// Timers

		if(millis() >= (UpdateAlarmStatus_StartedAt + TIMER_STATUS_UPDATE_DELAY))
		{
			UpdateAlarmStatus_StartedAt = millis();

			UpdateAlarmStatus();
		}

		// Buttons

		addTime
		(
			Switches->readKeyPulse(TIMER_BUTTON_ADD_HOUR_ID, TIMER_BUTTON_ADD_THRESHOLD) >= TIMER_BUTTON_ADD_THRESHOLD ? 3600 :
			Switches->readKeyPulse(TIMER_BUTTON_ADD_MINUTE_ID, TIMER_BUTTON_ADD_THRESHOLD) >= TIMER_BUTTON_ADD_THRESHOLD ? 60 :
			Switches->readKeyPulse(TIMER_BUTTON_ADD_SECOND_ID, TIMER_BUTTON_ADD_THRESHOLD) >= TIMER_BUTTON_ADD_THRESHOLD ? 1 : 0
		);

		if(Switches->readKeyPulse(TIMER_BUTTON_STARTSTOP_ID, TIMER_BUTTON_STARTSTOP_THRESHOLD) >= TIMER_BUTTON_STARTSTOP_THRESHOLD)
		{
			if(Timers[Selected].Started)
				stop(Selected);
			else
				start(Selected);

			printTime();

			delay(TIMER_BUTTON_STARTSTOP_DELAY);
		}
		else
		{
			uint16_t PulseLength = Switches->readKeyPulse(TIMER_BUTTON_CONTROL_ID, TIMER_BUTTON_CONTROL_RESET_THRESHOLD);

			if(PulseLength >= TIMER_BUTTON_CONTROL_RESET_THRESHOLD)
			{
				reset(Selected);

				printTime();

				delay(TIMER_BUTTON_CONTROL_RESET_DELAY);
			}
			else if(PulseLength >= TIMER_BUTTON_CONTROL_SWITCH_THRESHOLD)
			{
				if((++Selected) == TIMER_AMOUNT)
					Selected = 0;

				/*if(Selected < TIMER_AMOUNT - 1)
					Selected++;
				else
					Selected = 0;*/

				printTime();

				delay(TIMER_BUTTON_CONTROL_SWITCH_DELAY);
			}
		}

		// Print time :P
		printTime();
	}

	void TimerAlarm::printTime()
	{
		for(uint8_t i = 0; i < TIMER_AMOUNT; i++)
		{
			// Print selected timer
			LCD->setCursor(1, i + 2);
			LCD->print(i == Selected ? TIMER_SELECTED_TIMER_CHAR : ' ');
			LCD->print(' ');

			// Print current time
			// There's no need to floor() it because uint8_t cast in printZeroFill argument
			printZerofill(getTime(i) / 3600);
			LCD->print(':');
			printZerofill(getTime(i) % 3600 / 60);
			LCD->print(':');
			printZerofill(getTime(i) % 60);
		}
	}

	void TimerAlarm::UpdateAlarmStatus()
	{
		// Here we turn the alarm off
		// If any timer hasFinished(), we'll enable it again
		AlarmOn = false;

		for(uint8_t i = 0; i < TIMER_AMOUNT; i++)
		{
			// Print timer status
			LCD->setCursor(0, i + 2);

			if(hasFinished(i))
			{
				if(StatusCharacters[i])
					LCD->print(TIMER_FINISHED_CHAR);
				else
					LCD->write(Timers[i].Mode + 2); // Just a trick, char's index must match every timer mode

				StatusCharacters[i] = !StatusCharacters[i];

				AlarmOn = true;
			}
			else if(Timers[i].Started)
			{
				if(StatusCharacters[i])
					LCD->print(TIMER_ENABLED_CHAR);
				else
					LCD->write(Timers[i].Mode + 2); // Same trick as above

				StatusCharacters[i] = !StatusCharacters[i];
			}
			else
				LCD->write(Timers[i].Mode + 2); // Same :P
		}
	}

	uint32_t TimerAlarm::getTime(uint8_t TimerIndex)
	{
		if(Timers[TimerIndex].Mode == TIMER_MODE_STOPWATCH)
		{
			if(Timers[TimerIndex].Started)
			{
				// Save current ELAPSED time, then stop() will only pause the stopwatch
				Timers[TimerIndex].Time = seconds() - Timers[TimerIndex].StartedAt;

				// Limit max time
				if(Timers[TimerIndex].Time > TIMER_TIME_LIMIT)
				{
					stop(TimerIndex);

					Timers[TimerIndex].Time = TIMER_TIME_LIMIT;
				}
			}

			// Return saved time or 0
			return Timers[TimerIndex].Time;
		}
		else
		{
			// If is not started or has already finished, we'll return the set time
			if(!Timers[TimerIndex].Started || hasFinished(TimerIndex))
				return Timers[TimerIndex].Time;

			return Timers[TimerIndex].StartedAt + Timers[TimerIndex].Time - seconds();
		}
	}

	void TimerAlarm::addTime(uint32_t Seconds /* = 1 */)
	{
		// Limit time addition (also prevents buffer overflow)
		//if((Time > 0) && ((Seconds > (0xFFFFFFFF - Time)) || ((Time + Seconds) > KITCHENTIMER_TIMELIMIT)))
		// The first check will avoid changing the mode if we're trying to add 0
		if(Seconds == 0 || Timers[Selected].Time + Seconds > TIMER_TIME_LIMIT)
			return;

		// If the user added some time, we can think that he wants a countdown!
		Timers[Selected].Mode = TIMER_MODE_COUNTDOWN;

		Timers[Selected].Time += Seconds;
	}

	void TimerAlarm::start(uint8_t TimerIndex)
	{
		Timers[TimerIndex].StartedAt = seconds() - (Timers[TimerIndex].Mode == TIMER_MODE_STOPWATCH ? Timers[TimerIndex].Time : 0);

		Timers[TimerIndex].Started = true;
	}

	void TimerAlarm::stop(uint8_t TimerIndex)
	{
		Timers[TimerIndex].StartedAt = 0;
		Timers[TimerIndex].Started = false;
	}

	void TimerAlarm::reset(uint8_t TimerIndex)
	{
		stop(TimerIndex);
		Timers[TimerIndex].Time = 0;

		// If the user adds time, this will be changed, else, it will count up
		Timers[TimerIndex].Mode = TIMER_MODE_STOPWATCH;
	}

	bool TimerAlarm::hasFinished(uint8_t TimerIndex)
	{
		if(Timers[TimerIndex].Started && Timers[TimerIndex].Mode == TIMER_MODE_COUNTDOWN)
			return seconds() >= Timers[TimerIndex].StartedAt + Timers[TimerIndex].Time;

		return false;
	}

	bool TimerAlarm::isAlarmOn()
	{ return AlarmOn; }