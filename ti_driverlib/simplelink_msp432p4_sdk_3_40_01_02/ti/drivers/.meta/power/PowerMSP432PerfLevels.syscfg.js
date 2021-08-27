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
 *  ======== PowerMSP432PerfLevels.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

let VLOCLK  = 9400;     // 9.4 kHz
let REFOCLK = 32768;    // selectable to alternative 128000
let LFXTCLK = 32768;    // Assumed freq if enabled.
let MODOSC  = 25000000; // 25 MHz;

let divChoices = [
    { name:   1 },
    { name:   2 },
    { name:   4 },
    { name:   8 },
    { name:  16 },
    { name:  32 },
    { name:  64 },
    { name: 128 }
];

/* Array of Performance Level configurations for MSP432 */
let config =  [
    {
        name        : "dcoFrequency",
        displayName : "DCO Frequency",
        description : "Specify the center frequency to use in Hertz for the"
            + " Digitally Controller Oscillator (DCO).",
        longDescription:`
Specifies a list of nominal center frequencies to use for the DCO. If
a __Custom__ frequency is required, configure the desired frequency using the
__Custom DCO Frequency__ configurable.
`,
        default     : 3000000,
        onChange    : calcOutClockFreq,
        options     :
        [
            {
                name: 0, displayName : "Custom",
                description: "Specify a custom frequency for the DCO.",
                longDescription: "You can specify the custom frequency using"
                    + " the __Custom Center Frequency__ configurable."
            },
            {
                name: 1500000, description: "1 MHz to 2 MHz"
            },
            {
                name: 3000000, description: "2 MHz to 4 MHz"
            },
            {
                name: 6000000, description: "4 MHz to 8 MHz"
            },
            {
                name: 12000000, description: "8 MHz to 16 MHz"
            },
            {
                name: 24000000, description: "16 MHz to 32 MHz"
            },
            {
                name: 48000000, description: "32 MHz to 64 MHz"
            }
        ]
    },
    {
        name        : "customDcoFrequency",
        displayName : "Custom DCO Frequency",
        description : "Custom center frequency to tune Digital Controlled"
            + " Oscillator (DCO)",
        default     : 3500000,
        onChange    : calcOutClockFreq,
        hidden      : true
    },
    {
        name        : "mclk",
        displayName : "MCLK",
        description : "Master Clock (MCLK) frequency for this performance level",
        longDescription:`
Master Clock (MCLK) is used by the CPU and peripheral module interfaces. The
clock source can be configured using the __MCLK Source__ configurable.
`,
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "mclkSource",
        displayName : "MCLK Source",
        description : "Master Clock (MCLK) source for this performance level.",
        default     : "DCO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "DCO"    },
            { name: "HFXT"   },
            { name: "LFXT"   },
            { name: "VLO"    },
            { name: "REFO"   },
            { name: "MODOSC" }
        ]
    },
    {
        name        : "mclkDivider",
        displayName : "MCLK Divider",
        description : "Master Clock (MCLK) divider",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },
    {
        name        : "hsmclk",
        displayName : "HSMCLK",
        description : "Subsystem Master Clock (HSMCLK) frequency for"
        +   " this performance level",
        longDescription:`
The Subsystem Master Clock (HSMCLK) is software selectable by individual
peripheral modules.
`,
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "hsmclkSource",
        displayName : "HSMCLK Source",
        description : "Subsystem Master Clock (HSMCLK) source.",
        longDescription:`
Specifies the HSMCLK source. The Low-Speed Subsytem Master Clock (SMCLK) uses
the same clock source as HSMCLK.
`,
        default     : "DCO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "DCO"    },
            { name: "HFXT"   },
            { name: "LFXT"   },
            { name: "VLO"    },
            { name: "REFO"   },
            { name: "MODOSC" }
        ]
    },
    {
        name        : "hsmclkDivider",
        displayName : "HSMCLK Divider",
        description : "Subsystem Master Clock (HSMCLK) divider.",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },
    {
        name        : "smclk",
        displayName : "SMCLK",
        description : "Low-Speed Subsystem Master Clock (SMCLK) frequency for"
            + " this performance level",
        longDescription:`
SMCLK can be divided independently from HSMCLK. SMCLK is limited in frequency
to half the rated maximum frequency of HSMCLK. SMCLK is software
selectable by individual peripheral modules.
`,
        default     : 3000000,
        readOnly    : true
    },
    {
        name        : "smclkDivider",
        displayName : "SMCLK Divider",
        description : "The Low-Speed Subsystem Master Clock (SMCLK) uses"
            + " the same clock source as HSMCLK.",
        longDescription:`
SMCLK can be divided independently from HSMCLK. SMCLK is limited in frequency
to half the rated maximum frequency of HSMCLK. SMCLK is software
selectable by individual peripheral modules.
`,
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },
    {
        name        : "aclk",
        displayName : "ACLK",
        description : "Auxiliary Clock (ACLK) frequency for this"
            + " performance level",
        longDescription: "ACLK is software selectable by individual peripheral"
            + " modules. ACLK is restricted to maximum frequency of"
            + " operation of 128 kHz.",
        default     : 32768,
        readOnly    : true
    },
    {
        name        : "aclkSource",
        displayName : "ACLK Source",
        description : "Auxiliary Clock (ACLK) source.",
        longDescription: "ACLK is software selectable by individual peripheral"
            + " modules. ACLK is restricted to maximum frequency of"
            + " operation of 128 kHz.",
        default     : "REFO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "LFXT" },
            { name: "VLO"  },
            { name: "REFO" }
        ]
    },
    {
        name        : "aclkDivider",
        displayName : "ACLK Divider",
        description : "Auxiliary Clock (ACLK) divider",
        longDescription: "ACLK is software selectable by individual peripheral"
            + " modules. ACLK is restricted to maximum frequency of"
            + " operation of 128 kHz.",
        default     : 1,
        onChange    : calcOutClockFreq,
        options     : divChoices
    },
    {
        name        : "bclk",
        displayName : "BCLK",
        description : "Low-Speed Backup Domain Clock (BCLK} frequency for"
            + " this performance level",
        longDescription: "BCLK is used primarily in the backup domain and is"
            + " restricted to a maximum frequency of 32.768 kHz",
        default     : 32768,
        readOnly    : true
    },
    {
        name        : "bclkSource",
        displayName : "BCLK Source",
        description : "Low-Speed Backup Domain Clock (BCLK} source.",
        longDescription: "BCLK is used primarily in the backup domain and is"
            + " restricted to a maximum frequency of 32.768 kHz",
        default     : "REFO",
        onChange    : calcOutClockFreq,
        options     :
        [
            { name: "LFXT" },
            { name: "REFO" }
        ]
    },
    {
        name        : "activeState",
        displayName : "Active State",
        description : "Active State for the Device",
        longDescription:`
Active states refer to any power state in which CPU execution is possible.
Two core voltage level settings are supported: __VCORE0__ and __VCORE1__. See
the __Core Voltage Level__ configurable for more on core voltage levels. Three
active modes are associated with each core voltage level. The various active
modes allow for optimal power and performance across a broad range of
application requirements. The core voltage can be supplied by either
a low dropout (__LDO__) regulator or a DC/DC (__DCDC__) regulator.

Low Frequency (LF) active states are useful for low-frequency operation. The
low-frequency active states limit the maximum frequency of operation to 128
kHz. The application must not perform flash program or erase operations or
any change to SRAM bank enable or retention enable configurations while in
low-frequency active states. This allows the power management system to be
optimized to support the lower power demands in these operating states.
Low-frequency active states are based on LDO operation only.
`,
        default     : "LDO",
        options     :
        [
            { name: "LDO", description: "Low Dropout" },
            { name: "DCDC", description: "DC/DC Regulator" },
            { name: "LF", description: "Low Frequency" }
        ]
    },
    {
        name        : "coreVoltageLevel",
        displayName : "Core Voltage Level",
        description : "Selects generation voltage for the device core",
        longDescription:`
The Power Supply System (PSS) uses an integrated voltage regulator to produce
a secondary core voltage (VCORE) from the primary voltage that is applied to
the device (VCC). In general, VCORE supplies the CPU, memories, and the
digital modules, while VCC supplies the I/Os and analog modules.
The VCORE output is maintained using a dedicated voltage reference.
VCORE voltage level is programmable to allow power savings if the maximum
device speed is not required. Modifying this configurable will impact
the max frequencies available for the __MCLK__, __HSMCLK__, and __SMCLK__.
`,
        default     : "VCORE0",
        options     :
        [
            { name : "VCORE0" },
            { name : "VCORE1" }
        ]
    },
    {
        name        : "flashWaitStates",
        displayName : "Flash Wait-States",
        description : "Number of Flash Wait-States",
        longDescription:`
The flash controller is configurable in terms of the number of memory
bus cycles it takes to service any read command. This allows the CPU
execution frequency to be higher than the maximum read frequency
supported by the flash memory. If the bus clock speed is higher than
the native frequency of the flash, the access is stalled for the
configured number of wait states, allowing data from the flash to
be accessed reliably.

> See the device data sheet for CPU execution frequency and
wait-state requirements.
`,
        default     : 0
    },
    {
        name        : "enableFlashBuffer",
        displayName : "Enable Flash Buffer",
        description : "Enable Flash Read Buffering",
        longDescription:`
When read buffering is enabled, the flash memory always reads an
entire 128-bit line irrespective of the access size of 8, 16, or 32
bits. The 128-bit data and its associated address is internally
buffered by the flash controller, so subsequent accesses (expected to
be contiguous in nature) within the same 128-bit address boundary are
serviced by the buffer. Hence, the flash accesses see wait-states only
when the 128-bit boundary is crossed, while read accesses within the
buffer's range are serviced without any bus stalls. If read buffering
is disabled, accesses to the flash bypasses the buffer, and the data
read from the flash is limited to the width of the access (8, 16, or
32 bits). Each bank has independent settings for the read buffering.
In addition, within each bank, the application has independent
flexibility to enable read buffering for instruction and data fetches.
Read buffers are bypassed during any program or erase operation by
the controller to ensure data coherency.
`,
        default     : false
    }
];

