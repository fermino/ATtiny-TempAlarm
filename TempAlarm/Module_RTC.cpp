#include "Module_RTC.h"

	void RTCAlarm::init()
	{
		LCD->setCursor(14, 0);
		LCD->print(":  :");

		LCD->setCursor(12, 2);
		LCD->print(F("Ambiente"));

		if(Switches->readKey(RTC_BUTTON_CONFIGURE_ID))
		{
			loop();

			setAlarmStatus(true);
			delay(RTC_BUTTON_CONFIGURE_BUZZER_TIME);
			setAlarmStatus(false);

			for(int8_t ByteToConfigure = 6; ByteToConfigure >= 0; ByteToConfigure--)
			{
				for(;;)
				{
					if(Switches->readKeyPulse(RTC_BUTTON_MINUS_ID, RTC_BUTTON_MINUS_THRESHOLD) >= RTC_BUTTON_MINUS_THRESHOLD)
					{
						if(Time[ByteToConfigure] > TimeLowerLimits[ByteToConfigure])
						{
							Time[ByteToConfigure]--;
						}
					}
					else if(Switches->readKeyPulse(RTC_BUTTON_PLUS_ID, RTC_BUTTON_PLUS_THRESHOLD) >= RTC_BUTTON_PLUS_THRESHOLD)
					{
						if(Time[ByteToConfigure] < TimeUpperLimits[ByteToConfigure])
						{
							Time[ByteToConfigure]++;
						}
					}
					
					if(Switches->readKeyPulse(RTC_BUTTON_NEXT_ID, RTC_BUTTON_NEXT_THRESHOLD) >= RTC_BUTTON_NEXT_THRESHOLD)
					{
						setAlarmStatus(true);
						delay(RTC_BUTTON_NEXT_BUZZER_TIME);
						setAlarmStatus(false);

						delay(RTC_BUTTON_NEXT_DELAY);

						break;
					}

					setTime();
					loop();
				}
			}
		}
	}

	void RTCAlarm::loop()
	{
		getTime();

		LCD->setCursor(0, 0);
		LCD->print(Days[Time[RTC_DAY]]);

		LCD->setCursor(4, 0);
		printZerofill(Time[RTC_DATE]);
		LCD->print(Months[Time[RTC_MONTH] - 1]);
		printZerofill(Time[RTC_YEAR]);

		LCD->setCursor(12, 0);
		printZerofill(Time[RTC_HOURS]);
		LCD->setCursor(15, 0);
		printZerofill(Time[RTC_MINUTES]);
		LCD->setCursor(18, 0);
		printZerofill(Time[RTC_SECONDS]);

		LCD->setCursor(14, 3);
		LCD->print(getTemperature());
		LCD->print(RTC_DEGREE_CHAR);
		LCD->print("C ");
	}

	void RTCAlarm::getTime()
	{
		// The LCD already executed begin(), so, there's no need for it
		TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x00); // 00h
		TinyWireM.endTransmission();

		TinyWireM.requestFrom(RTC_I2C_ADDRESS, 7);

		Time[RTC_SECONDS]	= bcd2dec(TinyWireM.read() & 0x7f);
		Time[RTC_MINUTES]	= bcd2dec(TinyWireM.read());
		Time[RTC_HOURS]		= bcd2dec(TinyWireM.read() & 0x3f);
		// From 0 to 6, does not need bcd2dec() => saves 14 bytes
		Time[RTC_DAY]		= TinyWireM.read();
		Time[RTC_DATE]		= bcd2dec(TinyWireM.read());
		Time[RTC_MONTH]		= bcd2dec(TinyWireM.read());
		Time[RTC_YEAR]		= bcd2dec(TinyWireM.read());
	}

	void RTCAlarm::setTime()
	{
		TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x00);

		TinyWireM.write(dec2bcd(Time[RTC_SECONDS]));
		TinyWireM.write(dec2bcd(Time[RTC_MINUTES]));
		TinyWireM.write(dec2bcd(Time[RTC_HOURS]));
		// Same as above
		TinyWireM.write(Time[RTC_DAY]);
		TinyWireM.write(dec2bcd(Time[RTC_DATE]));
		TinyWireM.write(dec2bcd(Time[RTC_MONTH]));
		TinyWireM.write(dec2bcd(Time[RTC_YEAR]));

		TinyWireM.endTransmission();
	}

	int8_t RTCAlarm::getTemperature()
	{
		TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x11); // 011h
		TinyWireM.endTransmission();

		// Only request integer part
		TinyWireM.requestFrom(RTC_I2C_ADDRESS, 1);

		return TinyWireM.read();
	}

	bool RTCAlarm::isAlarmOn()
	{ return AlarmOn; }