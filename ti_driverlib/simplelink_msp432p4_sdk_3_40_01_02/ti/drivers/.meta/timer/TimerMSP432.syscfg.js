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
 *  ======== TimerMSP432.syscfg.js ========
 */

"use strict";

/* MSP432 specific Timer configurables. */
let config = [
    {
        name        : "timerType",
        displayName : "Timer Type",
        onChange    : onChangeTimerType,
        default     : "16 Bits",
        options     : [
                         { name: "32 Bits", displayName: 'Timer32' },
                         { name: "16 Bits", displayName: 'Timer_A' }
                      ]
    },
    /* clockSource is only valid for Timer_A.  It is hidden by default and
     * is exposed when timer type is changed to Timer_A
     */
    {
        name        : "clockSource",
        displayName : "Clock Source",
        description : "Specifies the clock source of the timer peripheral",
        longDescription:`The frequency of the clock sources are configured
per __Power Performance Level__ configured in the __Power Module__.
`,
        default     : "ACLK",
        hidden      : true,
        options     : [
            { name: "ACLK" },
            { name: "SMCLK" },
            { name: "EXTERNAL TXCLK" },
            { name: "INVERTED EXTERNAL TXCLK" }
        ]
    }
];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Timer configuration
 */
let devSpecific = {
    config : config,
    templates : {
        boardc : "/ti/drivers/timer/TimerMSP432.Board.c.xdt",
        boardh : "/ti/drivers/timer/Timer.Board.h.xdt"
    },

    pinmuxRequirements : pinmuxRequirements
};

/*
 *  ======== onChangeTimerType ========
 *  onChange callback function for the timerType config
 *
 *  param inst  - Instance containing the config that changed
 *  param ui    - The User Interface object
 */
function onChangeTimerType(inst, ui)
{
    if (inst.timerType === "32 Bits") {
        ui.clockSource.hidden = true;
    } 
    else {
        ui.clockSource.hidden = false;
    }
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    let timer = {
        name          : "timer",
        displayName   : "Timer Peripheral",
        interfaceName : "TIMER",
        resources     : []
    };

    if (inst.timerType === "32 Bits") {
        timer.interfaceName += "32";
    }
    else {
        timer.interfaceName += "_A";
    }

    return ([timer]);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Timer module to
 *  allow us to augment/override as needed for the CC32XX
 */
function extend(base)
{
    /* make a shallow copy of base.config */
    let clone = base.config.slice();

    /* determine the timerType config Index */
    let index = clone.findIndex(x => x.name === "timerType");

    /* delete the top level timerType config */
    clone.splice(index, 1);

    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = clone.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base Timer module */
    extend: extend
};
