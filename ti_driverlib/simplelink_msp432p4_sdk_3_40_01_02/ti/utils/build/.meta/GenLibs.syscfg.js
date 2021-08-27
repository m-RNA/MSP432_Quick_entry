/*
 *  ======== GenLibs.syscfg.js ========
 *  User configuration of the contents produced by the GenLibs.cmd.xdt template
 *
 *  GenLibs.cmd.xdt
 *  ---------------
 *  The GenLibs.cmd.xdt template generates a linker command file that names
 *  _all_ libraries required to link the target code configured by SysConfig.
 *  This linker command file can be added to the linker's command line and
 *  frees the user from having to explicitly name (and maintain) a changing
 *  list of libraries.
 *
 *  How it works
 *  ------------
 *  Any module in a SysConfig configuration can contribute one or more
 *  libraries to a generated linker command file, named
 *  ti_utils_build_linker.cmd.
 *
 *  Products typically include one or more SysConfig modules that, as a
 *  function of the current configuration, specify a list of libraries
 *  (and their dependencies) with which to link.
 *
 *  Modules contribute libraries by:
 *      1. defining a "getLibs" function that returns libraries and their
 *         dependencies
 *      2. declaring GenLibs.cmd.xdt in its exports.templates object; e.g.,
 *         a module named "/ti/ndk/NDK" would add the following:
 *           templates: {
 *               "/ti/utils/build/GenLibs.cmd.xdt": {
 *                   modName: "/ti/ndk/NDK",
 *                   getLibs: getLibs
 *               }
 *           }
 *
 *  The GenLibs.cmd.xdt then, as a function of the SysConfig specified
 *  toolchain, generates a linker command file containing the appropriate
 *  library linker options in the order dictated by the specified
 *  dependencies.  For example, the TI-DRIVERS libraries must precede the
 *  DriverLib library and the NDK libraries must preceed the TI-DRIVERS
 *  libraries.
 *
 *  The GenLibs.cmd.xdt template is ONLY expanded when both
 *     o the GenLibs.cmd.xdt template is declared in a product.json, and
 *     o either product.json specifies '"alwaysRun": true' for
 *      GenLibs.cmd.xdt, or at least one module in a configuration declares
 *      GenLibs.cmd.xdt in its exports.templates object.
 *
 *  Modules that name the GenLibs.cmd.xdt template must also pass an argument
 *  that defines the libraries required for the current configuration.
 *  This argument is an object of the form:
 *    {
 *        modName: "/ti/ndk/Global",   // name of the module to pass to getLibs
 *        getLibs: function (mod){...} // function to call to get library info
 *    }
 *
 *  NOTE: In the future modName will not need to be specified and, as a
 *        result, ONLY the getLibs() function itself needs to be passed to
 *        GenLibs.cmd.xdt.  See JIRA ????
 *
 *  When called, the getLibs() function is passed the module object
 *  corresponding to the specified by modName, and must return an object
 *  defining both the libraries with which to link as well as any
 *  dependencies these libraries have.  Specifically:
 *    {
 *      name: "/ti/ndk",       // name of this "library group"
 *      vers: "1.0.0.0"        // (optional) version of this library group
 *      libs: ["ti/...", ...], // array of physical library names provided
 *      deps: [                // array of lib groups required by libs above
 *        "/ti/drivers",
 *           :
 *      ]
 *    }
 *
 *  Libraries are specified using relative paths to each library to pass to
 *  the linker.  Paths should always use '/' to separate folder names (NEVER
 *  use backslashes).  Libraries are found relative to the directories named
 *  by the product's include paths (i.e., paths named by the includePaths
 *  array in the product's product.json or a designated application project
 *  folder).
 *
 *  Dependencies between libraries are expressed by referencing in the name
 *  of the "library group" declared in the object returned by another module's
 *  getLibs() function.  Because this name is likely to be referenced by
 *  separate products, it's important that, once a name is defined and
 *  avaliable in a shipping product, it should never be changed; otherwise,
 *  it will break the any product that references the old name.  However, the
 *  number and names of the libraries specified in libs (and their
 *  dependencies) can freely change from release to release.
 *
 *  Example
 *  -------
 *  The Network Developer's Kit (NDK) product provides libraries that
 *  reference functions defined in the libraries provided by TI-DRIVERS.
 *  To contribute these libraries to the generated linker command file, the
 *  NDK product must
 *      1. provide one or more modules that "represent" the libraries
 *         provided by the NDK product.  In this example, we assume a
 *         module, say NDK.syscfg.js, that's added to all configurations
 *         that use the NDK.
 *      2. within this hypothetical NDK.syscfg.js module,
 *             o define a function, say getLibs(mod), that returns the list of
 *               libraries dictated by the current configuration and their
 *               dependencies, and
 *             o add a line to the exports.templates object, that
 *               passes an argument specifying both the getLibs() function and
 *               the NDK's module name to the GenLibs.cmd.xdt template
 *
 *  In pseudo-code, NDK.syscfg.js would include something like:
 *
 *      // getLibs returns the list of libraries to link and their dependencies
 *      function getLibs(mod) {
 *          let result = {
 *              name: "/ti/ndk", // products that use NDK reference this
 *                               // "group" name in their deps
 *              libs: [],        // libraries that are needed by the
 *                               // current configuration
 *              deps: []         // our libs require TI-DRIVERS libraries
 *          }
 *
 *          // populate libs as a function of the NDK module's configuration
 *          result.libs.push("a/relative/path/to.lib");
 *              :
 *
 *          // populate deps as a function of the libs added above
 *          result.dep.push("/ti/drivers");
 *              :
 *          return (result);
 *      }
 *
 *      // pass our getLibs() function to the GenLibs template
 *      exports = {
 *          :
 *          templates: {
 *              "/ti/utils/build/GenLibs.cmd.xdt":
 *                  {"modName: "/ti/ndk/NDK", getLibs: getLibs}
 *              :
 *          }
 *      }
 */

