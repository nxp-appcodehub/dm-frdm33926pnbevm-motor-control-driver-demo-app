/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! File: frdm_mc33926_shield.h
* @brief The frdm_mc33926_shield.h file declares arduino pin mapping for frdm_mc33926_shield expansion board.
*/

#ifndef _FRDM_MC33926_SHIELD_H_
#define _FRDM_MC33926_SHIELD_H_

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------

#define PWM_SRC_CLK_FREQ 		(CLOCK_GetFreq(kCLOCK_BusClk))
#define DEMO_PWM_CLOCK_DEVIDER	(kPWM_Prescale_Divide_4)
#define DEMO_PWM_FAULT_LEVEL   	true
#define DEMO_LPADC_VREF_SOURCE  kLPADC_ReferenceVoltageAlt3

/* The shield name */
#define SHIELD_NAME "FRDM-MC33926"

// MC33926 Motor controller Information

#define MC33926_D1       D1
#define MC33926_D2B     D9
#define MC33926_EN     D18
#define MC33926_IPV_INV     D10
#define MC33926_SLEW     D8
#define MC33926_SF_B A3
#define MC33926_IN1 D5
#define MC33926_IN2 D6
#define MC33926_IN1_CONFIG port2_7_pinF8_config
#define MC33926_IN2_CONFIG port2_3_pinF8_config



#endif /* _FRDM_MC33926_SHIELD_H_ */
