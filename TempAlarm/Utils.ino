
	void ClearLCDRow(int8_t RowIndex)
	{
		LCD.setCursor(0, RowIndex);

		for(int i = LCD_COLUMNS; i > 0; i--)
			LCD.print(' ');
	}