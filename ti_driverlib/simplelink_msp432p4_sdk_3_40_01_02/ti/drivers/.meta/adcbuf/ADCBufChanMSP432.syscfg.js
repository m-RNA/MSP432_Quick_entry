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
 *  ======== ADCBufChanMSP432.syscfg.js ========
 */

"use strict";

let adcChanNames = [
    "A0", "A1", "A2", "A3",
    "A4", "A5", "A6", "A7",
    "A8", "A9", "A10", "A11",
    "A12", "A13", "A14", "A15",
    "A16", "A17", "A18", "A19",
    "A20", "A21", "A22", "A23"
];

let config = [
    {
        name: "$name",
        defaultPrefix: "CHANNEL_",
        description: "C identifier used to identify this ADCBuf channel.",
        longDescription: "This name is concatenated to the end of the name"
            + " __Name__ specified in the ADCBuf module configuration"
            + " above.",
        hidden: false
    },
    {
        name: "referenceSource",
        displayName: "Reference Source",
        default: "VDD",
        options: [
            { name: "VDD" },
            { name: "1.2V" },
            { name: "1.45V" },
            { name: "2.5V" },
            { name: "External" },
            { name: "External Buffered" }
        ],
        onChange: onReferenceSourceChange
    },
    {
        name: "referenceVoltage",
        displayName: "Reference Voltage",
        description: "Reference voltage in microvolts.",
        default: 3300000,
        readOnly: true
    },
    {
        name: "inputSource",
        displayName: "Input Source",
        description: "Specifies the channel input source.",
        default: "External Pin",
        options: [
            { name: "External Pin" },
            { name: "Internal Temperature" },
            { name: "Internal Battery" }
        ],
        onChange: onInputSourceChange
    },
    {
        name: "mode",
        displayName: "Mode",
        description: "Specifies the channel's operation mode",
        default: "Non Differential",
        options: [
            { name: "Non Differential" },
            { name: "Differential" }
        ]
    }
];

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    let adcBufChan = {
        name: "adc",
        displayName: "ADC Peripheral",
        hidden: true,
        interfaceName: "ADC14",
        canShareWith: "ADCBuf",
        resources: []
    };

    if (inst.inputSource === "External Pin") {
        adcBufChan.resources.push({
            name: "adcPin",
            displayName: "ADC Pin",
            interfaceNames: adcChanNames
        });
        adcBufChan.signalTypes = { adcPin: ["AIN"] };
    }

    return ([adcBufChan]);
}

/*
 *  ========= moduleInstances ========
 */
function moduleInstances(inst)
{
    let result =[];

    if (inst.mode === "Differential") {
        result.push({
            name: "differentialPin",
            displayName: "Differential Pin",
            moduleName: "/ti/drivers/adcbuf/ADCBufDiffMSP432"
        });
    }

    return (result);
}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with ADC
 *
 *  @param component - hardware object describing signals and
 *                     resources they're attached to
 *  @returns matching pinRequirement object if ADC is supported.
 */
function filterHardware(component)
{
    for (let sig in component.signals) {
        if (component.signals[sig].type == "AIN") {
            return (true);
        }
    }
    return (false);
}

/*
 *  ======== onReferenceSourceChange ========
 */
function onReferenceSourceChange(inst, ui)
{

    ui.referenceVoltage.readOnly = true;

    switch (inst.referenceSource) {
        case "VDD":
            inst.referenceVoltage = 3300000;
            break;
        case "1.2V":
            inst.referenceVoltage = 1200000;
            break;
        case "1.45V":
            inst.referenceVoltage = 1450000;
            break;
        case "2.5V":
            inst.referenceVoltage = 2500000;
            break;
        case "External":
            ui.referenceVoltage.readOnly = false;
            break;
        case "External Buffered":
            ui.referenceVoltage.readOnly = false;
            break;
    }
}

/*
 *  ======== onInputSourceChange ========
 */
function onInputSourceChange(inst, ui)
{
    if (inst.inputSource != "External Pin") {
        ui.mode.hidden = true;
        inst.mode = "Non Differential";
    }
    else {
        ui.mode.hidden = false;
    }
}

/*
 *  ======== onHardwareChange ========
 */
function onHardwareChanged(inst, ui)
{
    if (inst.$hardware != null || inst.$hardware != undefined) {
        inst.inputSource = "External Pin";
        ui.inputSource.readOnly = true;
    }
    else {
        ui.inputSource.readOnly = false;
    }
}

/*
 *  ======== exports ========
 */
exports = {
    config: config,

    /* override generic requirements with  device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    moduleInstances: moduleInstances,

    filterHardware: filterHardware,

    onHardwareChanged: onHardwareChanged
};
