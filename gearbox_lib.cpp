#include <Arduino.h>
#include <EEPROM.h>
#include "display_lib.h"
#include "gearbox_lib.h"

//GEAR_VALUES g_gear_values;

byte g_binValues[1024]; // значения бинов

//int g_CurrentGear = -1;

void write_eeprom(byte* in_buffer, int buffer_size)
{
	for(int i = 0; i < buffer_size; i++)
		EEPROM.write(i, in_buffer[i]);
}

void read_eeprom(byte* out_buffer, int buffer_size)
{
	for(int i = 0; i < buffer_size; i++)
		out_buffer[i] = EEPROM.read(i);
}

int get_abs_analog_value(int port, int aqqurate)
{
	unsigned int analog_value = 0;

	for(int i = 0; i < aqqurate + 1; i++)
	{
		for(int i = 0; i < 1023; i++) g_binValues[i] = 0;

		while(true)
		{
			unsigned int value = analogRead(port);
			//Serial.println(value);
			g_binValues[value] += 1;

			if(g_binValues[value] == BIN_PEAK)
			{
				analog_value += value;
				break;
			}
		}
	}

	return analog_value/(aqqurate + 1);
}

int CalibrateGear(PGEAR_BOX pGearBox, int gear_num)
{
	int temp_value = 0;
	
	for(int i = 0; i < AHTUNG_AMOUNT; i++)
	{
		delay(100);
		display_reset();
		delay(100);
		switch(gear_num)
		{
			//Neutral
			case 0:
			{
			  display_raw_out(GEAR_N);
			  break;
			}
			case 1:
			{
			  display_raw_out(GEAR_1);
			  break;
			}
			case 2:
			{
			  display_raw_out(GEAR_2);
			  break;
			}
			case 3:
			{
			  display_raw_out(GEAR_3);
			  break;
			}
			case 4:
			{
			  display_raw_out(GEAR_4);
			  break;
			}
			case 5:
			{
			  display_raw_out(GEAR_5);
			  break;
			}
			case 6:
			{
			  display_raw_out(GEAR_6);
			  break;
			}
			default:
			{
				break;
			}
		}
	}
	
	return get_abs_analog_value(0, TRY_AMOUNT);
}

void Calibrate(PGEAR_BOX pGearBox)
{  
	Serial.println("Start calibrate");
 
 	int gears[] = {1,0,2,3,4,5,6};
	for(int i = 0; i < 7; i++)
	{
		Serial.println(gears[i]);

		int abs_value = CalibrateGear(pGearBox, gears[i]);
    
		pGearBox->gear_value_array[i].gear_num = gears[i];
		pGearBox->gear_value_array[i].voltage = abs_value;
    
		Serial.println(abs_value);
	}

  pGearBox->initialized = true;

  write_eeprom((byte*)pGearBox, sizeof(GEAR_BOX));
  
  Serial.println("End calibrate");
  
  LoadGearing(pGearBox);
}

bool LoadGearing(PGEAR_BOX pGearBox)
{
  read_eeprom((byte*)pGearBox, sizeof(GEAR_BOX));

  //В памяти пусто
  if(pGearBox->initialized == false)
  {
    //6 - 960
    //5 - 893
    //4 - 747
    //3 - 614
    //2 - 464
    //N - 1023
    //1 - 368
    pGearBox->gear_value_array[0].voltage  = 368;
    pGearBox->gear_value_array[0].gear_num = 1;
    pGearBox->gear_value_array[1].voltage  = 1023;
    pGearBox->gear_value_array[1].gear_num = 0;
    pGearBox->gear_value_array[2].voltage  = 464;
    pGearBox->gear_value_array[2].gear_num = 2;
    pGearBox->gear_value_array[3].voltage  = 614;
    pGearBox->gear_value_array[3].gear_num = 3;
    pGearBox->gear_value_array[4].voltage  = 747;
    pGearBox->gear_value_array[4].gear_num = 4;
    pGearBox->gear_value_array[5].voltage  = 893;
    pGearBox->gear_value_array[5].gear_num = 5;
    pGearBox->gear_value_array[6].voltage  = 960;
    pGearBox->gear_value_array[6].gear_num = 6;
    
    Serial.println("Default gearing load");
  }
  else
    Serial.println("Saved gearing detected");
    
  return pGearBox->initialized;
}

void ChangeGear(int gear)
{
	switch(gear)
	{
		//Neutral
		case 0:
		{
		  display_raw_out(GEAR_N);
		  break;
		}
		case 1:
		{
		  display_raw_out(GEAR_1);
		  break;
		}
		case 2:
		{
		  display_raw_out(GEAR_2);
		  break;
		}
		case 3:
		{
		  display_raw_out(GEAR_3);
		  break;
		}
		case 4:
		{
		  display_raw_out(GEAR_4);
		  break;
		}
		case 5:
		{
		  display_raw_out(GEAR_5);
		  break;
		}
		case 6:
		{
		  display_raw_out(GEAR_6);
		  break;
		}
		default:
		{
			break;
		}
	}
}

PGEAR_BOX InitGearBox()
{
	PGEAR_BOX pGearBox = malloc(sizeof(GEAR_BOX));
	if(!pGearBox)
		return 0;
	
	for(int i = 0; i < 1023; i++) 
		g_binValues[i] = 0;
	
	//Загрузка сохраненных значений
	bool result = LoadGearing(pGearBox);
	
	return pGearBox;
}

void MeasureGear(PGEAR_BOX pGearBox)
{
	int analogValue = get_abs_analog_value(0, 0); // читаем значение на аналоговом входе
	//int analogValue = analogRead(0); // читаем значение на аналоговом входе

	//6 - 960
	//5 - 893
	//4 - 747
	//3 - 614
	//2 - 464
	//N - 1023
	//1 - 368
	int gear = -1;
	//Serial.println(analogValue); // выводим его в последовательный порт

	for(int i = 0; i < 7; i++)
	{
		if( (analogValue - DELTA) < pGearBox->gear_value_array[i].voltage && 
			(analogValue + DELTA) > pGearBox->gear_value_array[i].voltage)
			{
				gear =  pGearBox->gear_value_array[i].gear_num;
				break;
			}
	}

	if(gear != pGearBox->CurrentGear)
	{
		//Serial.println(gear);
		ChangeGear(gear);
		pGearBox->CurrentGear = gear;
	}
}