/*
 *  ======== getClockSourceFreq ========
 */
function getClockSourceFreq(clk, inst)
{
    let pow = inst.$ownedBy;

    switch (clk) {
        case 'VLO'   : { return VLOCLK; }
        case 'REFO'  : { return REFOCLK; }
        case 'MODOSC': { return MODOSC; }
        case 'DCO'   : {
            if(inst.dcoFrequency == 0) {
                return inst.customDcoFrequency;
            }
            else {
                return inst.dcoFrequency;
            }
        }
        case 'HFXT'  : { return pow.enableHFXTClock ? pow.hfxtFrequency : 0; }
        case 'LFXT'  : { return pow.enableLFXTClock ? LFXTCLK      : 0; }
    }
    return 0;
}

/*
 *  ======== calcOutClockFreq ========
 */
function calcOutClockFreq(inst, ui)
{
    let divm  = inst.mclkDivider;
    let divhs = inst.hsmclkDivider;
    let divs  = inst.smclkDivider;
    let diva  = inst.aclkDivider;

    if (inst.dcoFrequency === 0) {
        ui.customDcoFrequency.hidden = false;
    }
    else {
        ui.customDcoFrequency.hidden = true;
    }

    ui.mclk.readOnly = false;
    ui.hsmclk.readOnly = false;
    ui.smclk.readOnly = false;
    ui.bclk.readOnly = false;
    ui.aclk.readOnly = false;

    inst.mclk = getClockSourceFreq(inst.mclkSource, inst) / divm;
    inst.hsmclk  = getClockSourceFreq(inst.hsmclkSource, inst) / divhs;
    inst.smclk   = getClockSourceFreq(inst.hsmclkSource, inst) / divs;
    inst.bclk    = getClockSourceFreq(inst.bclkSource, inst);
    inst.aclk    = getClockSourceFreq(inst.aclkSource, inst) / diva;

    ui.mclk.readOnly = true;
    ui.hsmclk.readOnly = true;
    ui.smclk.readOnly = true;
    ui.bclk.readOnly = true;
    ui.aclk.readOnly = true;
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  param inst - PowerMSP432PerfLevels instance to be validated
 *  param vo   - Issue reporting object
 */
function validate(inst, vo)
{
    /* activeState, coreVoltageLevel, dcoFrequency,
     * flashWaitStates, enableFlashBuffer
     * DIV:  M, HS, S, A
     * SEL:  M, S, B, A
     * CLK:  M, HSM, SM, B, A
     */

    if (inst.ALCK > 128000) {
        logError(vo, inst, ["aclkSource", "aclkDivider"],
            'ACLK frequency must be 128kHz or lower.');
    }

    if (inst.BLCK > 32768) {
        logError(vo, inst, "bclkSource", 'BCLK frequency must be 32768 or lower.');
    }

    if (inst.smclk > 24000000) {
        logError(vo, inst, ["smclk", "smclkDivider"],
                 'SMCLK frequency must be 24000000 or lower.');
    }

    if (inst.coreVoltageLevel === "VCORE0") {
        if (inst.mclk > 24000000) {
            logError(vo, inst, "coreVoltageLevel",
                'MCLK frequency must be 24000000 or lower with VCORE0.');
        }
        if (inst.smclk > 12000000) {
            logError(vo, inst, "coreVoltageLevel",
                'SMCLK frequency must be 12000000 or lower, with VCORE0.');
        }
    }

    if (inst.activeState === 'LF') {
        if (inst.mclk   > 128000) {
            logError(vo, inst, ["activeState"],
            'MCLK frequency must be 128000 or lower, if active state is LF.');
        }
        if (inst.smclk  > 128000) {
            logError(vo, inst, ["activeState"],
            'SMCLK frequency must be 128000 or lower, if active state is LF.');
        }
        if (inst.hsmclk > 128000) {
            logError(vo, inst, ["activeState"],
            'HSMCLK frequency must be 128000 or lower, if active state is LF.');
        }
    }

    if (getClockSourceFreq(inst.mclkSource, inst) === 0) {
        logError(vo, inst, ["mclkSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.hsmclkSource, inst) === 0) {
        logError(vo, inst, ["hsmclkSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.bclkSource, inst) === 0) {
        logError(vo, inst, ["bclkSource"],
            'Selected clock source has not been configured.');
    }

    if (getClockSourceFreq(inst.aclkSource, inst) === 0) {
        logError(vo, inst, ["aclkSource"],
            'Selected clock source has not been configured.');
    }
}

/*
 *  ======== exports ========
 *  The exported objects for this module
 */
exports = {
    name        : "perfLevel",
    displayName : "MSP432P4 Performance Levels",
    description : "MSP432P4 performance levels used by the Power manager.",
    longDescription:`
MSP432 performance levels used by the Power manager.

* [Configuration Options][1]

[1]:/tidrivers/syscfg/html/ConfigDoc.html#PowerMSP432PerfLevels_Configuration_Options
`,
    validate    : validate,
    maxInstances: 8,
    config      : config
};
