#include <Arduino.h>

#define DATA_PIN  3
#define CLOCK_PIN 4
#define LATCH_PIN 5

//#define DEBUG

void display_init()
{
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(LATCH_PIN, OUTPUT);
	pinMode(DATA_PIN, OUTPUT);
}

void display_raw_out(byte b)
{
	
#ifdef DEBUG
	Serial.println(b);
#endif

	// устанавливаем синхронизацию "защелки" на LOW
	digitalWrite(LATCH_PIN, LOW);
	// передаем последовательно на dataPin
	shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b); 

	//"защелкиваем" регистр, тем самым устанавливая значения на выходах
	digitalWrite(LATCH_PIN, HIGH);
}

void display_reset()
{
	display_raw_out(0xff);
}
