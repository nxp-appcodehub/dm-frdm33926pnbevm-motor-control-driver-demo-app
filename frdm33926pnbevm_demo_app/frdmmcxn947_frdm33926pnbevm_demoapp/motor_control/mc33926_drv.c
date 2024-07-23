/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  mc33926_drv.c
 * @brief The mc33926_drv.c file implements the mc33926 functional interface.
 */

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "mc33926_drv.h"
#include "systick_utils.h"
#include "fsl_pwm.h"
#include "issdk_hal.h"
#include "fsl_debug_console.h"
#define DEMO_PWM_FAULT_LEVEL true
#define ADC_WAIT_TIME_IN_ms 10

lpadc_conv_result_t g_LpadcResultConfigStruct;
volatile bool g_LpadcConversionCompletedFlag = false;
volatile uint32_t g_LpadcInterruptCounter    = 0U;
const uint32_t g_LpadcFullRange   = 65536U;
const uint32_t g_LpadcResultShift = 0U;

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;
//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
int32_t MC33926_Initialize(
		mc33926_handle_t *pSensorHandle)
{
	int32_t status;
	uint8_t reg;
	if(!pSensorHandle)
	{

	}
	pSensorHandle->isInitialized = true;

	gpioDriver->pin_init(&MC33926_D1, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_D2B, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_EN, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_IPV_INV, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_SLEW, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_SF_B, GPIO_DIRECTION_IN, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_IN1, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	gpioDriver->pin_init(&MC33926_IN2, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

	return SENSOR_ERROR_NONE;
}

void DEMO_LPADC_IRQ_HANDLER_FUNC(void)
{
	g_LpadcInterruptCounter++;

	if (LPADC_GetConvResult(DEMO_LPADC_BASE, &g_LpadcResultConfigStruct, 0))
	{
		g_LpadcConversionCompletedFlag = true;
	}
	SDK_ISR_EXIT_BARRIER;
}

int32_t MC33926_Init_adc(lpadc_config_t *mLpadcConfigStruct)
{

	lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
	lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* attach FRO HF to ADC0 */
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

	LPADC_Init(DEMO_LPADC_BASE, mLpadcConfigStruct);
	LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
	LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
	/* Set conversion CMD configuration. */
	LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
	mLpadcCommandConfigStruct.channelNumber = DEMO_LPADC_USER_CHANNEL;
	mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
	LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);
	/* Set trigger configuration. */
	LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
	mLpadcTriggerConfigStruct.targetCommandId       = DEMO_LPADC_USER_CMDID; /* CMD15 is executed. */
	mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
	LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */
	LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFOWatermarkInterruptEnable);
	EnableIRQ(DEMO_LPADC_IRQn);
}

int32_t MC33926_Read_adc(void)
{
	int i = 0;
	PRINTF("Press Enter to start ADC conversion");
	GETCHAR();
	LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
	while ((!g_LpadcConversionCompletedFlag) && (i<=ADC_WAIT_TIME_IN_ms))
	{
		BOARD_DELAY_ms(500);
		i++;
	}
	if(i>10){
		PRINTF("\r\n ADC conversion failed! \r\n");
	}
	else{
	PRINTF("\r\n ADC value: %d\r\n ADC interrupt count: %d\r\n",
			((g_LpadcResultConfigStruct.convValue) >> g_LpadcResultShift), g_LpadcInterruptCounter);
	g_LpadcConversionCompletedFlag = false;

	return g_LpadcResultConfigStruct.convValue;
	}
}

static void MC33926_PWM_DRV_InitPwm_1(int32_t num, int freq, int dutyCycle)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCE;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 50; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState0;
    pwmSignal[0].pwmchannelenable = true;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level      = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 50;
    pwmSignal[1].deadtimeValue    = deadTimeVal;
    pwmSignal[1].faultState       = kPWM_PwmFaultState0;
    pwmSignal[1].pwmchannelenable = true;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
