/**
 * Some functions used by the project
 */

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

	// This function clears only one row of the LCD
	void ClearLCDRow(int8_t RowIndex)
	{
		LCD.setCursor(0, RowIndex);

		for(int i = LCD_COLUMNS; i > 0; i--)
			LCD.print(' ');
	}