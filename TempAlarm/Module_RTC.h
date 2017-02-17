#ifndef RTCAlarm_h
#define RTCAlarm_h

	#if ARDUINO >= 100
		#include <Arduino.h>
	#else
		#include <WProgram.h>
	#endif
	
	#include "TempAlarmModule.h"

 	#include <TinyWireM.h>
	#include "config/RTC.h"

	class RTCAlarm : public TempAlarmModule
	{
		using TempAlarmModule::TempAlarmModule;

		private:
			bool AlarmOn = false;

			void getTime(byte* Second, byte* Minute, byte* Hour, byte* DayOfWeek, byte* DayOfMonth, byte* Month, byte* Year);

			inline byte dec2bcd(byte dec)
			{ return (dec / 10 * 16) + (dec % 10); }
			inline byte bcd2dec(byte bcd)
			{ return (bcd / 16 * 10) + (bcd % 16); }

			void printZerofill(uint8_t Number);

		public:
			void init();
			void loop();

			bool isAlarmOn();
	};

#endif