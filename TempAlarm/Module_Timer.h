#ifndef TimerAlarm_h
#define TimerAlarm_h

	#include "TempAlarmModule.h"

	#include "config/Timer.h"

	#define TIMER_MODE_STOPWATCH 0
	#define TIMER_MODE_COUNTDOWN 1

	#define seconds() (millis() / 1000)

	class TimerAlarm : public TempAlarmModule
	{
		using TempAlarmModule::TempAlarmModule;

		private:
			struct Timer
			{
				bool Started = false;
				uint8_t Mode = TIMER_MODE_STOPWATCH;

				uint32_t StartedAt = 0;
				uint32_t Time = 0;
			};

			uint8_t Selected = 0;

			Timer Timers[TIMER_AMOUNT];
			uint8_t StatusCharacters[TIMER_AMOUNT];

			bool AlarmOn = false;

			void printTime();

			uint32_t getTime(uint8_t TimerIndex);
			void addTime(uint32_t Seconds = 1);

			void start(uint8_t TimerIndex);
			void stop(uint8_t TimerIndex);
			void reset(uint8_t TimerIndex);

			bool hasFinished(uint8_t TimerIndex);

			// Timers
			uint32_t UpdateAlarmStatus_StartedAt = 0;
			void UpdateAlarmStatus();

		public:
			void init();
			void loop();

			bool isAlarmOn();
	};

#endif