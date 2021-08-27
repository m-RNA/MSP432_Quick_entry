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
 *  ======== ADCBufMSP432.syscfg.js ========
 */

"use strict";

const MAXCHANNELS = 24; /* max number of channels per ADCBuf */

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "If the DMA is enabled, this is the DMA interrupt"
    + " priority. Otherwise, this is the ADC interrupt priority.";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADCBuf configuration
 */
let devSpecific = {
    config: [
        {
            name: "channels",
            displayName: "Channels",
            description: "Number of ADCBuf channels to configure. Must be"
                + " between 1 and " + MAXCHANNELS,
            default: 1
        },
        {
            name: "clockSource",
            displayName: "Clock Source",
            description: "Clock source used by the ADC peripheral",
            longDescription:`The frequency of the clock sources are configured
per __Power Performance Level__ configured in the __Power Module__.
`,
            default: "ADC",
            options: [
                { name: "ADC" },
                { name: "SYSOSC" },
                { name: "ACLK" },
                { name: "MCLK" },
                { name: "SMCLK" },
                { name: "HSMCLK" }
            ]
        },
        {
            name: "enableDMA",
            displayName: "Enable DMA",
            description: "Specifies if the DMA is used.",
            default: true
        },
        {
            name: "triggerSource",
            displayName: "Trigger Source",
            description: "Specifies the trigger to initiate a sample sequence.",
            default: "Timer",
            options: [
                { name: "Timer" },
                { name: "Auto" }
            ],
            onChange: onTriggerSourceChange
        },
        {
            name: "timerDutyCycle",
            displayName: "Timer Duty Cycle",
            description: "Specifies the timer duty cycle as a percentage"
                + " between 1 and 99.",
            default: 50
        },
        {
            name: "timerCaptureCompare",
            displayName: "Timer Capture Compare",
            description: "Specifies the capture compare number to use for the"
                + " specified timer peripheral used as the trigger source",
            default: 1,
            options: [
                { name: 1 }, { name: 2 }
            ]
        },
        intPriority
    ],

    moduleInstances: moduleInstances,

    /* override generic requirements with  device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc: "/ti/drivers/adcbuf/ADCBufMSP432.Board.c.xdt",
        boardh: "/ti/drivers/adcbuf/ADCBufMSP432.Board.h.xdt"
    },

    _getADCPinResources : _getADCPinResources
};

/*
 *  ======== _getADCPinResources ========
 *  Assume 'inst' is a channel
 */
function _getADCPinResources(inst)
{
    let pin;

    if (inst.inputSource) {
        if (inst.inputSource === "Internal Temperature") {
            return ("Temperature Sensor, ADC14 TCMAP");
        }
        else if (inst.inputSource === "Internal Battery") {
            return ("Battery Voltage, ADC14 BATMAP");
        }
    }

    if (inst.inputSource === "External Pin") {
        let adcPin = inst.adc.adcPin.$solution.devicePinName;
        pin = adcPin.match(/P\d\.\d/)[0];
    }

    if (inst.$hardware && inst.$hardware.displayName) {
        pin = "\n" + pin + ", " + inst.$hardware.displayName;
    }

    if (inst.mode === "Differential") {
        let adcPin = inst.differentialPin.adc.adcDifferentialPin.$solution.devicePinName;
        pin = "\n" + pin + "\nDifferential Pin: " + adcPin.match(/P\d\.\d/)[0];
    }

    return (pin);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{

    let reqs = [];

    let timer = {
        name: "timer",
        displayName: "Timer peripheral",
        description: "Timer peripheral used as the trigger source.",
        interfaceName: "TIMER_A",
        resources: []
    };

    let adc = {
        name: "adc",
        displayName: "ADC peripheral",
        hidden: true,
        interfaceName: "ADC14",
        canShareWith: "ADCBuf",
        resources: []
    };

    let dma = [
        {
            name: "dmaInterruptNumber",
            displayName: "DMA Interrupt Number",
            interfaceNames: [ "DMA_INT" ]
        },
        {
            name: "dmaChannel",
            displayName: "DMA Channel",
            hidden: true,
            readOnly: true,
            interfaceNames: [ "DMA_CH" ]
        }
    ];

    if (inst.triggerSource === "Timer") {
        reqs.push(timer);
    }

    if (inst.enableDMA) {
        adc.resources = dma;
    }

    reqs.push(adc);

    return (reqs);
}

/*
 *  ======== onTriggerSourceChange ========
 */
function onTriggerSourceChange(inst, ui)
{
    if (inst.triggerSource !== "Timer") {
        ui.timerDutyCycle.hidden = true;
        ui.timerCaptureCompare.hidden = true;
    }
    else {
        ui.timerDutyCycle.hidden = false;
        ui.timerCaptureCompare.hidden = false;
    }
}

/*
 *  ======== moduleInstances ========
 *  returns an array of ADCBufChanMSP432 instances.
 */
function moduleInstances(inst)
{
    let result = [];

    /* limit the loop because validate is not called before this method */
    let max = Math.min(inst.channels, MAXCHANNELS);
    for (let i = 0; i < max; i++) {
        result.push({
            name: "adcBufChannel" + i,
            displayName: "ADCBuf Channel " + i,
            moduleName: "/ti/drivers/adcbuf/ADCBufChanMSP432",
            hardware: inst.$hardware,
            hidden: false
        });
    }

    return (result);
}

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  @param inst - ADCBuf instance to be validated
 *  @param vo - object to hold detected validation issues
 *
 *  @param $super - needed to call the generic module's functions
 */
function validate(inst, vo, $super)
{
    if ($super.validate) {
        $super.validate(inst, vo);
    }

    if (inst.timerDutyCycle > 99 || inst.timerDutyCycle < 1
        || !Number.isInteger(inst.timerDutyCycle)) {
        Common.logError(vo, inst, "timerDutyCycle",
            "Duty cycle must be an integer between 1 and 99.");
    }

    if (inst.channels > MAXCHANNELS || inst.channels < 1
        || !Number.isInteger(inst.channels)) {
        Common.logError(vo, inst, "channels",
            "Channels must be an integer between 1 and " + MAXCHANNELS + ".");
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ADCBuf module to
 *  allow us to augment/override as needed for the MSP432
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

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ADCBuf module */
    extend: extend
};