#ifdef DEMO_PWM_CLOCK_DEVIDER
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz / (1 << DEMO_PWM_CLOCK_DEVIDER));
#else
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
#endif

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
#ifdef DEMO_PWM_CLOCK_DEVIDER
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz / (1 << DEMO_PWM_CLOCK_DEVIDER));
#else
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
#endif
}
static void MC33926_PWM_DRV_InitPwm(int32_t num, int freq, int dutyCycle)
{
	uint16_t deadTimeVal;
	pwm_signal_param_t pwmSignal[3];
	uint32_t pwmSourceClockInHz;
	uint32_t pwmFrequencyInHz = freq;
	pwm_submodule_t submod;

	pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

	/* Set deadtime count, we set this to about 650ns */
	deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

	if(num)
		submod = kPWM_Module_2;
	else
		submod = kPWM_Module_0;
	pwmSignal[0].pwmChannel       = kPWM_PwmB;
	pwmSignal[0].level            = kPWM_HighTrue;       /* Should be low true when edge aligned PWM. */

	pwmSignal[0].dutyCyclePercent = dutyCycle;

	pwmSignal[0].faultState       = kPWM_PwmFaultState0;
	pwmSignal[0].pwmchannelenable = true;

	/*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
	PWM_SetupPwm(BOARD_PWM_BASEADDR, submod, pwmSignal, 1, kPWM_EdgeAligned, pwmFrequencyInHz,
			pwmSourceClockInHz);

}

int32_t MC33926_Update_Duty_Cycle(int32_t num, int32_t dutyCycle)
{
	if (num == 0)
	{
		PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB, kPWM_EdgeAligned, dutyCycle);
	}
	else
	{
		PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB, kPWM_EdgeAligned, dutyCycle);
	}
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_2, true);
}

int32_t MC33926_SetPin_PWM(int32_t channel)
{

	PORT_Type *ports[] = PORT_BASE_PTRS;
	PORT_Type port;
	int portNum;
	int pinNum;

	port_pin_config_t MC33926_CONFIG = {/* Internal pull-up/down resistor is disabled */
			kPORT_PullDisable,
			/* Low internal pull resistor value is selected. */
			kPORT_LowPullResistor,
			/* Fast slew rate is configured */
			kPORT_FastSlewRate,
			/* Passive input filter is disabled */
			kPORT_PassiveFilterDisable,
			/* Open drain output is disabled */
			kPORT_OpenDrainDisable,
			/* Low drive strength is configured */
			kPORT_LowDriveStrength,
			/* Pin is configured as P3_12 */
			kPORT_MuxAlt5,
			/* Digital input enabled */
			kPORT_InputBufferEnable,
			/* Digital input is not inverted */
			kPORT_InputNormal,
			/* Pin Control Register fields [15:0] are not locked */
			kPORT_UnlockRegister
	};

	if(channel)
	{
		portNum = MC33926_IN2.portNumber;
		pinNum = MC33926_IN2.pinNumber;
	}
	else
	{
		portNum = MC33926_IN1.portNumber;
		pinNum = MC33926_IN1.pinNumber;
	}

	PORT_SetPinConfig(ports[portNum], pinNum, &MC33926_CONFIG);
}

