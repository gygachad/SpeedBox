#include "display_lib.h"
#include "gearbox_lib.h"
#include "tacho_lib.h"

bool g_CalibrationMode = false;
PGEAR_BOX g_pGearBox = 0;

void display_test()
{
    display_reset();
    delay(100);
    display_raw_out(1);
    delay(1000);
}

void setup()
{
  int buttonValue = 0;

  Serial.begin(9600);

  display_init();
  display_test();

  g_pGearBox = InitGearBox();
  if (!g_pGearBox)
  {
    Serial.println("GearBox initialize failed");
    while (1)
    {
      display_reset();
      delay(150);
      display_raw_out(12);//Display E
      delay(150);
    }
  }

  //InitTacho();

  byte roll[] = {253,246,191,127,239,223/*,250*/};
  int next = 0;
  
  for (int i = 0; i < 6*4; i++)
  {    
    if (g_CalibrationMode != true)
    {
      buttonValue = digitalRead(9);
      if (buttonValue == HIGH)
      {
        Serial.println("Calibration mode on");
        g_CalibrationMode = true;
      }
    }
    
    display_raw_out(roll[next]);
    delay(100);

    next++;
    if(next >= sizeof(roll))
      next = 0;
      
    Serial.println(next);
  }
}

void loop()
{
  //Calibration mode
  if (g_CalibrationMode == true)
  {
    Calibrate(g_pGearBox);

    g_CalibrationMode = false;
  }

  MeasureGear(g_pGearBox);

  //unsigned long rpm = MeasureRpm();
  //Serial.println(rpm);
}
