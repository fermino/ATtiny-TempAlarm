#ifndef RTCAlarm_h
#define RTCAlarm_h

	#include "TempAlarmModule.h"

 	#include <TinyWireM.h>
	#include "config/RTC.h"

	#define RTC_SECONDS	0
	#define RTC_MINUTES	1
	#define RTC_HOURS 	2
	#define RTC_DAY		3
	#define RTC_DATE	4
	#define RTC_MONTH	5
	#define RTC_YEAR	6

	static const char Days[7][4]	{RTC_DAY_0, RTC_DAY_1, RTC_DAY_2, RTC_DAY_3, RTC_DAY_4, RTC_DAY_5, RTC_DAY_6};
	static const char Months[12][4]	{RTC_MONTH_0, RTC_MONTH_1, RTC_MONTH_2, RTC_MONTH_3, RTC_MONTH_4, RTC_MONTH_5, RTC_MONTH_6, RTC_MONTH_7, RTC_MONTH_8, RTC_MONTH_9, RTC_MONTH_10, RTC_MONTH_11};

	extern void setAlarmStatus(bool Enabled);

	class RTCAlarm : public TempAlarmModule
	{
		using TempAlarmModule::TempAlarmModule;

		private:
			bool AlarmOn = false;

			// Seconds, minutes, hours, day, date, month, year
			const uint8_t TimeUpperLimits[7] {59, 59, 23, 7, 31, 12, 99};
			const uint8_t TimeLowerLimits[7] { 0,  0,  0, 1,  1,  1,  0};

			uint8_t Time[7];

			void getTime();
			void setTime();

			int8_t getTemperature();

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