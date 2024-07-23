/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file frdmmcxn947.c
 * @brief The frdmmcxn947.c file defines GPIO pins and I2C CMSIS utilities for frdmmcxn947 board.
 */

#include <frdmmcxa153.h>

/* [PIN 4] Configured as FB */
gpioHandleKSDK_t A0 = {.base = GPIO1, .pinNumber = 10, .mask = 1 << (10), .clockName = kCLOCK_GateGPIO1, .portNumber = 1};
/* [PIN 14] Configured as SF_B */
gpioHandleKSDK_t A3 = {.base = GPIO2, .pinNumber = 0, .mask = 1 << (0), .clockName = kCLOCK_GateGPIO2, .portNumber = 2};
/* [PIN 63] Configured as D1 */
gpioHandleKSDK_t D1 = {.base = GPIO1, .pinNumber = 5, .mask = 1 << (5), .clockName = kCLOCK_GateGPIO1, .portNumber = 1};
/* [PIN 38] Configured as PWM1-(IN1) */
gpioHandleKSDK_t D5 = {.base = GPIO3, .pinNumber = 12, .mask = 1 << (12), .clockName = kCLOCK_GateGPIO3, .portNumber = 3};
/* [PIN 37] Configured as PWM2-(IN2) */
gpioHandleKSDK_t D6 = {.base = GPIO3, .pinNumber = 13, .mask = 1 << (13), .clockName = kCLOCK_GateGPIO3, .portNumber = 3};
/* [PIN 35] Configured as SLEW */
gpioHandleKSDK_t D8 = {.base = GPIO3, .pinNumber = 15, .mask = 1 << (15), .clockName = kCLOCK_GateGPIO3, .portNumber = 3};
/* [PIN 36] Configured as D2B */
gpioHandleKSDK_t D9 = {.base = GPIO3, .pinNumber = 14, .mask = 1 << (14), .clockName = kCLOCK_GateGPIO3, .portNumber = 3};
/* [PIN 20] Configured as IPV_INV */
gpioHandleKSDK_t D10 = {.base = GPIO2, .pinNumber = 6, .mask = 1 << (6), .clockName = kCLOCK_GateGPIO2, .portNumber = 2};
/* [PIN 2] Configured as EN */
gpioHandleKSDK_t D18 = {.base = GPIO1, .pinNumber = 8, .mask = 1 << (8), .clockName = kCLOCK_GateGPIO1, .portNumber = 1};


/*!
 * @brief Configures the system to WAIT power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeWait(void *arg)
{
    // POWER_EnterSleep();

    return kStatus_Success;
}

/*!
 * @brief Configures the system to VLPR power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeVlpr(void *arg)
{
    // POWER_EnterSleep();

    return kStatus_Success;
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetLpuartClkFreq(0u);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPI2C0_GetFreq(void)
{
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C0, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPI2C0);

    return CLOCK_GetLpi2cClkFreq();
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPSPI0_GetFreq(void)
{
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI0, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPSPI0);

    return CLOCK_GetLpspiClkFreq(0);
}
