#ifndef RTCAlarm_h
#define RTCAlarm_h

	#include "TempAlarmModule.h"

 	#include <TinyWireM.h>
	#include "config/RTC.h"

	static const char DaysOfWeek[7][3] {RTC_DAYS_OF_WEEK};

	class RTCAlarm : public TempAlarmModule
	{
		using TempAlarmModule::TempAlarmModule;

		private:
			bool AlarmOn = false;

			void getTime(uint8_t* Second, uint8_t* Minute, uint8_t* Hour, uint8_t* DayOfWeek, uint8_t* DayOfMonth, uint8_t* Month, uint8_t* Year);
			uint8_t getTemperature();

			inline uint8_t dec2bcd(uint8_t dec)
			{ return (dec / 10 * 16) + (dec % 10); }
			inline uint8_t bcd2dec(uint8_t bcd)
			{ return (bcd / 16 * 10) + (bcd % 16); }

		public:
			void init();
			void loop();

			bool isAlarmOn();
	};

#endif