/*
 *  ======== config ========
 *  Config options to "tune" the content of GenLibs.cmd.xdt template
 */
let config = [
    {
        name: "enableLibs",
        displayName: "Generate Linker File with Libraries",
        hidden: true,
        default: false
    },
    {
        name: "asserts",
        displayName: "Asserts",
        hidden: true,
        default: ""
    },
    {
        name: "toolchain",
        displayName: "Toolchain",
        hidden: true,
        default: "",
        options: [
            {
                name: "TI",
                displayName: "TI"
            },
            {
                name: "GCC",
                displayName: "GCC"
            },
            {
                name: "IAR",
                displayName: "IAR"
            },
            {
                name: "",
                displayName: "Use command line settings"
            }
        ]
    }
];

/*
 *  ======== tsortLib ========
 *  Library of topological sort algorithms
 */
let tsortLib = system.getScript("tsort.syscfg.js");

/*
 *  ======== genList ========
 *  Generate sorted list of libraries based on array of "library groups"
 *
 *  genList sorts the specified array of library groups such that
 *  for every pair of groups, say A and B, if A depends on B, then A appears
 *  before B.  This is always possible _provided_ that the dependency graph
 *  determined by each group's deps[] array has no cycles.
 *
 *  In the case where there is a cycle, it is not possible to create a linear
 *  link order for the libraries that works for all applications.  In this
 *  case, the configuration parameters above should be used to "break cycles"
 *  and force a specific order of library groups.
 *
 *  Each library group is specified by an object of the form:
 *     { name: "/ti/display",
 *       libs: ["/...", ...],
 *       deps: ["/ti/drivers:1.0.1.0", ...]
 *     }
 *
 *  @param args - array of library groups and any user supplied "assertions"
 *  @returns    - new array of all library groups sorted in the order that
 *                their libraries should be specified to the linker
 *
 */
