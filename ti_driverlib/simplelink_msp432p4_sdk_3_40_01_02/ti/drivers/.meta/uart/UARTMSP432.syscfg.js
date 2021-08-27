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
 *  ======== UARTMSP432.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "UART peripheral interrupt priority";

let logError = Common.logError;
let logInfo  = Common.logInfo;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base UART configuration
 */
let devSpecific = {
    config:
    [
        /*
         * This field is inter-related with the baudRates field.  Ideally
         * the user specifies the baudRates and leaves this field as 'Any',
         * which allows the config tool to solve for clock source.  However,
         * for expert users, we allow clockSource to be locked which will
         * 1) disable auto solving based on baudRates, and potentially
         * 2) prevent the configuration from migrating to an alternate
         *    board/device which does not support the locked clockSource
         *    value.
         */
        {
            name        : "clockSource",
            displayName : "Clock Source",
            default     : "Any",
            description : "Specifies the clock source for the UART peripheral",
            longDescription :`If 'Any' is selected, an appropriate clock source
will be auto selected. For maximum portability to other devices, it is
recommended that this option be left in it's default state of 'Any'.

The frequency of the clock sources are configured per
__Power Performance Level__ configured in the __Power Module__.
`,
            /* These are ordered by preference.  If multiple sources can
             * satisfy the baud rate, then the first one in the list
             * that can satisfy the baud rate will be selected.
             */
            options:
            [
                { name: "Any" },
                { name: "ACLK" },
                { name: "SMCLK" }
                // { name : "HSMCLK" }, // Not Yet Supported by Driverlib
                // { name : "MCLK"   }, // Not Yet Supported by Driverlib
            ]
        },
        {
            name        : "ringBufferSize",
            displayName : "Ring Buffer Size",
            description : "Number of bytes in the ring buffer",
            longDescription : `The ring buffer serves as an extension of the
FIFO. If data is received when UART_read() is not called, data will be stored
in the ring buffer. The size can be changed to suit the application.`,
            default     : 32
        },
        {
            name        : "bitOrder",
            displayName : "Bit Order",
            default     : "LSB_FIRST",
            description : "Almost always LSB First",
            options     : [
                {
                    name : "MSB_FIRST"
                },
                {
                    name : "LSB_FIRST"
                }
            ]
        },
        intPriority
    ],

    /* override generic pin requirements */
    pinmuxRequirements    : pinmuxRequirements,

    /* override device-specific templates */
    templates: {
        boardc : "/ti/drivers/uart/UARTMSP432.Board.c.xdt",
        boardh : "/ti/drivers/uart/UART.Board.h.xdt"
    },

    /* override generic filterHardware with ours */
    filterHardware        : filterHardware,

    /* define MSP432 UART specific methods */
    autoAssignClockSource : autoAssignClockSource,
    genBaudRateTable      : genBaudRateTable,

    _getPinResources: _getPinResources
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;
    let rxPin = "Unassigned";
    let txPin = "Unassigned";

    if (inst.uart) {
        if (inst.uart.rxPin) {
            rxPin = inst.uart.rxPin.$solution.devicePinName;
            rxPin = rxPin.match(/P\d+\.\d+/)[0];
        }
        if (inst.uart.txPin) {
            txPin = inst.uart.txPin.$solution.devicePinName;
            txPin = txPin.match(/P\d+\.\d+/)[0];
        }

        pin = "\nTX: " + txPin + "\nRX: " + rxPin;

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += "\n" + inst.$hardware.displayName;
        }
    }

    return (pin);
}


/*
 *  ======== autoAssignClockSource ========
 *  Assign a clock source for a UART.
 *
 *  Assign the clock source based on a required baud rate and
 *  known frequencies of the clock sources from the Power module.
 *
 *  param baudRates         - The Baud Rate for the UART
 *
 *  returns clockSourceName - The name of the most appropriate clock source.
 */
