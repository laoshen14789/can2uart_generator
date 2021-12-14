#ifndef __GENERATOR_H
#define __GENERATOR_H

#include <stdint.h>
#pragma pack(1)
typedef struct
{
	struct
	{
		uint8_t ExtenderVersion;
		uint8_t ExtenderBrand1;
		uint8_t ExtenderBrand2;
		uint8_t ExtenderBrand3;
		uint8_t ExtenderSN_YY;
		uint8_t ExtenderSN_MM;
		uint8_t ExtenderSN_DD;
		uint8_t ExtenderSN_SS;
	} generator0_s;

	struct
	{
		uint16_t EngineSpeed;
		uint16_t ThrottlePosition;
		uint8_t FuelPosition;
		uint8_t CylinderTemp;
		uint8_t CoolantTemp;
		uint8_t EngineState;
	} generator1_s;

	struct
	{
		uint16_t OutputVoltage;
		uint16_t OutputCurrent;
		uint16_t ExtenderAlarm;
		uint8_t Save;
		uint8_t Fault;

	} generator2_s;

	struct
	{
		uint8_t ExtenderControl;
		uint8_t ExtenderSetting;
		uint16_t SettingParameter;
		uint16_t Save;
		uint8_t StatusCount;
		uint8_t CheckSum;

	} generator3_s;

	struct
	{
		uint32_t TotalRunTime;
		uint16_t TimeForService;
		uint16_t TimeForLocking;

	} generator4_s;

} generator_info_s;

typedef struct
{

	uint8_t headermagic1;
	uint8_t headermagic2;
	uint16_t version;
	uint8_t runtime_minutes;
	uint8_t runtime_seconds;
	uint16_t runtime_hours;
	uint16_t seconds_until_maintenance_high;
	uint16_t seconds_until_maintenance_low;
	uint16_t errors;
	uint16_t rpm;
	uint16_t throttle;
	uint16_t idle_throttle;
	uint16_t output_voltage;
	uint16_t output_current;
	uint16_t dynamo_current;
	uint8_t unknown1;
	uint8_t mode;
	uint8_t unknown6[38]; // "data"?!
	uint16_t checksum;
	uint8_t footermagic1;
	uint8_t footermagic2;
}generator_data_s;
#pragma pack()

void send_generator(void);
void update_generator_data(void);

#endif

