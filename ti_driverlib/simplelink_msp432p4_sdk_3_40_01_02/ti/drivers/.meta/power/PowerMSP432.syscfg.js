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
 * ======== PowerMSP432.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let isCName  = Common.isCName;

let priIntCS             = Common.newIntPri()[0];
    priIntCS.name        = "clockSystemInterruptPriority";
    priIntCS.displayName = "Clock System Interrupt Priority";
    priIntCS.hidden      = true;

/* Array of power configurables for this device family */
let config = [
    {
        name        : "enablePolicy",
        displayName : "Enable Policy",
        description : "Enable the power policy to run when the CPU is idle.",
        longDescription:`
If enabled, the policy function will be invoked once for each pass of the
idle loop.

In addition to this static setting, the Power Policy can be dynamically
enabled and disabled at runtime, via the [Power_enablePolicy()][1] and
[Power_disablePolicy()][2] functions, respectively.

[1]: /tidrivers/doxygen/html/_power_8h.html#ti_drivers_Power_Examples_enable
"Enabling power policy"
[2]: /tidrivers/doxygen/html/_power_8h.html#ti_drivers_Power_Examples_disable
"Disabling power policy"
`,
        onChange    : onChangeEnablePolicy,
        default     : true
    },
    {
        name        : "policyFunction",
        displayName : "Policy Function",
        description : "Power policy function called from the idle loop.",
        longDescription:`
When enabled, this function is invoked in the idle loop, to opportunistically
select and activate sleep states.

Two reference policies are provided:

* __PowerMSP432_sleepPolicy__
* __PowerMSP432_deepSleepPolicy__

In addition to this static selection, the Power Policy can be
dynamically changed at runtime, via the Power_setPolicy() API.
`,
        onChange    : onChangePolicyFxn,
        default     : "PowerMSP432_sleepPolicy",
        options     :
        [
            {
                name: "PowerMSP432_sleepPolicy",
                description: "A simple policy that will put the device in"
                    + " either the PowerMSP432_SLEEP state (unless there is a"
                    + " constraint prohibiting this), or CPU wait for"
                    + " interrupt (WFI)."
            },
            {
                name: "PowerMSP432_deepSleepPolicy",
              description: "An agressive policy that checks constraints and"
                + " optionally chooses PowerMSP432_DEEPSLEEP_1,"
                + " PowerMSP432_DEEPSLEEP_0, PowerMSP432_SLEEP, or WFI, in"
                + " that order of preference."
            },
            {
                name: "Custom",
                description: "Custom policies can be written, and specified"
                    + " via the Policy Custom Function configuration."
            }
        ]
    },
    {
        name        : "policyCustomFunction",
        displayName : "Policy Custom Function",
        description : "User provided Power policy function. Usage not typical.",
        default     : "customPolicyFxn",
        hidden      : true
    },
    {
        name        : "policyInitFunction",
        displayName : "Policy Init Function",
        description : "The initialization function for the Power policy.",
        longDescription:`
This is an optional policy initialization function that will be called during
application startup, during Power Manager initialization.
`,
        onChange    : onChangePolicyInitFxn,
        default     : "PowerMSP432_initPolicy",
        options     :
        [
            { name: "PowerMSP432_initPolicy" },
            {
                name: "Custom",
                description: "Custom initialization policies can be written"
                    + "and specified via the Policy Init Custom Function"
                    + " configuration."
            }
        ]
    },
    {
        name        : "policyInitCustomFunction",
        displayName : "Policy Init Custom Function",
        description : "User provided Power policy init function. " +
                      "Usage not typical.",
        default     : "customPolicyInitFxn",
        hidden      : true
    },
    {
        name        : "enablePinParking",
        displayName : "Enable Pin Parking",
        description : "Enable automatic pin parking during LPM3 and LPM4.",
        longDescription:`
Specifies if internal pull resistors should be automatically applied to input
pins during PowerMSP432_DEEPSLEEP_0 and PowerMSP432_DEEPSLEEP_1.
`,
        default     : false
    },
    {
        name        : "resumeShutdownHookFunction",
        displayName : "Resume Shutdown Hook Function",
        description : "Optional hook function to be called on resume from "
                       + "shutdown",
        longDescription:`
This function is called when the device wakes and resumes from a
previous entry into either the SHUTDOWN_0 or SHUTDOWN_1 states.
If this function is defined (i.e., not an empty string), it will
be called during Power_init(), right before the Power driver clears
the LOCKLPM5 and LOCKBKUP bits in the PMCTL1 register, to unlock I/Os
and the backup domain.
`,
        placeholder : "Enter a function name to enable",
        default     : ""
    },
    {
        name        : "initialPerformanceLevel",
        displayName : "Initial Performance Level",
        description : "The performance level at system start up.",
        longDescription:`
Levels 0-3 are always available. Levels 4-7 are available if
__Custom Performance Levels__ are defined.
`,
        default     : 2,
        options     :
        [
            { name: 0 },
            { name: 1 },
            { name: 2 },
            { name: 3 },
            { name: 4 },
            { name: 5 },
            { name: 6 },
            { name: 7 }
        ]
    },
    {
        name        : "enablePerformanceLevels",
        displayName : "Enable Performance Levels",
        description : "Enable performance level control. Strongly recommended.",
        longDescription:
`
If enabled, the performance scaling feature will be enabled for the
application. The __Initial Performance Level__ will be established
during Power Manager initialization.

See the __Performance Level Configuration__ for more information.
`,
        onChange    : onChangeEnablePerf,
        default     : true
    },
    {
        name        : "customPerformanceLevels",
        displayName : "Custom Performance Levels",
        description : "Advanced option. Select the number of Custom"
            + " Performance Levels to configure.",
        longDescription:"See the __Performance Level Configuration__ for more"
            + " information.",
        default     : 0,
        onChange    : onChangeCustomLevels,
        options     :
        [
            { name: 0 },
            { name: 1 },
            { name: 2 },
            { name: 3 },
            { name: 4 }
        ]
    },
    {
        name        : "advancedConfiguration",
        displayName : "Advanced Configuration",
        description : "Show Advanced Power Configuration",
        onChange    : onChangeUseExtendedPerf,
        default     : false
    },
    {
        name        : "clockSystemCallback",
        displayName : "Clock System Callback",
        description : "Clock System (CS) callback function to serve as the"
            + " interrupt service routine.",
        longDescription:`
The function specified will be configured for catching Clock System (CS)
interrupts. If left empty, interrupts from the CS will not be enabled.
`,
        onChange    : onChangeIsrCS,
        hidden      : true,
        placeholder : "Enter a function name to enable",
        default     : ""
    },
    priIntCS,
    {
        name        : "enableHFXTClock",
        displayName : "Enable HFXT Clock",
        description : "Enable high frequency crystal (HFXT) clock.",
        longDescription:`
If enabled, the high frequency crystal (HFXT) clock pin will be configured
for HFXT function.
`,
        onChange    : onChangeConfigurePinHFXT,
        hidden      : true,
        default     : false
    },
    {
        name        : "bypassHFXT",
        displayName : "Bypass HFXT",
        description : "Configure high frequency crystal (HFXT) pin for bypass",
        longDescription:`
If enabled, the high frequency crystal (HFXT) pin will be configured for
HFXT bypass.
`,
        hidden      : true,
        default     : false
    },
    {
        name        : "hfxtFrequency",
        displayName : "HFXT Frequency",
        description : "High frequency crystal (HFXT) frequency",
        longDescription:`
The source is specified via an enumerated value from cs.h, for
example __CS_48MHZ__, __CS_24MHZ__, etc..
`,
        hidden      : true,
        default     : 48000000,
        options     :
        [
            { name:  1000000, displayName : "1 MHz" },
            { name:  1500000, displayName : "1.5 MHz" },
            { name:  3000000, displayName : "3 MHz" },
            { name:  4000000, displayName : "4 MHz" },
            { name:  6000000, displayName : "6 MHz" },
            { name:  8000000, displayName : "8 MHz" },
            { name: 12000000, displayName : "12 MHz" },
            { name: 16000000, displayName : "16 MHz" },
            { name: 24000000, displayName : "24 MHz" },
            { name: 32000000, displayName : "32 MHz" },
            { name: 40000000, displayName : "40 MHz" },
            { name: 48000000, displayName : "48 MHz" }
        ]
    },
    {
        name        : "enableLFXTClock",
        displayName : "Enable LFXT Clock",
        description : "Enable low frequency crystal (LFXT) clock",
        longDescription:`
If enabled, the low frequency crystal (LFXT) clock pin will be configured
for LFXT function.
`,
        onChange    : onChangeConfigurePinLFXT,
        hidden      : true,
        default     : false
    },
    {
        name        : "bypassLFXT",
        displayName : "Bypass LFXT",
        description : "Configure low frequency crystal (LFXT) pin for bypass",
        longDescription:`
If enabled, the low frequency crystal (LFXT) pin will be configured for
LFXT bypass.
`,
        hidden      : true,
        default     : false
    },
    {
        name        : "lfxtDriveLevel",
        displayName : "LFXT Drive Level",
        description : "The low frequency crystal (LFXT) drive level",
        longDescription:
`The source is specified via an enumerated value from __cs.h__, for example:
__CS_LFXT_DRIVE3__, __CS_LFXT_DRIVE2__, etc.
`,
        default     : 0,
        hidden      : true,
        options     :
        [
            {
                name: 0,
                description: "CS_LFXT_DRIVE0"
            },
            {
                name: 1,
                description: "CS_LFXT_DRIVE1"
            },
            {
                name: 2,
                description: "CS_LFXT_DRIVE2"
            },
            {
                name: 3,
                description: "CS_LFXT_DRIVE3"
            }
        ]
    }
];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Power configuration
 */
