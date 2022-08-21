/*
 * flash.h
 *
 *  Created on: Jul 29, 2022
 *      Author: Almaz
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32f1xx.h"
#include "hardware_settings.h"


#define START_FLASH 0x08000000

extern const MemoryMap EEPROM_SETTING;

uint32_t Flash_Write(uint32_t address, uint32_t* data, uint32_t len);

HAL_StatusTypeDef Flash_Erase(uint32_t address);

#endif /* INC_FLASH_H_ */
