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
 *  ======== CaptureMSP432.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Capture configuration
 */
let devSpecific = {
    config: [
        {
            name        : "clockSource",
            displayName : "Clock Source",
            default     : "SMCLK",
            description : "Specifies the timer peripheral clock source.",
            longDescription:`The frequency of the clock sources are configured
per __Power Performance Level__ configured in the __Power Module__.
`,
            options     : [
                { name: "ACLK" },
                { name: "SMCLK" },
                { name: "External TXCLK" },
                { name: "Inverted External TXCLK" }
            ]
        },
        {
            name        : "clockDivider",
            displayName : "Clock Divider",
            description : "Specifies the clock source divider.",
            longDescription: "The __Clock Source__ will be divided by this"
                + " value before being used by the timer peripheral",
            default     : 2,
            options     : [
                { name: 1 },  { name: 2 },  { name: 3 },  { name: 4 },
                { name: 5 },  { name: 6 },  { name: 7 },  { name: 8 },
                { name: 10 }, { name: 12 }, { name: 14 }, { name: 16 },
                { name: 20 }, { name: 24 }, { name: 28 }, { name: 32 },
                { name: 40 }, { name: 48 }, { name: 56 }, { name: 64 }
            ]
        },

        intPriority
    ],

    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc : "/ti/drivers/capture/CaptureMSP432.Board.c.xdt",
        boardh : "/ti/drivers/capture/Capture.Board.h.xdt"
    },

    _getPinResources: _getPinResources
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;

    if (inst.timer) {
        pin = inst.timer.capturePin.$solution.devicePinName;
        pin = pin.match(/P\d+\.\d+/)[0];

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += ", " + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    let timer = {
        name: "timer",
        displayName: "Timer Peripheral",
        interfaceName: "TIMER_A",
        resources: [
            {
                name: "capturePin",
                displayName: "Capture Pin",
                interfaceNames: [
                    "CCI0A", "CCI1A", "CCI2A", "CCI3A", "CCI4A"
                ]
            }
        ]
    };

    return ([timer]);
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Capture module to
 *  allow us to augment/override as needed for the MSP432
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base Capture module */
    extend: extend
};