function autoAssignClockSource(baudRates)
{
    /* Find the clock source options from the module */
    let clockSources;
    for (let idx = 0; idx < devSpecific.config.length; idx++) {
        if (devSpecific.config[idx].name === 'clockSource') {
            clockSources = devSpecific.config[idx].options;
        }
    }

    if (clockSources === undefined) {
        throw new Error('Uart Metacode is not in sync with Uart Metadata. ');
    }

    let choices = [];

    /* Walk through the clock source options for this uart.
     * This walks through the module exports, not the instance
     */
    for (let i = 0; i < clockSources.length; i++) {
        let optionName = clockSources[i].name;
        if (optionName === 'Any') {
            continue;
        }

        let Power = system.modules["/ti/drivers/Power"];
        let frequencies = Power.getClockFrequencies(optionName);
        if (frequencies.length === 0) {
            throw new Error(optionName +
                ' cannot be found in Performance_levels tables. ');
        }

        let numGoodFreqs = 0;

        for (let f = 0; f < frequencies.length; f++) {
            for (let bidx = 0; bidx < baudRates.length; bidx++) {
                if (frequencies[f] >= baudRates[bidx]) {
                    numGoodFreqs++;
                }
            }
        }

        /* If we have a clock where all freqs cover all baudRates, then done */
        if (numGoodFreqs == (frequencies.length * baudRates.length)) {
            return optionName;
        }

        /* else save the coverage factor so that a later choice can be made */
        choices.push({
            clock: optionName,
            quality: (numGoodFreqs / (frequencies.length * baudRates.length))
        });
    }

    let bestIdx = 0;
    for (let c = 1; c < choices.length; c++) {
        if (choices[c].quality > choices[bestIdx].quality) {
            bestIdx = c;
        }
    }

    return choices[bestIdx].clock;
}

/*
 *  ======== genBaudRateTable ========
 *  Generate a baud rate table for a UART instance
 *
 *  Given a baud rate and a clock source, generate baud rate table entries for
 *  the cross product of baud rates x clock source frequencies. Currently the
 *  only a single baud rate is supported.
 *
 *  param baudRates       - The Baud Rates for the UART
 *  param clockSource     - The name of the clock source.
 *
 *  returns baudRateTable - The baud rate table entries for this UART instance.
 */
function genBaudRateTable(baudRates, clockSource)
{
    let Power = system.modules["/ti/drivers/Power"];
    let frequencies = Power.getClockFrequencies(clockSource);
    if (frequencies.length === 0) {
        throw new Error(clockSource +
            ' cannot be found in Performance_Levels tables. ');
    }

    let baudRateTable = [];

    for (let b = 0; b < baudRates.length; b++) {
        for (let j = 0; j < frequencies.length; j++) {
            let res = eusci_calcBaudDividers(frequencies[j], baudRates[b]);
            res.baud = baudRates[b];
            res.freq = frequencies[j];
            baudRateTable.push(res);
        }
    }

    return baudRateTable;
}

/*
 *  ======== bitPosition ========
 *  Determine if a bit in an integer is set
 *
 *  param value      - The integer to check
 *  param position   - The position of the bit to check.
 *
 *  returns boolean  - true, if the bit in the position is set,
 *                     false, otherwise.
 */
function bitPosition(value, position)
{
    if ((value & (1 << position)) !== 0) {
        return (1);
    }
    return (0);
}

/*
 *  ======== eusci_calcBaudDividers ========
 *  Computes the eUSCI_UART register settings for a given clock and baud rate
 *
 *  This function returns a JavaScript object containing the fields:
 *      UCOS16:      the oversampling bit (0 or 1)
 *      UCBRx:       the Baud Rate Control Word
 *      UCFx:        the First modulation stage select (UCBRFx)
 *      UCSx:        the Second modulation stage select (UCBRSx)
 *      maxAbsError: the maximum TX error for the register setting above
 *
 *  The first four field names match the names used in Table 18-5,
 *  "Recommended Settings for Typical Crystals and Baudrates", of the
 *  MSP430FR57xx Family User's Guide (SLAU272A).
 *
 *  param clockRate      - The input clock frequency
 *  param baudRate       - The desired output baud rate
 *
 * returns baudRateTableEntry - A baud rate table entriy
 */
function eusci_calcBaudDividers(clockRate, baudRate)
{
    let result      = { UCOS16: 0, UCBRx: 0, UCFx: 0, UCSx: 0, maxAbsError: 0 };
    let N           = Math.floor(clockRate / baudRate);
    let baudPeriod  = 1 / baudRate;
    let clockPeriod = 1 / clockRate;
    let minAbsError = 100000;

    for (let jj = 0; jj <= 255; jj++) {
        let maxAbsErrorInByte = 0;
        let count = 0;

        for (let ii = 0; ii <= 10; ii++) {
            count += N + bitPosition(jj, 7 - (ii % 8));
            let error = (ii + 1) * baudPeriod - count * clockPeriod;

            error = Math.abs(error);
            if (error > maxAbsErrorInByte) {
                maxAbsErrorInByte = error;
            }
        }

        if (maxAbsErrorInByte < minAbsError) {
            minAbsError = maxAbsErrorInByte;
            result.UCSx = jj;
        }
    }

    if (N < 20) {
        result.UCOS16 = 0;
        result.UCBRx  = N;
        result.UCFx   = 0;
    }
    else {
        result.UCOS16 = 1;
        result.UCBRx  = Math.floor(N / 16);
        result.UCFx   = N - (result.UCBRx * 16);
    }

    result.maxAbsError = minAbsError * baudRate * 100;

    return (result);
}