let devSpecific = {
    displayName         : "Power",
    getClockFrequencies : getClockFrequencies,

    moduleStatic : {
        config          : config,
        validate        : validate,
        moduleInstances : moduleInstances,
        modules: Common.autoForceModules(["Board"])
    },

    templates : {
        boardc     : "/ti/drivers/power/PowerMSP432.Board.c.xdt",
        board_initc: "/ti/drivers/power/PowerMSP432.Board_init.c.xdt"
    }
};

/*
 *  ======== moduleInstances ========
 *  Return array of other module instances required by the PowerMSP432 module
 *  The first four instances correspond to the pre-defined performance levels
 *  defined in the driver source.  Additional instances are for any custom
 *  levels defined by the user.
 */
function moduleInstances(inst)
{

    let moduleName = "/ti/drivers/power/PowerMSP432PerfLevels";
    let moduleDescription = "Read Only. Nominal for MSP432P4.";
    let levelDefs = [];

    for (let i = 1; i <= inst.customPerformanceLevels; ++i) {
        let customNumber = i + 3;
        levelDefs.push({
            moduleName  : moduleName,
            name        : "performanceLevel" + customNumber,
            displayName : "Performance Level " + customNumber,
            description : "Custom Performance Level"
        });
    }

    levelDefs.push(
        {
            moduleName : moduleName,
            readOnly   : true,
            name       : "performanceLevel3",
            displayName: "Performance Level 3",
            description: moduleDescription,
            args       : {
                mclk              : 48000000,
                hsmclk            : 48000000,
                smclk             : 24000000,
                bclk              : 32768,
                aclk              : 32768,
                activeState       : 'DCDC',
                coreVoltageLevel  : "VCORE1",
                dcoFrequency      : 48000000,
                mclkSource        : 'DCO',
                mclkDivider       : 1,
                hsmclkSource      : 'DCO',
                hsmclkDivider     : 1,
                smclkDivider      : 2,
                bclkSource        : 'REFO',
                aclkSource        : 'REFO',
                aclkDivider       : 1,
                flashWaitStates   : 1,
                enableFlashBuffer : true
            }
        },
        {
            moduleName : moduleName,
            name       : "performanceLevel2",
            displayName: "Performance Level 2",
            description: moduleDescription,
            readOnly   : true,
            args       : {
                mclk              : 48000000,
                hsmclk            : 24000000,
                smclk             : 12000000,
                bclk              : 32768,
                aclk              : 32768,
                activeState       : 'DCDC',
                coreVoltageLevel  : "VCORE1",
                dcoFrequency      : 48000000,
                mclkSource        : 'DCO',
                mclkDivider       : 1,
                hsmclkSource      : 'DCO',
                hsmclkDivider     : 2,
                smclkDivider      : 4,
                bclkSource        : 'REFO',
                aclkSource        : 'REFO',
                aclkDivider       : 1,
                flashWaitStates   : 1,
                enableFlashBuffer : true
            }
        },
        {
            moduleName : moduleName,
            name       : "performanceLevel1",
            displayName: "Performance Level 1",
            description: moduleDescription,
            readOnly   : true,
            args       : {
                mclk              : 24000000,
                hsmclk            : 6000000,
                smclk             : 6000000,
                bclk              : 32768,
                aclk              : 32768,
                activeState       : 'DCDC',
                coreVoltageLevel  : "VCORE0",
                dcoFrequency      : 24000000,
                mclkSource        : 'DCO',
                mclkDivider       : 1,
                hsmclkSource      : 'DCO',
                hsmclkDivider     : 4,
                smclkDivider      : 4,
                bclkSource        : 'REFO',
                aclkSource        : 'REFO',
                aclkDivider       : 1,
                flashWaitStates   : 1,
                enableFlashBuffer : true
            }
        },
        {
            moduleName : moduleName,
            name       : "performanceLevel0",
            displayName: "Performance Level 0",
            description: moduleDescription,
            readOnly   : true,
            args       : {
                mclk              : 12000000,
                hsmclk            : 3000000,
                smclk             : 3000000,
                bclk              : 32768,
                aclk              : 32768,
                activeState       : 'DCDC',
                coreVoltageLevel  : "VCORE0",
                dcoFrequency      : 12000000,
                mclkSource        : 'DCO',
                mclkDivider       : 1,
                hsmclkSource      : 'DCO',
                hsmclkDivider     : 4,
                smclkDivider      : 4,
                bclkSource        : 'REFO',
                aclkSource        : 'REFO',
                aclkDivider       : 1,
                flashWaitStates   : 0,
                enableFlashBuffer : false
            }
        }
    );

    return levelDefs;
}

