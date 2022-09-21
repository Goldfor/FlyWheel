/*
 * hardware_settings.h
 *
 *  Created on: Jul 26, 2022
 *      Author: Almaz
 */

#include "flash.h"
#ifndef INC_HARDWARE_SETTINGS_H_
#define INC_HARDWARE_SETTINGS_H_

#include "stdint.h"
#include <string.h>
#include "tim.h"
#include "gpio.h"

#define ChannelCount 2
#define SilentMode 1
#define ProgramMemory 1
#define UsingMinForce 1
#define CanWriteMemoryFromI2C 1

#define SetVersion 0
#define SetWriteCount 0

#define DefaultConfig \
	0x09, \
	0x38, \
	0, \
	3, \
	0, \
	0, \
	3000, \
	1, \
	233.37f, \
	1.17f, \
	0.0f, \
	0, \
	0, \
	5000, \
	0, \
	6.55f, \
	0.0f, \
	0.0f, \
	0, \
	0, \
	1, \
	0


typedef struct
{
	uint8_t Config; /*|	7|	6|	5|	4|	3|	2|	1|	0| Default = 0x09*/
					/*		 |RV1|RV2|MC1|MC2|DM1|DM2| */
	uint8_t BaseAddress; // 1
	uint8_t Reserve1; // 2
	uint8_t Clock_Setting; // 3
	/*|	7|	6|	5|	4|	3|	2|	1|	0| Default = 0x03*/
	/*		 |   |   |   |   |MulSens| */
	int16_t Need_F_ChannelSpeed; // 4
	int16_t Current_F_ChannelSpeed; // 6
	uint16_t F_MaxAbsSpeed; // 8
	uint16_t F_Min_Force; // 10
	float F_P; // 12
	float F_I; // 16
	float F_D; // 20
	int16_t Need_S_ChannelSpeed; // 24
	int16_t Current_S_ChannelSpeed; // 26
	uint16_t S_MaxAbsSpeed; // 28
	uint16_t S_Min_Force; // 30
	float S_p; // 32
	float S_I; // 36
	float S_D; // 40
	uint8_t Reserve2; // 44
	uint8_t Reserve3; // 45
	uint8_t Version; // 46
	uint8_t WriteToMemory; // 47
} MemoryMap;

extern MemoryMap DefaultSettings;
extern MemoryMap NowSettings;
void Set_Configuration(uint8_t configuration);

void Set_Channel_Raw(uint8_t channel, int32_t value);
void Calculate_Channel(uint8_t channel);
void Clear_Chanel(uint8_t channel);

void Select_Setting(MemoryMap *from, volatile MemoryMap *to);

int16_t CalculateRPM(int8_t state, int32_t counter);

#endif /* INC_HARDWARE_SETTINGS_H_ */