/*
 *  ======== pinmuxRequirements ========
 *  Control RX, TX pin usage by the user specified dataDirection.
 *
 *  param inst      - UART instance
 *
 *  returns req[] - array of requirements needed by inst
 */
function pinmuxRequirements(inst)
{
    let tx = {
        name              : "txPin",  /* config script name */
        displayName       : "TX Pin", /* GUI name */
        interfaceNames    : ["TXD"]   /* pinmux tool name */
    };

    let rx = {
        name              : "rxPin",
        displayName       : "RX Pin",
        interfaceNames    : ["RXD"]
    };

    let resources = [];

    if (inst.dataDirection != 'Receive Only') {
        resources.push(tx);
    }

    if (inst.dataDirection != 'Send Only') {
        resources.push(rx);
    }

    let uart = {
        name          : "uart",
        displayName   : "UART Peripheral",
        interfaceName : "UART",
        resources     : resources,
        signalTypes   : {
            txPin     : ['UART_TXD'],
            rxPin     : ['UART_RXD']
        }
    };

    return [uart];
}

/*
 *  ======== filterHardware ========
 *  Check 'component' signals for compatibility with UART
 *
 *  param component - hardware object describing signals and
 *                     resources they're attached to
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    return (Common.typeMatches(component.type, ["UART"]));
}

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  param inst       - UART instance to be validated
 *  param validation - object to hold detected validation issues
 *
 *  @param $super    - needed to call the generic module's functions
 */
function validate(inst, validation, $super)
{
    let baudRates   = inst.baudRates;
    let clockName   = inst.clockSource;

    if (clockName == 'Any') {
        clockName = autoAssignClockSource(baudRates);
    }

    validateBaudRates(inst, validation, clockName);

    if (inst.ringBufferSize < 0) {
        logError(validation, inst, "ringBufferSize", "value must be positive");
    }

    /* don't allow an unreasonably large ring buffer size */
    if (inst.ringBufferSize > 1024) {
        logInfo(validation, inst, "ringBufferSize",
            "consider reducing size for space optimization");
    }

    if ($super.validate) {
        $super.validate(inst, validation);
    }
}

/*
 *  ======== validateBaudRates ========
 *  Validate the clockName vs. the selected baud rates.
 *
 *  param inst       - UART instance to be validated
 *  param validation - Issue reporting object
 *  param clockName  - The selected clock source name
 */
function validateBaudRates(inst, validation, clockName)
{
    let Power = system.modules["/ti/drivers/Power"];
    let frequencies = Power.getClockFrequencies(clockName);
    let baudRates   = inst.baudRates;
    let message     = '';
    let numGoodFreqs = 0;

    for (let f = 0; f < frequencies.length; f++) {
        for (let b = 0; b < baudRates.length; b++) {
            if (frequencies[f] >= baudRates[b]) {
                numGoodFreqs++;
            }
        }
    }

    /* If clock has all freqs covering all baudRates, then no problems */
    if (numGoodFreqs == (frequencies.length * baudRates.length)) {
        return; // no error or warning
    }

    /* if the clock source cannot support the baudRates at all */
    if (numGoodFreqs == 0) {
        message = "Clock source " + clockName +
            " cannot support any of the specified baud rates";
        logError(validation, inst, ["baudRates","clockSource"], message);
    }

    /* if the clock source can support some baudRates at only some
     * performance levels, but not all of them
     */
    else {
        message = "Clock source " + clockName +
            " can only support some baud rates at some performance levels.";
        logInfo(validation, inst, ["baudRates","clockSource"], message);
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* override base validate */
    devSpecific.validate = function (inst, validation) {
        return validate(inst, validation, base);
    };

    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    /* override baudRates table */
    for (let i = 0; i < result.config.length; i++) {
        let cfg = result.config[i];
        if (cfg.name == "baudRates") {
            cfg.hidden = false;
            break;
        }
    }

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base UART module */
    extend: extend
};