/*
 *  ======== onChangeEnablePolicy ========
 *  onChange callback function for the enablePolicy config
 */
function onChangeEnablePolicy(inst, ui)
{
    let subState = !inst.enablePolicy;

    ui.enablePinParking.hidden = subState;
    ui.policyInitFunction.hidden = subState;
    ui.policyFunction.hidden = subState;

    onChangePolicyInitFxn(inst,ui);
    onChangePolicyFxn(inst,ui);
}

/*
 *  ======== onChangeEnablePerf ========
 *  onChange callback function for the enablePerformanceLevels config
 */
function onChangeEnablePerf(inst, ui)
{
    let subState = !inst.enablePerformanceLevels;

    ui.initialPerformanceLevel.hidden = subState;
    ui.customPerformanceLevels.hidden = subState;
    ui.advancedConfiguration.hidden = subState;

    onChangeCustomLevels(inst, ui);
    onChangeUseExtendedPerf(inst, ui);

    for (let i = 0; i < (4 + Number(inst.customPerformanceLevels)); i++) {
        ui["performanceLevel" + i].hidden = subState;
    }
}

/*
 *  ======== onChangeUseExtendedPerf ========
 *  onChange callback function for the onChangeUseExtendedPerf config
 */
function onChangeUseExtendedPerf(inst,ui)
{
    let subState = !inst.advancedConfiguration;

    ui.clockSystemCallback.hidden    = subState;
    ui.enableHFXTClock.hidden   = subState;
    ui.enableLFXTClock.hidden   = subState;

    onChangeConfigurePinHFXT(inst, ui);
    onChangeConfigurePinLFXT(inst, ui);
    onChangeIsrCS(inst, ui);
}

