#include "Module_RTC.h"

	void RTCAlarm::init()
	{
		LCD->setCursor(5, 0);
		LCD->print('/');
		LCD->setCursor(8, 0);
		LCD->print('/');

		LCD->setCursor(14, 0);
		LCD->print(':');
		LCD->setCursor(17, 0);
		LCD->print(':');
	}

	void RTCAlarm::loop()
	{
		byte Second, Minute, Hour, DayOfWeek, DayOfMonth, Month, Year;

		getTime(&Second, &Minute, &Hour, &DayOfWeek, &DayOfMonth, &Month, &Year);

		const char DaysOfWeek[7][2] = {"Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sa"};
		LCD->setCursor(0, 0);
		LCD->print(DaysOfWeek[DayOfWeek][0]);
		LCD->print(DaysOfWeek[DayOfWeek][1]);

		LCD->setCursor(3, 0);
		printZerofill(DayOfMonth);
		LCD->setCursor(6, 0);
		printZerofill(Month);
		LCD->setCursor(9, 0);
		printZerofill(Year);

		LCD->setCursor(12, 0);
		printZerofill(Hour);
		LCD->setCursor(15, 0);
		printZerofill(Minute);
		LCD->setCursor(18, 0);
		printZerofill(Second);

		/*TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x00);
		TinyWireM.write(dec2bcd(Second));
		TinyWireM.write(dec2bcd(Minute));
		TinyWireM.write(dec2bcd(Hour));
		TinyWireM.write(dec2bcd(DayOfWeek));
		TinyWireM.write(dec2bcd(DayOfMonth));
		TinyWireM.write(dec2bcd(Month));
		TinyWireM.write(dec2bcd(Year));
		TinyWireM.endTransmission();*/
	}

	void RTCAlarm::getTime(uint8_t* Second, uint8_t* Minute, uint8_t* Hour, uint8_t* DayOfWeek, uint8_t* DayOfMonth, uint8_t* Month, uint8_t* Year)
	{
		// The LCD already executed begin(), so, there's no need for it
		TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x00); // 00h
		TinyWireM.endTransmission();

		TinyWireM.requestFrom(RTC_I2C_ADDRESS, 7);

		*Second 	= bcd2dec(TinyWireM.read() & 0x7f);
		*Minute 	= bcd2dec(TinyWireM.read());
		*Hour 		= bcd2dec(TinyWireM.read() & 0x3f);
		*DayOfWeek 	= bcd2dec(TinyWireM.read());
		*DayOfMonth = bcd2dec(TinyWireM.read());
		*Month 		= bcd2dec(TinyWireM.read());
		*Year 		= bcd2dec(TinyWireM.read());
	}

	uint8_t RTCAlarm::getTemperature()
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