	// LCD
	#include <SerialLCD.h>
	#include <SoftwareSerial.h>

	// DS18B20 (DS1820 and DS1820 should work too)
	#include <OneWire.h>
	#include <DallasTemperature.h>

	// Using one analog input for read multiple pushbutons
	#include <OneWireSwitches.h>

	// LCD configuration
		#define LCD_RX_PIN 0
		#define LCD_TX_PIN 1

	// Temperature sensor configuration
		#define ONE_WIRE_BUS 3 // Disturbs the SPI signals while programming if connected at pint 2

	// User interface configuration
		#define AFTER_WELCOME_DELAY 1000 // How many time we'll wait before start working

		#define HIGHEST_TEMPERATURE 100 // Up to 127
		#define LOWEST_TEMPERATURE -20 // Up to -127

		#define START_TEMPERATURE 25 // Between -127 and 127

		#define TEMP_BUTTON_THRESHOLD 50
		#define TEMP_BUTTON_DELAY 50

		#define START_STOP_BUTTON_THRESHOLD 150
		#define START_STOP_BUTTON_DELAY 500

		#define CHANGE_MODE_BUTTON_THRESHOLD 50
		#define CHANGE_MODE_BUTTON_DELAY 500

		#define BLINKING_CHARACTER_DELAY 500

		#define BUZZER_PIN 2

	// OneWireSwitches configuration
		#define SWITCHES_INPUT_PIN A2

		#define SWITCHES_AMOUNT 4

		#define SWITCHES_GROUND_RESISTOR 10000 // 10K resistor (tied from SWITCHES_INPUT_PIN to ground)
		#define SWITCHES_RESISTOR_TOLERANCE 100 // 10%, % resistor's tolerance multiplied by 10

		// Resistors array indexes
		#define TEMP_PLUS_ID 0
		#define TEMP_MINUS_ID 1
		#define START_STOP_ID 2
		#define CHANGE_MODE_ID 3

		// Resistor values (tied to VCC)
		const uint32_t SwitchesVCCResistors[SWITCHES_AMOUNT]
		{
			100000,	// 100K
			22000,	// 22K
			10000,	// 10K
			1000	// 1K
		};

	// This variable holds the temp that must be reached before alarm activation
	int8_t AlarmTemperature = START_TEMPERATURE;

	bool AlarmEnabled = false;
	bool AlarmReverse = false;

	bool BlinkingCharacterEnabled = false;
	unsigned long BlinkingCharacterUntil;

	// Instance LCD library
	SerialLCD LCD(LCD_RX_PIN, LCD_TX_PIN);

	// Instance OneWire and DallasTemperature
	OneWire OneWireBus(ONE_WIRE_BUS);
	DallasTemperature Sensors(&OneWireBus);

	// Instance OneWireSwitches
	OneWireSwitches Switches(SWITCHES_INPUT_PIN, SWITCHES_AMOUNT, SwitchesVCCResistors, SWITCHES_GROUND_RESISTOR, SWITCHES_RESISTOR_TOLERANCE);

	void setup()
	{
		// Configure Buzzer Pin

		pinMode(BUZZER_PIN, OUTPUT);

		// Init LCD

		delay(100);

		LCD.begin();
		LCD.backlight();

		// Init OneWire and DallasTemperature
		Sensors.begin();

		// Allows us to perform readings while the sensor is reading
		Sensors.setWaitForConversion(false);

		// Start a reading, we want to have a stable value as soon as possible
		Sensors.requestTemperatures(); // Sensors.requestTemperaturesByIndex(0); uses around 200 more bytes of flash

		// Print some info
		// Can be commented if you need more flash for improvements

		LCD.home();
		LCD.print(" TempAlarm v0.1");
		LCD.setCursor(0, 1);
		LCD.print("  By @fermino");

		delay(AFTER_WELCOME_DELAY);

		// Write some info characters

		LCD.clear();
		LCD.home(); // Needed, else the character get written in the first row

		LCD.setCursor(15, 0);
		LCD.print(45);

		LCD.setCursor(15, 1);
		LCD.print(AlarmReverse ? 60 : 62); // ">" or "<"
	}

	void loop()
	{
		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(ReadPulse(TEMP_PLUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD)
		{
			if(AlarmTemperature < HIGHEST_TEMPERATURE)
				AlarmTemperature++;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_MINUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD)
		{
			if(AlarmTemperature > LOWEST_TEMPERATURE)
				AlarmTemperature--;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(START_STOP_ID, START_STOP_BUTTON_THRESHOLD) >= START_STOP_BUTTON_THRESHOLD)
		{
			if(AlarmEnabled)
			{
				digitalWrite(BUZZER_PIN, LOW);

				AlarmEnabled = false;
				
				LCD.setCursor(15, 0);
				LCD.print(45);
			}
			else
			{
				AlarmEnabled = true;
				
				ResetBlinkingCharacter(false);
			}

			delay(START_STOP_BUTTON_DELAY);
		}
		else if(ReadPulse(CHANGE_MODE_ID, CHANGE_MODE_BUTTON_THRESHOLD) >= CHANGE_MODE_BUTTON_THRESHOLD)
		{
			AlarmReverse = !AlarmReverse;

			LCD.setCursor(15, 1);
			LCD.print(AlarmReverse ? 60 : 62); // ">" or "<"

			delay(CHANGE_MODE_BUTTON_DELAY);
		}

		// Show Alarm Temperature

		LCD.setCursor(0, 0);
		LCD.print("Alarma: ");
		if(AlarmTemperature < 0)
			LCD.print("-");
		LCD.print((unsigned long) abs(AlarmTemperature), DEC);
		LCD.print(" ");
		LCD.print(223);
		LCD.print("C ");

		// Show current temperature at sensor

		float Temperature = Sensors.getTempCByIndex(0);

		if(Temperature != DEVICE_DISCONNECTED_C)
		{
			LCD.setCursor(0, 1);
			LCD.print("Temp: ");
			if(Temperature < 0)
				LCD.print("-");
			LCD.print((unsigned long) abs(Temperature), DEC);
			LCD.print(".");
			LCD.print((Temperature - (long) Temperature) * 10, DEC);
			LCD.print(" ");
			LCD.print(223);
			LCD.print("C ");
		}
		else
		{
			LCD.clear();
			LCD.home();
			LCD.print("Sensor");
			LCD.setCursor(0, 1);
			LCD.print("    disconnected");

			while(1);
		}

		// Do some alarm things (?

		if(AlarmEnabled)
		{
			// Write some status info in the LCD

			if(millis() >= BlinkingCharacterUntil)
			{
				LCD.setCursor(15, 0);

				LCD.print(BlinkingCharacterEnabled ? 33 : ' ');

				ResetBlinkingCharacter(!BlinkingCharacterEnabled);
			}

			// If the threshold is obove or below the setted temperature, activate the alarm

			if(!AlarmReverse && Temperature >= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
			
			if(AlarmReverse && Temperature <= AlarmTemperature)
				digitalWrite(BUZZER_PIN, HIGH);
		}

		// Perform another reading

		Sensors.requestTemperatures();
	}

	// Reads a pulse from a button and returns the time since the press
	// Returns 0 if the button is not pressed
	uint16_t ReadPulse(uint8_t KeyIndex, uint16_t Timeout)
	{
		uint16_t Time = 0;

		while(Switches.readKey(KeyIndex) == HIGH && Time < Timeout)
		{
			Time++;
			delay(1);
		}

		return Time;
	}

	void ResetBlinkingCharacter(bool Enabled)
	{
		BlinkingCharacterEnabled = Enabled;
		BlinkingCharacterUntil = millis() + BLINKING_CHARACTER_DELAY;
	}