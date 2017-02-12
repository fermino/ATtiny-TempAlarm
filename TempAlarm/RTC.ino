	#include <TinyWireM.h>

	const char DaysOfWeek[7][2] = {"Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sa"};

	void InitRTC()
	{
		// Template

		LCD.setCursor(17, 2);
		LCD.print(':');

		LCD.setCursor(14, 3);
		LCD.print('/');
		LCD.setCursor(17, 3);
		LCD.print('/');
	}

	void RTCLoop()
	{
		// The LCD already executed begin(), so, there's no need for it
		TinyWireM.beginTransmission(RTC_I2C_ADDRESS);
		TinyWireM.write(0x00);
		TinyWireM.endTransmission();

		TinyWireM.requestFrom(RTC_I2C_ADDRESS, 7);

		/*
			byte Second 	= bcd2dec(TinyWireM.read() & 0x7f);
			byte Minute 	= bcd2dec(TinyWireM.read());
			byte Hour 		= bcd2dec(TinyWireM.read() & 0x3f);
			byte DayOfWeek 	= bcd2dec(TinyWireM.read());
			byte DayOfMonth = bcd2dec(TinyWireM.read());
			byte Month 		= bcd2dec(TinyWireM.read());

			Optimized code instead of previous one (134 flash bytes): 
		*/

		// Second
		TinyWireM.read();

		// Minute
		LCD.setCursor(18, 2);
		PrintZerofill(bcd2dec(TinyWireM.read()));

		// Hour
		LCD.setCursor(15, 2);
		PrintZerofill(TinyWireM.read() & 0x3f);

		// Day of week
		byte DayOfWeek 	= bcd2dec(TinyWireM.read());
		LCD.setCursor(12, 2);
		// If I try to write it in just one sentence, all the days are written
		LCD.print(DaysOfWeek[DayOfWeek][0]);
		LCD.print(DaysOfWeek[DayOfWeek][1]);

		// Day of month
		LCD.setCursor(12, 3);
		PrintZerofill(bcd2dec(TinyWireM.read()));

		// Month
		LCD.setCursor(15, 3);
		PrintZerofill(bcd2dec(TinyWireM.read()));

		// Year
		LCD.setCursor(18, 3);
		PrintZerofill(bcd2dec(TinyWireM.read()));
	}

	byte dec2bcd(byte dec)
	{
		return (dec / 10 * 16) + (dec % 10);
	}

	byte bcd2dec(byte bcd)
	{
		return (bcd / 16 * 10) + (bcd % 16);
	}