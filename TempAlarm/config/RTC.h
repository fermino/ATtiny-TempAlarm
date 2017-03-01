	/**
	 * Module: RTC
	 * ===========
	 */

		//#define RTC_HW DS3231
		#define RTC_I2C_ADDRESS 0x68

		#define RTC_DAY_0 "Dom\0"
		#define RTC_DAY_1 "Lun\0"
		#define RTC_DAY_2 "Mar\0"
		#define RTC_DAY_3 "Mie\0"
		#define RTC_DAY_4 "Jue\0"
		#define RTC_DAY_5 "Vie\0"
		#define RTC_DAY_6 "Sab\0"

		#define RTC_MONTH_0		"ene\0"
		#define RTC_MONTH_1		"feb\0"
		#define RTC_MONTH_2		"mar\0"
		#define RTC_MONTH_3		"abr\0"
		#define RTC_MONTH_4		"may\0"
		#define RTC_MONTH_5		"jun\0"
		#define RTC_MONTH_6		"jul\0"
		#define RTC_MONTH_7		"ago\0"
		#define RTC_MONTH_8		"sep\0"
		#define RTC_MONTH_9		"oct\0"
		#define RTC_MONTH_10	"nov\0"
		#define RTC_MONTH_11	"dic\0"

	/**
	 * LCD configuration
	 */

		#define RTC_DEGREE_CHAR ((char) 223)

	/**
	 * OneWireSwitches configuration
	 */

		#define RTC_BUTTON_CONFIGURE_ID TEMPERATURE_BUTTON_DIRECTION_ID
		#define RTC_BUTTON_CONFIGURE_BUZZER_TIME 5

		#define RTC_BUTTON_MINUS_ID TEMPERATURE_BUTTON_MINUS_ID
		#define RTC_BUTTON_MINUS_THRESHOLD 100

		#define RTC_BUTTON_PLUS_ID TEMPERATURE_BUTTON_PLUS_ID
		#define RTC_BUTTON_PLUS_THRESHOLD 100

		#define RTC_BUTTON_NEXT_ID TEMPERATURE_BUTTON_STARTSTOP_ID
		#define RTC_BUTTON_NEXT_THRESHOLD 50
		#define RTC_BUTTON_NEXT_DELAY 150
		#define RTC_BUTTON_NEXT_BUZZER_TIME 5