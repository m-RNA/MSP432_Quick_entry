/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ADCBufDiffMSP432.syscfg.js ========
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
        signalTypes: { adcPin: ["AIN"] },
        resources: [{
            name: "adcDifferentialPin",
            displayName: "ADC Differential Pin",
            interfaceNames: adcChanNames
        }]
    };

    return ([adcBufChan]);
}

/*
 *  ======== exports ========
 */
exports = {
    pinmuxRequirements: pinmuxRequirements
};
