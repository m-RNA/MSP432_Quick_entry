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
 *
 */

/*
 *  ======== Board.syscfg.js ========
 */

/* Module used to invoke Board.c and Board.h templates */

"use strict";

/* get ti/drivers common utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== getLibs ========
 */
function getLibs(mod)
{
    const FAMILY2LIBS = {
        CC26X2: [
            "ti/drivers/lib/drivers_cc26x2.aem4f",
            "ti/dpl/lib/dpl_cc26x2.aem4f"
        ],
        CC13X2: [
            "ti/drivers/lib/drivers_cc13x2.aem4f",
            "ti/dpl/lib/dpl_cc13x2.aem4f"
        ],
        CC26X0: [
            "ti/drivers/lib/drivers_cc26x0.aem3",
            "ti/dpl/lib/dpl_cc26x0.aem3"
        ],
        CC26X0R2: [
            "ti/drivers/lib/drivers_cc26x0r2.aem3",
            "ti/dpl/lib/dpl_cc26x0r2.aem3"
        ],
        CC13X0: [
            "ti/drivers/lib/drivers_cc13x0.aem3",
            "ti/dpl/lib/dpl_cc13x0.aem3"
        ],
        CC32XX: [
            "ti/drivers/lib/drivers_cc32xx.aem4",
            "ti/dpl/lib/dpl_cc32xx.aem4"
        ],
        MSP432E4: [
            "ti/drivers/lib/drivers_msp432e4.aem4f",
            "ti/dpl/lib/dpl_msp432e4.aem4f"
        ],
        MSP432P4x1xI: [
            "ti/drivers/lib/drivers_msp432p4x1xi.aem4f",
            "ti/dpl/lib/dpl_msp432p4x1xi.aem4f"
        ],
        MSP432P4: [
            "ti/drivers/lib/drivers_msp432p4xx.aem4f",
            "ti/dpl/lib/dpl_msp432p4xx.aem4f"
        ]
    };

    /* get device ID to select appropriate libs */
    let devId = system.deviceData.deviceId;

    /* get device information from DriverLib */
    var DriverLib = system.getScript("/ti/devices/DriverLib");
    let attrs = DriverLib.getAttrs(devId);

    /* select a device family-specific set of libs */
    let libs;
    let family = attrs.deviceDefine;
    if (family != "") {
        family = family.replace(/^DeviceFamily_/, "");
        if (family.indexOf("MSP432E") == 0) {
            family = "MSP432E4";
        }
        else if (family.indexOf("MSP432P401") == 0) {
            family = "MSP432P4";
        }
        else if (family.indexOf("CC32") == 0) {
            family = "CC32XX";
        }
        libs = FAMILY2LIBS[family];
    }
    if (libs == null) {
        throw Error("device2LinkCmd: unknown device family ('"
            + family + "') for deviceId '" + devId + "'");
    }

    /* create a GenLibs input argument */
    var linkOpts = {
        name: "/ti/drivers",
        vers: "1.0.0.0",
        deps: ["/ti/devices/driverlib"],
        libs: libs
    };

    return (linkOpts);
}

/*
 *  ======== base ========
 */
let base = {
    displayName  : "Board",
    staticOnly   : true,

    templates    : {
        /* contribute TI-DRIVERS libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt"   :
            {modName: "/ti/drivers/Board", getLibs: getLibs},

        /* trigger generation of ti_drivers_config.[ch] */
        "/ti/drivers/templates/Board.c.xdt" : true,
        "/ti/drivers/templates/Board.h.xdt" : true
    },

    moduleStatic : {
        /* ensure somthing supplies appropriate DriverLib library */
        modules  : Common.autoForceModules(["/ti/devices/DriverLib"]),
        config   : []
    }
};

/*
 *  ======== exports ========
 */
exports = base;