/*
 *  ======== onChangeConfigurePinHFXT ========
 *  onChange callback function for the onChangeConfigurePinHFXT config
 */
function onChangeConfigurePinHFXT (inst, ui)
{
    let subState = !inst.enableHFXTClock;

    ui.bypassHFXT.hidden = subState;
    ui.hfxtFrequency.hidden   = subState;
}

/*
 *  ======== onChangeConfigurePinLFXT ========
 *  onChange callback function for the onChangeConfigurePinLFXT config
 */
function onChangeConfigurePinLFXT (inst, ui)
{
    let subState = !inst.enableLFXTClock;

    ui.bypassLFXT.hidden  = subState;
    ui.lfxtDriveLevel.hidden   = subState;
}

/*
 *  ======== onChangeIsrCS ========
 *  onChange callback function for the onChangeIsrCS config
 */
function onChangeIsrCS (inst, ui)
{
    let subState = (inst.clockSystemCallback == '');
    ui.clockSystemInterruptPriority.hidden = subState;
}

/*
 *  ======== onChangeCustomLevels ========
 *  onChange callback function for the customPerformanceLevels config
 */
function onChangeCustomLevels(inst, ui)
{
    /* TODO: Need to update the enum for Initial Performance Level */
}

/*
 *  ======== onChangePolicyInitFxn ========
 *  onChange callback function for the policyInitFunction config
 */
function onChangePolicyInitFxn(inst, ui)
{
    let subState = inst.policyInitFunction !== 'Custom';

    ui.policyInitCustomFunction.hidden = subState;
}

/*
 *  ======== onChangePolicyFxn ========
 *  onChange callback function for the policyFunction config
 */