int32_t MC33926_SetPin_GPIO(int32_t channel)
{

	PORT_Type *ports[] = PORT_BASE_PTRS;
	PORT_Type port;
	int portNum;
	int pinNum;

	if(channel < 0)
		return SENSOR_ERROR_INVALID_PARAM;
	port_pin_config_t MC33926_CONFIG = {/* Internal pull-up/down resistor is disabled */
			kPORT_PullDisable,
			/* Low internal pull resistor value is selected. */
			kPORT_LowPullResistor,
			/* Fast slew rate is configured */
			kPORT_FastSlewRate,
			/* Passive input filter is disabled */
			kPORT_PassiveFilterDisable,
			/* Open drain output is disabled */
			kPORT_OpenDrainDisable,
			/* Low drive strength is configured */
			kPORT_LowDriveStrength,
			/* Pin is configured GPIO */
			kPORT_MuxAlt0,
			/* Digital input enabled */
			kPORT_InputBufferEnable,
			/* Digital input is not inverted */
			kPORT_InputNormal,
			/* Pin Control Register fields [15:0] are not locked */
			kPORT_UnlockRegister
	};

	if(channel)
	{
		portNum = MC33926_IN2.portNumber;
		pinNum = MC33926_IN2.pinNumber;
	}
	else
	{
		portNum = MC33926_IN1.portNumber;
		pinNum = MC33926_IN1.pinNumber;
	}

	PORT_SetPinConfig(ports[portNum], pinNum, &MC33926_CONFIG);
	if(channel)
		gpioDriver->pin_init(&MC33926_IN2, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
	else
		gpioDriver->pin_init(&MC33926_IN1, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
}

int32_t MC33926_Start_Pwm_1 (int32_t num, int32_t freq, int32_t dutyCycle)
{
	pwm_fault_param_t faultConfig;
	/* Structure of initialize PWM */
	pwm_config_t pwmConfig;

	uint32_t pwmVal = 4;
	/* Use full cycle reload */
	    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
	    /* PWM A & PWM B form a complementary PWM pair */
	    pwmConfig.pairOperation   = kPWM_ComplementaryPwmA;
	    pwmConfig.enableDebugMode = true;

	    /* Initialize submodule 0 */
	    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
	    {
	        PRINTF("PWM initialization failed\n");
	        return 1;
	    }

	    /* Initialize submodule 1, make it use same counter clock as submodule 0. */
	    pwmConfig.clockSource           = kPWM_Submodule0Clock;
	    pwmConfig.prescale              = kPWM_Prescale_Divide_1;
	    pwmConfig.initializationControl = kPWM_Initialize_MasterSync;
	    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
	    {
	        PRINTF("PWM initialization failed\n");
	        return 1;
	    }

	    /* Initialize submodule 2 the same way as submodule 1 */
	    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_2, &pwmConfig) == kStatus_Fail)
	    {
	        PRINTF("PWM initialization failed\n");
	        return 1;
	    }

	    /*
	     *   config->faultClearingMode = kPWM_Automatic;
	     *   config->faultLevel = false;
	     *   config->enableCombinationalPath = true;
	     *   config->recoverMode = kPWM_NoRecovery;
	     */
	    PWM_FaultDefaultConfig(&faultConfig);

	#ifdef DEMO_PWM_FAULT_LEVEL
	    faultConfig.faultLevel = DEMO_PWM_FAULT_LEVEL;
	#endif

	    /* Sets up the PWM fault protection */
	    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_0, &faultConfig);
	    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_1, &faultConfig);
	    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_2, &faultConfig);
	    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_3, &faultConfig);

	    /* Set PWM fault disable mapping for submodule 0/1/2 */
	    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_faultchannel_0,
	                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
	    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_faultchannel_0,
	                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
	    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_faultchannel_0,
	                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);

	    MC33926_PWM_DRV_InitPwm(num, freq, dutyCycle);

	    /* Set the load okay bit for all submodules to load registers from their buffer */
	    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_2 , true);
	    /* Start the PWM generation from Submodules 0, 1 and 2 */

	    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 |  kPWM_Control_Module_2);
	    return SENSOR_ERROR_NONE;
}

