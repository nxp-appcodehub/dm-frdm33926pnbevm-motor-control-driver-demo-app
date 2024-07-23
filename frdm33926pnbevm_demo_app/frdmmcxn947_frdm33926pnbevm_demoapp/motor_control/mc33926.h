/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef MC33926_H_
#define MC33926_H_

#define BOARD_PWM_BASEADDR 		(PWM1)
#define PWM_SRC_CLK_FREQ 		(CLOCK_GetFreq(kCLOCK_BusClk))
#define DEMO_PWM_FAULT_LEVEL      true
#define APP_DEFAULT_PWM_FREQUENCE (10000UL)

#define DEMO_SPC_BASE               SPC0
#define DEMO_LPADC_BASE         	ADC0
#define DEMO_LPADC_IRQn             ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC0_IRQHandler

#define DEMO_LPADC_USER_CHANNEL     0U
#define DEMO_LPADC_USER_CMDID       1U /* CMD1 */

#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true
#define DEMO_LPADC_OFFSET_VALUE_A        0x10U
#define DEMO_LPADC_OFFSET_VALUE_B        0x10U

#define DEMO_VREF_BASE VREF0

#endif



