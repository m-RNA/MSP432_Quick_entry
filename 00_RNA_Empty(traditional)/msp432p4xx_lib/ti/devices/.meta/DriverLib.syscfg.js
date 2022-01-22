/*
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 */

/*
 *  ======== DriverLib.syscfg.js ========
 *  Support use of DriverLib within SysConfig meta-code
 */

"use strict";

/*
 *  ======== config ========
 *  User Config options
 */
let config = [
    {
        name: "path", /* override computed path to driverlib archive */
        displayName: "Library Path",
        description: "Relative path to the driverlib library",
        default: "",
        hidden: true
    }
];

/*
 *  ======== getAttrs ========
 *  Map a SysConfig deviceId to a set of "device family" attrs
 *
 *  @param deviceId  - a pinmux deviceId (system.deviceData)
 *
 *  @returns object - a set of device attributes of the form
 *      {
 *          deviceDir:    "", // name of the directory in /ti/devices/
 *                            // containing driverlib
 *          deviceDefine: ""  // DeviceFamily #define
 *          deviceGroup:  "", // one of "LPRF", "WIFI", "MSP", "TIVA"
 *      }
 */
function getAttrs(deviceId, part)
{
    var result = {};

    if (deviceId.match(/CC13.2/)) {
        result.deviceDir  = "cc13x2_cc26x2";
        result.deviceGroup = "LPRF";
        result.deviceDefine = "DeviceFamily_CC13X2";
    }
    else if (deviceId.match(/CC26.2/)) {
        result.deviceDir = "cc13x2_cc26x2";
        result.deviceGroup = "LPRF";
        result.deviceDefine = "DeviceFamily_CC26X2";
    }
    else if (deviceId.match(/CC13.0/)) { /* issue: remove, not suported?? */
        result.deviceDir = "cc13x0";
        result.deviceGroup = "LPRF";
        result.deviceDefine = "DeviceFamily_CC13X0";
    }
    else if (deviceId.match(/CC26.0R2/)) { /* issue: remove, not suported?? */
        result.deviceDir = "cc26x0r2";
        result.deviceGroup = "LPRF";
        result.deviceDefine = "DeviceFamily_CC26X0R2";
    }
    else if (deviceId.match(/CC26.0/)) { /* issue: remove, not suported?? */
        result.deviceDir = "cc26x0";
        result.deviceGroup = "LPRF";
        result.deviceDefine = "DeviceFamily_CC26X0";
    }
    else if (deviceId.match(/CC32/)) {
        result.deviceDir = "cc32xx";
        result.deviceGroup = "WIFI";
        result.deviceDefine = "DeviceFamily_CC3220";
    }
    else if (deviceId.match(/MSP432E/)) {
        result.deviceDir = "msp432e4";
        result.deviceGroup = "TIVA";
        if (part && part.match(/MSP432E411/)) {
            result.deviceDefine = "DeviceFamily_MSP432E411Y";
        }
        else {
            result.deviceDefine = "DeviceFamily_MSP432E401Y";
        }
    }
    else if (deviceId.match(/MSP432P4.1.I/) || deviceId.match(/MSP432P4111/)) {
        result.deviceDir = "msp432p4xx";
        result.deviceGroup = "MSP";
        result.deviceDefine = "DeviceFamily_MSP432P4x1xI";
    }
    else if (deviceId.match(/MSP432P4.1.T/)) { /* issue: remove, not suported? */
        result.deviceDir = "msp432p4xx";
        result.deviceGroup = "MSP";
        result.deviceDefine = "DeviceFamily_MSP432P4x1xT";
    }
    else if (deviceId.match(/MSP432P401/)) {
        result.deviceDir = "msp432p4xx";
        result.deviceGroup = "MSP";
        result.deviceDefine = "DeviceFamily_MSP432P401x";
    }
    else {
        result.deviceDir = "";
        result.deviceGroup = "";
        result.deviceDefine = "";
    }

    return (result);
}

/*
 *  ======== getLibs ========
 */
function getLibs(mod)
{
    /* get device and compiler toolchain from SysConfig */
    let devId     = system.deviceData.deviceId;
    let GenLibs   = system.getScript("/ti/utils/build/GenLibs");
    let tcDir     = GenLibs.getToolchainDir();

    /* compute device and toolchain-specific driverlib path */
    let templates = {
        LPRF: "ti/devices/{devDir}/driverlib/bin/{tcDir}/driverlib.lib",
        WIFI: "ti/devices/{devDir}/driverlib/{tcDir}/Release/driverlib.a",
        TIVA: "ti/devices/{devDir}/driverlib/lib/{tcDir}/m4f/{devDir}_driverlib.a",
        MSP:  "ti/devices/{devDir}/driverlib/{tcDir}/{devDir}_driverlib.lib"
    };

    let lib;
    if (mod.$static.path != "") {
        lib = mod.$static.path;
    }
    else {
        let attrs = getAttrs(devId);
        var template = templates[attrs.deviceGroup];
        if (template == null) {
            throw Error("device ID '" + devId
                + "' isn't from a supported group: '"
                + attrs.deviceGroup + "'" );
        }
        lib = template
            .replace("{tcDir}",   tcDir)
            .replace(/{devDir}/g, attrs.deviceDir);
    }

    /* create a GenLibs input argument */
    var libGroup = {
        name: "/ti/devices/driverlib",
        vers: "1.0.0.0",
        deps: [],
        libs: [lib]
    };

    return (libGroup);
}

/*
 *  ======== exports ========
 */
exports = {
    staticOnly   : true,
    displayName  : "DriverLib",

    templates    : {
        /* support library option generation for linker */
        "/ti/utils/build/GenLibs.cmd.xdt":
            {modName: "/ti/devices/DriverLib", getLibs: getLibs}
    },

    /* enable end-user to override computed library path */
    moduleStatic : {
        config: config
    },

    /* DriverLib-specific exports */
    getAttrs: getAttrs
};
