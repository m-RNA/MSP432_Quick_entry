/*
 * Copyright (c) 2018-2019 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

/*
 *  ======== WatchdogMSP432.syscfg.js ========
 */

"use strict";

let Power  = system.getScript("/ti/drivers/Power");
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.default = "1";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Watchdog configuration
 */
let devSpecific = {
    config : [
        intPriority
    ],

    templates : {
        boardc : "/ti/drivers/watchdog/WatchdogMSP432.Board.c.xdt",
        boardh : "/ti/drivers/watchdog/Watchdog.Board.h.xdt"
    }
};

/*
 *  ======== addPeriodChoices ========
 *  Add enumeration values for the period configurable.
 *
 *  The common period configurable is not an enumerable, but the MSP432
 *  watchdog is constrained to a set of values.  This routine will constrain
 *  the period configurable defined in the common watchdog module, with MSP432
 *  specific constraints.
 *
 *  @param config - The configurables list for watchdog
 *
 */
function addPeriodChoices(config)
{
    let divide_ratios = [31, 27, 23, 19, 15, 13, 9, 6];
    let frequencies   = Power.getClockFrequencies('ACLK');

    /* We restrict the possible period values to just the one
     * supported by ACLK clock source. This eliminates dependency
     * on performance level.
     */
    if (frequencies.length !== 1) {
        throw new Error('Watchdog clock (ACLK) cannot vary with perf levels.');
    }

    /* find the period configurable in config */
    let period;
    for (let idx = 0; idx < config.length; idx++) {
        if (config[idx].name === 'period') {
            period = config[idx];
        }
    }

    /* define period's options */
    if (period != null) {
        let freq = frequencies[0];

        period.options = [];
        for (let idx = 0; idx < divide_ratios.length; idx++) {
            let periodMs = (Math.pow(2, divide_ratios[idx]) / freq) * 1000;
            period.options[idx] = { name: periodMs };
        }
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);
    addPeriodChoices(result.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base Watchdog module */
    extend : extend
};
