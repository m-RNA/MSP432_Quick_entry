/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       WatchdogMSP432.h
 *
 *  @brief      Watchdog driver implementation for MSP432
 *
 *  The Watchdog header file for MSP432 should be included in an application
 *  as follows:
 *  @code
 *  #include <ti/drivers/Watchdog.h>
 *  #include <ti/drivers/watchdog/WatchdogMSP432.h>
 *  @endcode
 *
 *  Refer to @ref Watchdog.h for a complete description of APIs & example of
 *  use.
 *
 *  ## Overview #
 *  The MSP432 Watchdog Timer will cause resets at an interval based on the
 *  count calculated from the clock source and clock divider specified in the
 *  WatchdogMSP432_HWAttrs. By default the Watchdog driver has resets turned on.
 *  This means that if Watchdog_clear() is not called to reset the Watchdog
 *  timer before it times out, a reset will be generated. Watchdog_close() will
 *  stop the Watchdog timer. To restart it again, Watchdog_open() must be
 *  called.
 *
 *  Opening the Watchdog driver with resets turned off (Using the
 *  #Watchdog_Params.resetMode parameter) allows the Watchdog Timer to be used
 *  like another timer interrupt.  The callback fxn provided in the params will
 *  be executed when the timer expires.
 *
 *  @warning The watchdog peripheral does not support a Non-Maskable Interrupt (NMI).
 *
 *  ## Unsupported Functionality #
 *  1.  #Watchdog_Params.debugStallMode is not supported by this implementation.
 *  2.  #Watchdog_Params.callbackFxn is not supported when using
 *  #Watchdog_RESET_ON mode.
 *  3.  Watchdog_setReload() and Watchdog_convertMsToTicks() APIs are not
 *  supported by this implementation.
 ******************************************************************************
 */

#ifndef ti_drivers_watchdog_WatchdogMSP432__include
#define ti_drivers_watchdog_WatchdogMSP432__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/Watchdog.h>

#include <ti/drivers/dpl/HwiP.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @addtogroup Watchdog_STATUS
 *  WatchdogMSP432_STATUS_* macros are command codes only defined in the
 *  WatchdogMSP432.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/watchdog/WatchdogMSP432.h>
 *  @endcode
 *  @{
 */

/* Add WatchdogMSP432_STATUS_* macros here */

/** @}*/

/**
 *  @addtogroup Watchdog_CMD
 *  WatchdogMSP432_CMD_* macros are command codes only defined in the
 *  WatchdogMSP432.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/watchdog/WatchdogMSP432.h>
 *  @endcode
 *  @{
 */

/* Add WatchdogMSP432_CMD_* macros here */

/** @}*/

/*! @brief  Watchdog function table for MSP432 */
extern const Watchdog_FxnTable WatchdogMSP432_fxnTable;

/*!
 *  @brief      Watchdog hardware attributes for MSP432
 *  These fields are used by driverlib APIs and therefore must be populated by
 *  driverlib macro definitions. For MSP432Ware these definitions are found in:
 *      - wdt_a.h
 *
 *  intPriority is the Watchdog timer's interrupt priority, as defined by the
 *  underlying OS.  It is passed unmodified to the underlying OS's interrupt
 *  handler creation code, so you need to refer to the OS documentation
 *  for usage.  For example, for SYS/BIOS applications, refer to the
 *  ti.sysbios.family.arm.m3.Hwi documentation for SYS/BIOS usage of
 *  interrupt priorities.  If the driver uses the ti.dpl interface
 *  instead of making OS calls directly, then the HwiP port handles the
 *  interrupt priority in an OS specific way.  In the case of the SYS/BIOS
 *  port, intPriority is passed unmodified to Hwi_create().
 *
 *  A sample structure is shown below:
 *  @code
 *  const WatchdogMSP432_HWAttrs watchdogMSP432HWAttrs[MSP_EXP432P401R_WATCHDOGCOUNT] = {
 *      {
 *          .baseAddr = WDT_A_BASE,
 *          .intNum = INT_WDT_A,
 *          .intPriority = ~0,
 *          .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
 *          .clockDivider = WDT_A_CLOCKDIVIDER_8192K
 *      },
 *  };
 *  @endcode
 */
typedef struct {
    uint32_t baseAddr;     /*!< Base address of Watchdog */
    uint8_t  intNum;       /*!< WDT interrupt number */
    uint8_t  intPriority;  /*!< WDT interrupt priority */
    uint8_t  clockSource;  /*!< Clock source for Watchdog */
    uint8_t  clockDivider; /*!< Clock divider for Watchdog */
} WatchdogMSP432_HWAttrs;

/*!
 *  @brief      Watchdog Object for MSP432
 *
 *  Not to be accessed by the user.
 */
typedef struct {
    Watchdog_ResetMode resetMode;
    HwiP_Handle        hwiHandle;
    bool               isOpen; /* Flag for open/close status */
} WatchdogMSP432_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_watchdog_WatchdogMSP432__include */
