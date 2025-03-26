/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(uint8_t address)
{

  /* USER CODE BEGIN I2C1_Init 0 */

#if 0
  /* Please change
   * void MX_I2C1_Init(void) to void MX_I2C1_Init(uint8_t address)
   * hi2c1.Init.OwnAddress1 = xxx; to hi2c1.Init.OwnAddress1 = address << 1;
   * Add after USER CODE END I2C1_Init 0
   * #endif
   */
  /* USER CODE END I2C1_Init 0 */
#endif
  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = address << 1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_I2C1_ENABLE();

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

volatile uint8_t first = 1;
volatile uint8_t offset = 0;
volatile uint8_t MOC = 0;
#define ram ((uint8_t *) &DefaultSettings)

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	first = 1;
	HAL_I2C_EnableListen_IT(hi2c); // slave is ready again
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	uint8_t* addres = (offset < sizeof(MemoryMap)) ? &ram[offset] : (uint8_t *)&MOC;

	if(TransferDirection==I2C_DIRECTION_TRANSMIT)
	{
		if(first)
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t *)&offset, 1, I2C_NEXT_FRAME);
		}
		else
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, addres, 1, I2C_NEXT_FRAME);
		}
	}
	else
	{
		HAL_I2C_Slave_Seq_Transmit_IT(hi2c, addres, 1, I2C_NEXT_FRAME);
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(first)
	{
		first = 0;
		Select_Setting(&NowSettings, &DefaultSettings);
		DefaultSettings.WriteToMemory = 0;
	}
	else
	{
		offset++;
	}
	uint8_t* addres = (offset < sizeof(MemoryMap)) ? &ram[offset] : (uint8_t *)&MOC;
	HAL_I2C_Slave_Seq_Receive_IT(hi2c, addres, 1, I2C_NEXT_FRAME);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	offset++;
	uint8_t* addres = (offset < sizeof(MemoryMap)) ? &ram[offset] : (uint8_t *)&MOC;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, addres, 1, I2C_NEXT_FRAME);
}

#ifdef DEBUG

typedef struct  {
	uint32_t error;
	HAL_I2C_ModeTypeDef mode;
	HAL_I2C_StateTypeDef state;
} ErrorLogEntry;

ErrorLogEntry errorLog[100];
ErrorLogEntry* cursor = errorLog;

void ErrorLogCursor_Next() {
	if (++cursor == (errorLog + sizeof(errorLog) / sizeof(ErrorLogEntry)))
		cursor = errorLog;
}

void ErrorLogAdd(I2C_HandleTypeDef* hi2c) {
	cursor->error = HAL_I2C_GetError(hi2c);
	cursor->mode = HAL_I2C_GetMode(hi2c);
	cursor->state = HAL_I2C_GetState(hi2c);
	ErrorLogCursor_Next();
}

#endif

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
#ifdef DEBUG
	ErrorLogAdd(hi2c);
#endif
	const uint32_t error = HAL_I2C_GetError(hi2c);
	if(error & HAL_I2C_ERROR_AF)
	{
		offset--;
		DefaultSettings.Current_F_ChannelSpeed = NowSettings.Current_F_ChannelSpeed;
		DefaultSettings.Current_S_ChannelSpeed = NowSettings.Current_S_ChannelSpeed;
#if CanWriteMemoryFromI2C
		uint8_t writeToMemory = DefaultSettings.WriteToMemory != 0;
#endif
		DefaultSettings.WriteToMemory = 0;
		if (DefaultSettings.F_D != NowSettings.F_D || DefaultSettings.F_I != NowSettings.F_I)
		{
			Clear_Chanel(0);
		}
		if (DefaultSettings.S_D != NowSettings.S_D || DefaultSettings.S_I != NowSettings.S_I)
		{
			Clear_Chanel(1);
		}
		Select_Setting(&DefaultSettings, &NowSettings);
		Set_Configuration();
#if CanWriteMemoryFromI2C
		if(writeToMemory)
		{
			DefaultSettings.Need_F_ChannelSpeed = 0;
			DefaultSettings.Need_S_ChannelSpeed = 0;
			DefaultSettings.Current_F_ChannelSpeed = 0;
			DefaultSettings.Current_S_ChannelSpeed = 0;
			Select_Setting(&DefaultSettings, (MemoryMap *)&EEPROM_SETTING);
			HAL_NVIC_SystemReset();
		}
#endif
	}
	if (error & HAL_I2C_ERROR_BERR) {
		HAL_I2C_DisableListen_IT(hi2c);
		HAL_I2C_DeInit(hi2c);
		HAL_I2C_Init(hi2c);
		HAL_I2C_EnableListen_IT(hi2c);
	}
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
}


/* USER CODE END 1 */
