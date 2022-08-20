#include <Arduino.h>

#define  FQ_PIN  2 // Для ATMega32 только 2 или 3.
const int K = 1;
const unsigned long maxFq = 20000; // rpm (оборотов в минуту)
const unsigned long minInterval = 1000000UL / ( K * maxFq ); // минимальный интервал в мкс 

volatile unsigned long counter; // Количество отсчётов.
volatile unsigned long mks;     // Время последнего отсчёта.

unsigned long oldTime;  // Время последнего отсчёта в предыдущем вычислении.

// Функция для обработки прерывания.
void InterruptRoutine()
{
  // Здесь код прерывания
  static unsigned long oldTmr; // сохраняем старый таймер
  unsigned long tmr=micros();
  //if (tmr - oldTmr > minImterval) 
  {
    mks=micros(); 
    counter++;
    oldTmr=tmr;  
  }
}

void InitTacho()
{
	attachInterrupt(digitalPinToInterrupt(FQ_PIN), InterruptRoutine, RISING);
}

unsigned long MeasureRpm()
{
	unsigned long rpm;
	// Получаем данные. 
	noInterrupts();
	unsigned long cnt = counter;
	counter = 0;
	unsigned long tmr = mks;
	interrupts();
	
	// Выводим частоту в оборотах в секунду
	if (cnt > 0) 
	{
		rpm = K * 1000000UL / ((tmr - oldTime) / cnt);
		oldTime = tmr;
	} 
	else 
	{
		rpm = 0;
	}
	
	return rpm;
}