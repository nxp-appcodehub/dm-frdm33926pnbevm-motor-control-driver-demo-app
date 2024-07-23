/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpadc.h"
#include "frdm_mc33926_shield.h"
//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "systick_utils.h"
#include "../motor_control/mc33926_drv.h"

int setPinLevel(void)
{
	int character;
	PRINTF("\r\n Enter 1 for High and 0 for Low :- ");
	SCANF("%d",&character);
	PRINTF("%d\r\n",character);
	return character;
}

int pwm_active_channel = -1;

int setupPWMFromScratch()
{
	int dutyCycle;
	int channel;
	int freq;
	int nonPWMLevel;
	PRINTF("\r\n Enter PWM Channel Number (0 for INT1 or 1 for INT2) :- ");
	SCANF("%d",&channel);
	PRINTF("%d\r\n",channel);
	if(channel != 0 && channel != 1)
	{
		PRINTF("\r\n \033[31m Wrong channel number \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}

	PRINTF("\r\n duty cycle (0 to 100 in multiples of 10) :- ");
	SCANF("%d",&dutyCycle);
	PRINTF("%d\r\n", dutyCycle);
	if(dutyCycle < 0 ||  dutyCycle > 100 || dutyCycle % 10 != 0)
	{
		PRINTF("\r\n \033[31m Wrong duty cycle value \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}

	PRINTF("\r\n Enter Freq in HZ (1500/2000/3000/4000) :- ");
	SCANF("%d", &freq);
	PRINTF("%d\r\n", freq);
	if(freq != 1500 && freq != 2000 && freq != 3000 && freq != 4000)
	{
		PRINTF("\r\n \033[31m Wrong freq value \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}
	pwm_active_channel = channel;

	PRINTF("\r\n Enter Non PWM Channel Level (0/1) :- ");
	SCANF("%d", &nonPWMLevel);
	PRINTF("%d\r\n", nonPWMLevel);
	if(nonPWMLevel != 0 && nonPWMLevel != 1)
	{
		PRINTF("\r\n \033[31m Wrong Level value \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}

	MC33926_SetPin_PWM(channel);
	MC33926_Start_Pwm(channel, freq, dutyCycle);

	MC33926_SetPin_GPIO(1 - channel);

	if(1 - channel)
		MC33926_IN2_Pin_Write(nonPWMLevel);
	else
		MC33926_IN1_Pin_Write(nonPWMLevel);

	return SENSOR_ERROR_NONE;
}

int changeDutyCycle()
{
	int dutyCycle;
	if(pwm_active_channel < 0)
	{
		PRINTF("\r\n \033[31m Can't change Duty Cycle, No active PWM channel \033[37m\r\n");
		return SENSOR_ERROR_INVALID_PARAM;
	}
	else
	{
		PRINTF("\r\n duty cycle (0 to 100 in multiples of 10) :- ");
		SCANF("%d",&dutyCycle);
		PRINTF("%d\r\n", dutyCycle);
		if(dutyCycle < 0 ||  dutyCycle > 100 || dutyCycle % 10 != 0)
		{
			PRINTF("\r\n \033[31m Wrong duty cycle value \033[37m");
			return SENSOR_ERROR_INVALID_PARAM;
		}
	}
	MC33926_Update_Duty_Cycle(pwm_active_channel, dutyCycle);
	return SENSOR_ERROR_NONE;
}

static int InPinsWrite(void)
{
	int input;
	int level;

	int status = SENSOR_ERROR_NONE;
	PRINTF("\r\n 1. Write IN1 Pin \r\n");
	PRINTF("\r\n 2. Write IN2 Pin \r\n");
	PRINTF("\r\n Enter your choice :- ");
	SCANF("%d",&input);
	PRINTF("%d\r\n",input);
	if(input < 1 ||  input > 2)
	{
		PRINTF("\r\n \033[31m Wrong input pin \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}
	PRINTF("\r\n Enter 1 to set high and 0 to set low :-");
	SCANF("%d",&level);
	PRINTF("%d\r\n",level);
	if(level < 0 ||  level > 1)
	{
		PRINTF("\r\n \033[31m Wrong input level \033[37m");
		return SENSOR_ERROR_INVALID_PARAM;
	}
	MC33926_SetPin_GPIO(input - 1);
	if(input == 1)
	{
		MC33926_IN1_Pin_Write(level);
	}
	else
	{
		MC33926_IN2_Pin_Write(level);
	}
}

int setPWM(void)
{

	int dutyCycle;
	int character;
	int status = SENSOR_ERROR_NONE;
	PRINTF("\r\n 1.  Setup PWM channel from scratch \r\n");
	PRINTF("\r\n 2.  Change duty cycle only  \r\n");
	PRINTF("\r\n Enter your choice :- ");
	SCANF("%d",&character);
	PRINTF("%d\r\n",character);

	switch (character)
	{
	case 1:
		if(pwm_active_channel >= 0)
		{
			MC33926_Stop_Pwm(pwm_active_channel);
		}
		status = setupPWMFromScratch();
		break;
	case 2:
		status = changeDutyCycle();
		break;
	default:
		PRINTF("\r\n Invalid option...chose correct one from Main Menu\r\n");
		break;
	}
	return status;
}

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*!
 * @brief Main function
 */
int main(void)
{
	int32_t status;
	mc33926_handle_t SensorHandle;
	lpadc_config_t mLpadcConfigStruct;
	int level;
	int adcVal;
	int character;

	BOARD_InitPins();
	BOARD_InitBootClocks();
	BOARD_SystickEnable();
	BOARD_InitDebugConsole();
	LPADC_GetDefaultConfig(&mLpadcConfigStruct);
	mLpadcConfigStruct.enableAnalogPreliminary = true;
	mLpadcConfigStruct.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
	mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;
	MC33926_Init_adc(&mLpadcConfigStruct);
	PRINTF("\r\n MC33926 Motor controller Example !\r\n");
	MC33926_Initialize(&SensorHandle);


	PRINTF("\r\n Successfully Applied MC33926 Configuration\r\n");
	do
	{
		PRINTF("\r\n *********** Main Menu ***************\r\n");

		PRINTF("\r\n 1.  Set D2B pin level (Active Low) \r\n");
		PRINTF("\r\n 2.  Set D1 pin level (Active high) \r\n");
		PRINTF("\r\n 3.  Set INV pin level (Active High) \r\n");
		PRINTF("\r\n 4.  Set Slew pin level \r\n");
		PRINTF("\r\n 5.  Enable output \r\n");
		PRINTF("\r\n 6.  Disable output \r\n");
		PRINTF("\r\n 7.  Set Input (IN1 & IN2) pins level \r\n");
		PRINTF("\r\n 8.  Set PWM \r\n");
		PRINTF("\r\n 9.  Stop PWM \r\n");
		PRINTF("\r\n 10. Read feedback current \r\n");
		PRINTF("\r\n 11. Check error pin \r\n");
		PRINTF("\r\n Enter your choice :- ");
		SCANF("%d",&character);
		PRINTF("%d\r\n",character);

		switch (character)
		{
		case 1:
			PRINTF("\r\n \033[32m Set high for motor operation \r\n");
			PRINTF("\r\n \033[32m Setting low will put output in tri-state \033[37m\r\n");
			level = setPinLevel();
			if((level > 1) || (level < 0))
			{
				PRINTF("\r\n Invalid Input \r\n");
			}
			else
			{
				MC33926_D2B_Pin_Write(level);
				PRINTF("\r\n D2B pin set to %d \r\n", level);
			}
			break;

		case 2:
			PRINTF("\r\n \033[32m Set low for motor operation \r\n");
			PRINTF("\r\n Setting high will put output in tri-state \033[37m\r\n");
			level = setPinLevel();
			if((level > 1) || (level < 0))
			{
				PRINTF("\r\n Invalid Input \r\n");
			}
			else
			{
				MC33926_D1_Pin_Write(level);
				PRINTF("\r\n D1 pin set to %d \r\n", level);
			}
			break;

		case 3:
			PRINTF("\r\n \033[32m Setting this pin from Low to High  will reverse motor rotation \033[37m \r\n");
			level = setPinLevel();
			if((level > 1) || (level < 0))
			{
				PRINTF("\r\n Invalid Input \r\n");
			}
			else
			{
				MC33926_IPV_INV_Pin_Write(level);
				PRINTF("\r\n IPV pin set to %d\r\n", level);
			}
			break;

		case 4:
			level = setPinLevel();
			if((level > 1) || (level < 0))
			{
				PRINTF("\r\n Invalid Input\r\n");
			}
			else
			{
				MC33926_SLEW_Pin_Write (level);
				PRINTF("\r\n SLEW pin set to %d \r\n", level);
			}
			break;

		case 5:
			MC33926_EN_Pin_Write(1);
			PRINTF("\r\n Output Enabled", level);
			break;

		case 6:
			MC33926_EN_Pin_Write(0);
			PRINTF("\r\n Output Disabled", level);
			break;
		case 7:
			InPinsWrite();
			break;
		case 8:
			status = setPWM();
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n PWM Generation failed \r\n");
			}
			else
			{
				PRINTF("\r\n PWM Generated \r\n");
			}
			break;

		case 9:
			if(pwm_active_channel >= 0)
			{
				MC33926_Stop_Pwm(pwm_active_channel);
			}
			break;

		case 10:
			adcVal= MC33926_Read_adc();
			break;

		case 11:
			level = MC33926_SF_B_Pin_Read();
			if(!level)
			{
				PRINTF("\r\n \033[31m Error detected \033[37m \r\n");
			}
			else
			{
				PRINTF("\r\n \033[32m NO error detected \033[37m \r\n");
			}
			break;

		default:
			PRINTF("\r\n Invalid option...choose correct one from Main Menu\r\n");
			break;
		}
		PRINTF("\r\n Please press enter to goto main menu");
		GETCHAR();
	}
	while(1);
	return 0;
}