function genList(args)
{
    var groups = []; /* array of all getLibs() return values */
    var edges  = []; /* user/module added dependencies */
    var cuts   = {}; /* user/module specified edge cuts */

    /* parse input arguments for dependency assertions and getLibs() */
    var i, j;
    for (i = 0; i < args.length; i++) {
        var obj = args[i];

        if ("getLibs" in obj) {
            var mod = system.modules[obj.modName];
            //console.log("    adding group: from ", obj.modName + "(" + mod + ")");
            if (mod != null) {
                groups.push(obj.getLibs(mod));
            }
        }

        if ("asserts" in obj) {
            var list = obj.asserts.split(/;+\s*/);
            for (j = 0; j < list.length; j++) {
                var expr = list[j].trim();
                var tokens = expr.match(
                    /^([a-zA-Z0-9/]+)\s*([<>#])\s*([a-zA-Z0-9/]+)$/);
                if (tokens) {
                    switch (tokens[2]) {
                    case '>':
                        edges.push({start: tokens[1], end: tokens[3]});
                        break;
                    case '<':
                        edges.push({start: tokens[3], end: tokens[1]});
                        break;
                    case '#':
                        if (cuts[tokens[1]] == null) {
                            cuts[tokens[1]] = {};
                        }
                        cuts[tokens[1]][tokens[3]] = true;
                        break;
                    }
                }
            }
        }
    }

    /* sort groups to ensure deterministic output */
    groups.sort(
        function (a, b) {
            return ((a.name == b.name) ? 0 : ((a.name < b.name) ? 1 : -1));
        }
    );

    /* map of all library contributions, keyed by name in getLibs return val */
    var groupMap = {};

    /* create DAG of components pointing to direct dependent components */
    function myWarning(node, prefix, cycle) {
        console.log("warning: " + prefix + "\n    " + cycle);
    }
    var graph = new tsortLib.Graph("GenLibs Dependency Graph", myWarning);

    /* add asserted edges - cuts to the DAG */
    for (i = 0; i < edges.length; i++) {
        var edge = edges[i];
        if ((edge.start in cuts) && (edge.end in cuts[edge.start])) {
            console.log("warning: the added dependency '"
                        + edge.start + "' -> '" + edge.end + "' has been cut");
        }
        else {
            graph.mkNode(edge.start).pointsTo(graph.mkNode(edge.end));
        }
    }

    /* add library group dependencies - cuts to the DAG */
    for (i = 0; i < groups.length; i++) {
        var group = groups[i];

        /* add group to a groupMap (used for error checking) */
        if (groupMap[group.name] != null) {
            console.log("warning: '" + group.name
                + "' specified more than once; overriding previous value");
        }
        groupMap[group.name] = group;

        /* add group's dependency edges - cuts */
        var start = group.name;
        var startAdded = false;
        for (j = 0; j < group.deps.length; j++) {
            var end = group.deps[j].split(':')[0];
            if ((start in cuts) && (end in cuts[start])) {
                console.log("    cutting '" + start + "' -> '" + end + "'");
            }
            else {
                console.log("    '" + start + "' -> '" + end + "'");
                graph.mkNode(start).pointsTo(graph.mkNode(end));
            }
        }
        if (startAdded == false) {
            graph.mkNode(start);  /* ensure start's added to graph */
        }
    }

    /* sort the graph's nodes */
    var nodes = graph.tsort();

    /* use nodes to create a dependency sorted array of library groups */
    var groupList = [];
    for (i = 0; i < nodes.length; i++) {
        var name = nodes[i].name;
        group = groupMap[name];
        if (group != null) {
            groupList.unshift(group); /* add to start of groupList array */
        }
        else {
            console.log("warning: required dependency '" + name
                + "' was not provided, no libraries corresponding to this"
                + " dependency can be added to the linker command file.");
        }
    }

    return (groupList);
}

/*
 *  ======== getToolchainDir ========
 */
function getToolchainDir()
{
    let GenLibs = system.modules["/ti/utils/build/GenLibs"];

    let tcDir = ("compiler" in system) ? system.compiler : "ccs";
    if (GenLibs && GenLibs.$static.toolchain != "") {
        let tc2c = {TI: "ccs", GCC: "gcc", IAR: "iar"};
        tcDir = tc2c[GenLibs.$static.toolchain];
    }
    return (tcDir);
}

/*
 * Given a namespace (e.g. "ti/drivers") and library name (e.g. "drivers.a"),
 * returns the SimpleLink-convention-compliant path in which a Component's
 * libraries reside (e.g. "ti/drivers/lib/ccs/m4f/drivers.a").
 */
function libPath(namespace, library)
{
    let devId = system.deviceData.deviceId;
    let isa = "";

    switch (devId) {
        case "MSP432E":
        case "MSP432P401R":
        case "MSP432P4111":
            isa = "m4f";
            break;

        case "CC32XX":
            isa = "m4";
            break;

        /* TODO: add more devices */

        /* is there a good default? For now, just use devId so users
         * see it's broken and report what devId isn't handled here
         */
        default:
            isa = devId;
            break;
    }

    return(namespace + "/lib/" + getToolchainDir() + "/" + isa + "/" +
            library);
}

/*
 *  ======== longDescription ========
 *  Intro splash on GUI
 */
let longDescription =
    "GenLibs enables users to generate a linker command file, specific " +
    "to your toolchain, with the list of libraries required to support " +
    "the current configuration.";

/*
 *  ======== exports ========
 */
exports = {
//    staticOnly: true,
    displayName: "GenLibs",
    description: "Generate list of libraries needed to link",
    longDescription: longDescription,
    moduleStatic: {
        config: config
    },

    genList: genList,

    getToolchainDir: getToolchainDir,
    libPath: libPath,

    templates: {
        "/local/support/GenLibs.cmd.xdt": {}
    }
};