int32_t MC33926_Start_Pwm(int32_t num, int32_t freq, int32_t dutyCycle)
{
	pwm_config_t pwmConfig;
	pwm_fault_param_t faultConfig;
	PWM_GetDefaultConfig(&pwmConfig);
	pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
	pwmConfig.enableDebugMode = true;
	//pwmConfig.pairOperation = kPWM_ComplementaryPwmB;
	/* Initialize submodule 0 */
	if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
	{
		PRINTF("PWM initialization failed\n");
		return SENSOR_ERROR_INIT;
	}
	/* Initialize submodule 1, make it use same counter clock as submodule 0. */
	pwmConfig.clockSource           = kPWM_BusClock;
	pwmConfig.prescale              = kPWM_Prescale_Divide_4;
	pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
	if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_2, &pwmConfig) == kStatus_Fail)
	{
		PRINTF("PWM initialization failed\n");
		return SENSOR_ERROR_INIT;
	}


	/*
	 *   config->faultClearingMode = kPWM_Automatic;
	 *   config->faultLevel = false;
	 *   config->enableCombinationalPath = true;
	 *   config->recoverMode = kPWM_NoRecovery;
	 */
	PWM_FaultDefaultConfig(&faultConfig);

#ifdef DEMO_PWM_FAULT_LEVEL
	faultConfig.faultLevel = true;
#endif

	/* Sets up the PWM fault protection */
	PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_0, &faultConfig);
	PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_1, &faultConfig);
	PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_2, &faultConfig);
	PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_3, &faultConfig);

	/* Set PWM fault disable mapping for submodule 0/1/2 */
	PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_faultchannel_0,
			kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
	PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_faultchannel_0,
			kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
	PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_faultchannel_0,
			kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);


	MC33926_PWM_DRV_InitPwm(num, freq, dutyCycle);

	/* Set the load okay bit for all submodules to load registers from their buffer */
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_2 , true);
	/* Start the PWM generation from Submodules 0, 1 and 2 */

	PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 |  kPWM_Control_Module_2);
	return SENSOR_ERROR_NONE;
}

int32_t MC33926_Stop_Pwm(int32_t pwm_active_channel)
{
	//if(pwm_active_channel == 0)
	{
		PWM_SetOutputToIdle(BOARD_PWM_BASEADDR, kPWM_PwmB, kPWM_Module_0, 0);
	}
	//else
	{
		PWM_SetOutputToIdle(BOARD_PWM_BASEADDR, kPWM_PwmB, kPWM_Module_2, 0);
	}
	if(pwm_active_channel < 0)
		return SENSOR_ERROR_NONE;

	PRINTF(" PWM stopped\r\n");
	MC33926_SetPin_GPIO(0);
	MC33926_SetPin_GPIO(1);
	MC33926_IN1_Pin_Write(0);
	MC33926_IN2_Pin_Write(0);
	pwm_active_channel = -1;


	return SENSOR_ERROR_NONE;
}

void MC33926_Pin_Write(gpioHandleKSDK_t *pin, int32_t val)
{
	if(val)
		gpioDriver->set_pin(pin);
	else
		gpioDriver->clr_pin(pin);
}

int32_t MC33926_EN_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_EN);
	else
		gpioDriver->clr_pin(&MC33926_EN);
}


int32_t MC33926_D2B_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_D2B);
	else
		gpioDriver->clr_pin(&MC33926_D2B);
}

int32_t MC33926_D1_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_D1);
	else
		gpioDriver->clr_pin(&MC33926_D1);
}

int32_t MC33926_IPV_INV_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_IPV_INV);
	else
		gpioDriver->clr_pin(&MC33926_IPV_INV);
}

int32_t MC33926_SLEW_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_SLEW);
	else
		gpioDriver->clr_pin(&MC33926_SLEW);
}

int32_t MC33926_IN1_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_IN1);
	else
		gpioDriver->clr_pin(&MC33926_IN1);
}

int32_t MC33926_IN2_Pin_Write (int32_t val)
{
	if(val)
		gpioDriver->set_pin(&MC33926_IN2);
	else
		gpioDriver->clr_pin(&MC33926_IN2);
}

int32_t MC33926_SF_B_Pin_Read (void)
{
	return gpioDriver->read_pin(&MC33926_SF_B);
}


