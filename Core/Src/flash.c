
#include "flash.h"

const MemoryMap EEPROM_SETTING __attribute__ ((__section__(".eeprom")))
#if ProgramMemory
 = {
	DefaultConfig
}
#endif
;

FLASH_EraseInitTypeDef eraseSetting = {
	FLASH_TYPEERASE_PAGES,
	FLASH_BANK_1,
	0,
	1

};

uint32_t Flash_Write(uint32_t address, uint32_t* data, uint32_t len)
{
	HAL_StatusTypeDef result = HAL_OK;

	result = HAL_FLASH_Unlock();
	if (result) return result;
	result = Flash_Erase(address);
	if (result) return HAL_FLASH_GetError();
	uint32_t i = 0;
	for (uint32_t addr = address; addr < address + len; addr += 4)
	{
		result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data[i++]);
		if (result) return HAL_FLASH_GetError();
	}
	result = HAL_FLASH_Lock();
	return result;
}

HAL_StatusTypeDef Flash_Erase(uint32_t address)
{
	static uint32_t PAGEError = 0;
	eraseSetting.PageAddress = address - address % 0x400;
	return HAL_FLASHEx_Erase(&eraseSetting, &PAGEError);
}


