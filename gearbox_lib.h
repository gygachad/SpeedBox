#define TRY_AMOUNT 20
#define AHTUNG_AMOUNT 15

#define BIN_PEAK 255

#define DELTA 60

typedef struct _GEAR_VALUE
{
  unsigned int voltage;
  unsigned int gear_num;
}GEAR_VALUE, *PGEAR_VALUE;

typedef struct _GEAR_BOX
{
  bool initialized;
  int CurrentGear;
  _GEAR_VALUE gear_value_array[7];
}GEAR_BOX, *PGEAR_BOX;

int get_abs_analog_value(int port, int aqqurate);

PGEAR_BOX InitGearBox();

void Calibrate(PGEAR_BOX pGearBox);
void MeasureGear(PGEAR_BOX pGearBox);

bool LoadGearing(PGEAR_BOX pGear_values);
void ChangeGear(int gear);

int CalibrateGear(PGEAR_BOX pGearBox, int gear_num);

void write_eeprom(byte* in_buffer, int buffer_size);
void read_eeprom(byte* out_buffer, int buffer_size);
