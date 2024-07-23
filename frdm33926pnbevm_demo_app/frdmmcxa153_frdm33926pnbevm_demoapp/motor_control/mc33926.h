/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MC33926_H_
#define MC33926_H_


#define BOARD_PWM_BASEADDR 		(FLEXPWM0)
#define PWM_SRC_CLK_FREQ 		(CLOCK_GetFreq(kCLOCK_BusClk))
#define DEMO_PWM_CLOCK_DEVIDER	(kPWM_Prescale_Divide_2)
#define DEMO_LPADC_BASE         ADC0
#define DEMO_LPADC_IRQn                  ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC      ADC0_IRQHandler

#define DEMO_LPADC_USER_CHANNEL          8U
#define DEMO_LPADC_USER_CMDID            1U /* CMD1 */

#endif



