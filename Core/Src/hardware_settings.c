#include "hardware_settings.h"

void Set_Configuration(uint8_t configuration)
{
	/**
	 * Ограничение на минимальное измерение в 10 оборотов в минуту
	 * Если хотим другое
	 * Считаем по формуле 60000 / обороты в минунту
	 */
	if (configuration & (1 << 3))
	{
		__HAL_TIM_SET_AUTORELOAD(&htim3, 3000); // Рассчитанное значение / 2
		__HAL_TIM_SET_AUTORELOAD(&htim4, 3000); // Рассчитанное значение / 2
	}
	else
	{
		__HAL_TIM_SET_AUTORELOAD(&htim3, 6000); // Рассчитанное значение
		__HAL_TIM_SET_AUTORELOAD(&htim4, 6000); // Рассчитанное значение
	}
}


void Set_Channel_Raw(uint8_t channel, int32_t value)
{
	uint32_t absValue = abs(value);
	GPIO_PinState polarity = value > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	if (absValue >= 0x7FFF)
	{
		absValue = 0x7FFF;
	}
	else
	{
		absValue = absValue << 1;
	}
	uint8_t revers = (NowSettings.Config >> (5 - channel)) & 1;
	switch (channel) {
		case 0:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, absValue);

			HAL_GPIO_WritePin(POLARITY_MOTOR_GPIO_Port, POLARITY_MOTOR_Pin, polarity ^ revers);
			break;
		case 1:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, absValue);
			HAL_GPIO_WritePin(POLARITY_COIL_GPIO_Port, POLARITY_COIL_Pin, polarity ^ revers);
			break;
		default:
			break;
	}
}

int32_t lastError[ChannelCount] = {0};
float Integral[ChannelCount] = {0};
void Calculate_Channel(uint8_t channel)
{
	uint8_t DM = NowSettings.Config & (channel == 0 ? 0x02 : 0x01);
	int16_t *NeedSpeed = channel == 0 ? &NowSettings.Need_F_ChannelSpeed : &NowSettings.Need_S_ChannelSpeed;
	if (DM)
	{
		Set_Channel_Raw(channel, *NeedSpeed);
	}
	else
	{
		int16_t *CurrentSpeed = NeedSpeed + 1;
		uint16_t *MaxAbsSpeed = CurrentSpeed + 1;
		uint16_t *Min_Force = MaxAbsSpeed + 1;
		if (*NeedSpeed > *MaxAbsSpeed)
		{
			*NeedSpeed = *MaxAbsSpeed;
		}
		if (*NeedSpeed < -(*MaxAbsSpeed))
		{
			*NeedSpeed = -(*MaxAbsSpeed);
		}
		float *Kp = Min_Force + 1, *Ki = Kp + 1, *Kd = Ki + 1;

		int32_t error = (1000.0/950)*(*NeedSpeed) - *CurrentSpeed;
//						 ^ компенсация трения

		float Prop = *Kp * error;

		float Dif = 0;//*Kd * (error - lastError[channel]);
		lastError[channel] = error;

#if SilentMode
		if (error == 0 && *NeedSpeed == 0)
		{
			Integral[channel] = 0;
		}
#endif
		float val = Prop + Integral[channel] + Dif;
		if (abs(*NeedSpeed) < 10 && abs(*CurrentSpeed) < 200)
		{
			val = 0;
		}
#if UsingMinForce
		else
		{
			val += val > 0 ? *Min_Force : -(*Min_Force);
		}
#endif
		if(val < 32766 && val > -32766)
		{
			Integral[channel] = Integral[channel] + (*Ki * error);
		}
		if(val > 32766) val = 32766;
		if(val < -32766) val = -32766;
		Set_Channel_Raw(channel, (int32_t)val);
	}
}

void Clear_Chanel(uint8_t channel)
{
	Integral[channel] = 0;
	lastError[channel] = channel == 0 ? NowSettings.Current_F_ChannelSpeed : NowSettings.Current_S_ChannelSpeed;
}

void Select_Setting(MemoryMap *from, volatile MemoryMap *to)
{
	if ((uint32_t)to < 0x20000000)
	{
		Flash_Write((uint32_t)to, (uint32_t *)from, sizeof(MemoryMap));
	}
	else
	{
		memcpy(to, from, sizeof(MemoryMap));
	}
}

MemoryMap DefaultSettings =
{
	DefaultConfig
};

MemoryMap NowSettings = {};

