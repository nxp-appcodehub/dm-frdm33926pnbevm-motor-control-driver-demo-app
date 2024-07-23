/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MC33926_DRV_H_
#define MC33926_DRV_H_

/* Standard C Includes */
#include <stdint.h>

/* ISSDK Includes */
#include "sensor_io_i2c.h"
#include "register_io_i2c.h"
#include "fsl_lpadc.h"
#include "mc33926.h"
#include "gpio_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct
{
	bool isInitialized;              /*!< whether sensor is intialized or not.*/

} mc33926_handle_t;

/*! @brief       The interface function to initialize Motor control driver
 *  @details     This function initializes Motor control driver
 *  @param[in]   pSensorHandle   Motor control driver handler
 *  @return    	 ::MC33926_Initialize() returns the status.
 */
int32_t MC33926_Initialize(mc33926_handle_t *pSensorHandle);

/*! @brief       The interface function Update duty cycle of PWM
 *  @details     This function Update duty cycle of PWM
 *  @param[in]   num  			PWM Subchannel number
 *  @param[in]   dutyCycle      New PWM Duty cycle
 *  @return    	 ::MC33926_Update_Duty_Cycle() returns the status.
 */
int32_t MC33926_Update_Duty_Cycle(int32_t num, int32_t dutyCycle);

/*******************************************************************************
 * APIs
 ******************************************************************************/
/*! @brief       The interface function Start PWM
 *  @details     This function initialize PWM as per given Frequency and Duty cycle
 *  @param[in]   num  			PWM Subchannel number
 *  @param[in]   Freq           PWM Frequency
 *  @param[in]   dutyCycle      PWM Duty cycle
 *  @return    	 ::Start_Pwm() returns the status.
 */
int32_t MC33926_Start_Pwm(int32_t num, int32_t freq, int32_t dutyCycle);

/*! @brief       The interface function Stop PWM
 *  @details     This function initialize PWM as per given Frequency and Duty cycle
 *  @param[in]   num  			PWM Subchannel number
 *  @param[in]   Freq           PWM Frequency
 *  @param[in]   dutyCycle      PWM Duty cycle
 *  @return      ::Stop_Pwm() returns the status.
 */

int32_t MC33926_Stop_Pwm( );

/*! @brief      :  The interface function to Enable/Disable Motor control output
 *  Val  		:  1 - Enable, 0 - Disable
 */
int32_t MC33926_EN_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/Reset D2B Line
 *  @details     This function set/Reset D2B line based upon the input value
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_D2B_Pin_Write() returns the status.
 */
int32_t MC33926_D2B_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/Reset D1 Line
 *  @details     This function set/Reset D1 line based upon the input value
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_D1_Pin_Write() returns the status.
 */
int32_t MC33926_D1_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/Reset IPV_INV Line
 *  @details     This function set/Reset IPV_INV line based upon the input value
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_IPV_INV_Pin_Write() returns the status.
 */
int32_t MC33926_IPV_INV_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/Reset SLEW Line
 *  @details     This function set/Reset SLEW line based upon the input value
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_SLEW_Pin_Write() returns the status.
 */
int32_t MC33926_SLEW_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/reset input Line 1.
 *  @details     This function set input line high or low.
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_IN1_Pin_Write() returns the status.
 */

int32_t MC33926_IN1_Pin_Write (int32_t val);

/*! @brief       The interface function to Set/reset input Line 2.
 *  @details     This function set input line high or low.
 *  @param[in]   Val		1 - Set to High, 0 - Set to low
 *  @return      ::MC33926_IN2_Pin_Write() returns the status.
 */

int32_t MC33926_IN2_Pin_Write (int32_t val);

/*! @brief       The interface function initialises the ADC. 
 *  @details     This function initialises ADC line based upon the input value
 *  @return      ::MC33926_Init_adc() returns the status.
 */

int32_t MC33926_Init_adc(lpadc_config_t *mLpadcConfigStruct);

/*! @brief       The interface function to write gpio line
 *  @details     This function set/clear the gpio pin.
 *  @return      ::MC33926_Pin_Write() returns the status.
 */

void MC33926_Pin_Write(gpioHandleKSDK_t *pin, int32_t val);

/*! @brief       The interface function to read the adc.
 *  @details     This function is used to draw the feedback current.
 *  @return      ::MC33926_Read_adc() returns the status.
 */

int32_t MC33926_Read_adc(void);

/*! @brief       The interface function to Set the GPIO pin line.
 *  @return      ::MC33926_SetPin_GPIO() returns the status.
 */

int32_t MC33926_SetPin_GPIO(int32_t channel);

/*! @brief       The interface function to Set the PWM line input.
 *  @details     This function set one input line as PWM. 
 *  @param[in]   Val 1 - Set to INT1, 0 - Set to INT2.
 *  @return      ::MC33926_SetPin_PWM() returns the status.
 */

int32_t MC33926_SetPin_PWM(int32_t channel);

/*! @brief       The interface function to read the status flag
 *  @details     This function read the SF based upon the input value.
		 if it is 1, then error is there, if 0, then no issues exist. 
 *  @return      ::MC33926_SF_B_Pin_Read() returns the status.
 */

int32_t MC33926_SF_B_Pin_Read (void);


#endif // MC33926_DRV_H_
