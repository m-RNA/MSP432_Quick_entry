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
 *  ======== ADCMSP432.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADC configuration
 */
let devSpecific = {
    config: [
        {
            name: "referenceVoltage",
            displayName: "Reference Voltage",
            description: "Specifies the ADC's reference voltage source.",
            longDescription:`
Specifies the ADC's reference voltage source.
`,
            default: "VDD",
            options: [
                { name: "VDD" },
                { name: "1.2V" },
                { name: "1.45V" },
                { name: "2.5V" },
                {
                    name: "External",
                    description: "Select this option if you are supplying an"
                        + " external reference voltage to the device.",
                    longDescription:`
"The external reference voltage can be specified using the
__External Reference Voltage__ configuration option.
`
                },
                {
                    name: "External Buffered",
                    description: "Select this option if you are supplying a "
                        + "buffered external reference voltage to the device.",
                    longDescription:`
"The external reference voltage can be specified using the
__External Reference Voltage__ configuration option.
`
                }
            ],
            onChange: onReferenceVoltageChange
        },
        {
            name: "externalReferenceVoltage",
            displayName: "External Reference Voltage",
            description: "Specifies the external reference voltage in"
                + " microvolts.",
            longDescription:`
Specifies the external reference voltage in microvolts.
This value can only be modified when the __Reference Voltage__ is configured
as __External__ or __External Buffered__. Otherwise, this value is read only
and indicates the internal voltage used in microvolts.
`,
            default: 3300000,
            readOnly: true
        },
        {
            name: "resolution",
            displayName: "Resolution",
            description: "Specifies the ADC's resolution",
            default: "14 Bits",
            options: [
                { name: "8 Bits" },
                { name: "10 Bits" },
                { name: "12 Bits" },
                { name: "14 Bits" }
            ]
        }
    ],

    /* override generic requirements with dev-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    modules: Common.autoForceModules(["Board", "Power"]),

    templates: {
        boardc: "/ti/drivers/adc/ADCMSP432.Board.c.xdt",
        boardh: "/ti/drivers/adc/ADC.Board.h.xdt"
    },

    _getAdcPinName: _getAdcPinName,
    _getPinResources: _getPinResources
};

/*
/*
 *  ======== _getAdcPinName ========
 *  Returns ADC Pin Name
 *
 * Example inputs of chan:
 * P4.3.GPIO/CS.MCLK/RTC_C.CLK/ADC14.A10
 * P4.1.GPIO/ADC14.A12/LCD_F.L12
 * P4.5.GPIO/ADC14.A8
 */
function _getAdcPinName(inst)
{
    let chan = inst.adc.adcPin.$solution.devicePinName;

     /* PX.Y, ie P4.3 */
    let pName = chan.match(/P\d\.\d/)[0];

     /* ADC14.Axx */
    let ax = chan.match(/ADC14\.A\d{1,2}/)[0].replace("ADC14.", "");
    let px = pName.substr(0, 2);     /* PX */
    let py = pName.substr(3, 1);     /* Y */

    return (px + "_" + py + "_" + ax);
}

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let adcPin = _getAdcPinName(inst);
    let pin = (adcPin.substring(0, adcPin.lastIndexOf("_"))).replace("_", ".");

    if (inst.$hardware && inst.$hardware.displayName) {
        pin += ", " + inst.$hardware.displayName;
    }

    return (pin);
}

/*
 *  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 *
 *  param inst    - a fully configured ADC instance
 *
 *  returns req[] - an array of pin requirements needed by inst
 */
function pinmuxRequirements(inst)
{
    let adc = {
        name: "adc",
        hidden: true,
        displayName: "ADC Peripheral",
        interfaceName: "ADC14",
        canShareWith: "ADC",
        resources: [
            {
                name: "adcPin",
                hidden: false,
                displayName: "ADC Pin",
                interfaceNames: [
                    "A0", "A1", "A2", "A3",
                    "A4", "A5", "A6", "A7",
                    "A8", "A9", "A10", "A11",
                    "A12", "A13", "A14", "A15",
                    "A16", "A17", "A18", "A19",
                    "A20", "A21", "A22", "A23"
                ]
            }
        ],
        signalTypes: { adcPin: ["AIN"] }
    };

    return ([adc]);
}

/*
 *  ======== onReferenceVoltageChange ========
 */
 function onReferenceVoltageChange(inst, ui)
{
    ui.externalReferenceVoltage.readOnly = true;

    switch (inst.referenceVoltage) {
        case "VDD":
            inst.externalReferenceVoltage = 3300000;
            break;
        case "1.2V":
            inst.externalReferenceVoltage = 1200000;
            break;
        case "1.45V":
            inst.externalReferenceVoltage = 1450000;
            break;
        case "2.5V":
            inst.externalReferenceVoltage = 2500000;
            break;
        case "External":
            ui.externalReferenceVoltage.readOnly = false;
            break;
        case "External Buffered":
            ui.externalReferenceVoltage.readOnly = false;
            break;
    }
}


/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ADC module to
 *  allow us to augment/override as needed.
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
    /* required function, called by base ADC module */
    extend: extend
};
