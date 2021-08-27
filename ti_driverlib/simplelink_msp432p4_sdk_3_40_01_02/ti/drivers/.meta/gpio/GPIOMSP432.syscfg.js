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
 *  ======== GPIOMSP432.syscfg.js ========
 */

"use strict";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base GPIO configuration
 */
let devSpecific = {

    templates:
    {
        boardc : "/ti/drivers/gpio/GPIOMSP432.Board.c.xdt",
        boardh : "/ti/drivers/gpio/GPIO.Board.h.xdt"
    },

    _getPinResources: _getPinResources
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;

    if (inst.gpioPin) {
        pin = inst.gpioPin.$solution.devicePinName;
        pin = pin.match(/P\d+\.\d+/)[0];

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += ", " + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== resourcesWithoutInterruptsFilter ========
 *  Only ports 1-6 support interrupts.
 */
function resourcesWithoutInterruptsFilter (devicePin, peripheralPin)
{
    let pName = peripheralPin.peripheralName;
    let pNum = pName.substring(1, pName.indexOf("."));

    /* only ports 1-6 support interrupts */
    if (pNum < 7) {
        return (true);
    }
    else {
        return (false);
    }
}

/*
 *  ======== noFilter ========
 *  All ports/resources are available if interrupts are not required.
 */
function noFilter (devicePin, peripheralPin)
{
    return (true);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 *  Called on instantiation and every config change.
 */
function pinmuxRequirements(inst, $super)
{
    let result = $super.pinmuxRequirements ? $super.pinmuxRequirements(inst) : [];

    /*
     * Install the appropriate GPIO filter
     * Unless the filter function changes, it is called only once
     * for every possible pin. The following logic forces a re-evaluation
     * of the set of valid resources whenever the state of 'interrupt'
     * changes.
     */
    if (result[0] != null) {
        result[0].filter = inst.interruptTrigger !== "None"
            ? resourcesWithoutInterruptsFilter : noFilter;
    }

    return (result);
}


/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic module to
 *  allow us to augment and override as needed.
 */
function extend(base)
{
    /* override base pinmuxRequirements */
    devSpecific.pinmuxRequirements = function (inst) {
        return pinmuxRequirements(inst, base);
    };

    /* merge and overwrite base module attributes */
    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    extend: extend
};