function onChangePolicyFxn(inst, ui)
{
    let subState = inst.policyFunction !== 'Custom';

    ui.policyCustomFunction.hidden = subState;
}

/*
 *  ======== numericCompare ========
 *  Provides a numeric comparison function for use in sort algorithms
 */
function numericCompare(a, b)
{
    return (a - b);
}

/*
 *  ======== firstUniq ========
 *  Determine if a list element is the 1st unique copy of that element value.
 */
function firstUniq (element, index, list)
{
    return (list.indexOf(element) === index);
}

/*
 *  ======== getClockFrequencies ========
 *  Returns a sorted list of unique frequencies that a named clock source may
 *  have across any supported performance level.
 *
 *  param clockSource     - The clock source name
 */
function getClockFrequencies(clockSource)
{
    let freqArray  = [];
    let clock = clockSource.toLowerCase();
    let inst = null;

    if ("/ti/drivers/Power" in system.modules) {
        inst = system.modules["/ti/drivers/Power"].$static;
    }

    if (inst !== null && inst.enablePerformanceLevels == true) {
        for (let level = 0; level < 8; ++level) {
            let levelName = "performanceLevel" + level;

            if (levelName in inst) {
                freqArray.push(getClock(inst, level, clock));
            }
            else {
                break;
            }
        }
    }
    else {
        freqArray.push(rawBootClock(clock));
    }

    freqArray.sort(numericCompare);
    return (freqArray.filter(firstUniq));
}

/*
 *  ======== rawBootClock ========
 *  Return the raw (non driver modified) frequency of a clock on boot.
 */
function rawBootClock(clock)
{
    switch (clock) {
        case 'ACLK':
        case 'BCLK':
        case 'aclk':
        case 'bclk':
            return (32768);
        default:
            return (3000000);
    }
}

/*
 *  ======== getClock ========
 *  Return the raw (non driver modified) frequency of a clock on boot.
 *
 *  param inst   - Power instance
 *  param level  - a performance level
 *  param clock  - A clock name
 *
 *  returns freq - The frequency of clock in the specified performance level.
 */
function getClock(inst, level, clock)
{
    if (level === 'none') {
        return (rawBootClock(clock));
    }

    let perfLevelName = 'performanceLevel' + level;
    return (inst[perfLevelName][clock]);
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  param inst - Power instance to be validated
 *  param vo   - Issue reporting object
 *
 */
function validate(inst, vo)
{
    if (inst.enablePolicy) {
        if (inst.policyInitFunction === 'Custom') {
            if (!isCName(inst.policyInitCustomFunction)) {
                logError(vo, inst, "policyInitCustomFunction",
                    "Not a valid C identifier");
            }
            if (inst.policyInitCustomFunction === '') {
                logError(vo, inst, "policyInitCustomFunction",
                    "Must contain a valid function name if the " +
                    "Policy Init Function field == 'Custom'");
            }
        }
        if (inst.policyFunction === 'Custom') {
            if (!isCName(inst.policyCustomFunction)) {
                logError(vo, inst, "policyCustomFunction",
                    "is not a valid C identifier");
            }
            if (inst.policyCustomFunction === '') {
                logError(vo, inst, "policyCustomFunction",
                    "Must contain a valid function name if the " +
                    "Policy Function field == 'Custom'");
            }
        }
    }
    if ((inst.enableHFXTClock)) {
        if ((inst.hfxtFrequency == '0')) {
            if (inst.bypassHFXT) {
                logError(vo, inst, 'hfxtFrequency',
                         'If HFXT is enabled in bypass mode, please specify ' +
                         'the external crystal square wave frequency.');
            } else {
                logError(vo, inst, 'hfxtFrequency',
                         'Specify the desired HFXT oscillator frequency.');
            }
        }
    }

    if ((inst.initialPerformanceLevel - inst.customPerformanceLevels) > 3) {
        logError(vo, inst, ['initialPerformanceLevel', 'customPerformanceLevels'],
                'The initial performance level refers to an undefined custom ' +
                'performance level.');
    }

    if (!isCName(inst.resumeShutdownHookFunction)) {
        logError(vo, inst, "resumeShutdownHookFunction", 'Not a valid C identifier');
    }

    if (inst.advancedConfiguration) {
        if (!isCName(inst.clockSystemCallback)) {
            logError(vo, inst, "clockSystemCallback", 'Not a valid C identifier');
        }
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Power module to
 *  allow us to augment/override as needed for the MSP432
 */
function extend(base)
{
    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ADCBuf module */
    extend: extend
};
