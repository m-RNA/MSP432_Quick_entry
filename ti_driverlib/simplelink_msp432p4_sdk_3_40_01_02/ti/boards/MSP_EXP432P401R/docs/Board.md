# SimpleLink&trade; MSP-EXP432P401R LaunchPad&trade; Settings & Resources

The [__SimpleLink MSP-EXP432P401R LaunchPad__][launchpad] contains a
[__MSP432P401R__][device] device.


## Jumper Settings

* Close the __`RXD<<`__ and __`TXD>>`__ to enable UART via the XDS110 on-board
USB debugger for UART0 (default).


## SysConfig Board File

The [MSP_EXP432P401R.syscfg.json](../.meta/MSP_EXP432P401R.syscfg.json)
is a handcrafted file used by SysConfig. It describes the physical pin-out
and components on the LaunchPad.


## Driver Examples Resources

Examples utilize SysConfig to generate software configurations into
the __ti_drivers_config.c__ and __ti_drivers_config.h__ files. The SysConfig
user interface can be utilized to determine pins and resources used.
Information on pins and resources used is also present in both generated files.


## TI BoosterPacks&trade;

The following BoosterPack(s) are used with some driver examples.

#### [__BOOSTXL-SHARP128 LCD & SD Card BoosterPack__][boostxl-sharp128]
  * No modifications are needed.

#### [__BOOSTXL-BASSENSORS BoosterPack__][boostxl-bassensors]
  * No modifications are needed.



[device]: http://www.ti.com/product/MSP432P401R
[launchpad]: http://www.ti.com/tool/MSP-EXP432P401R
[boostxl-sharp128]: http://www.ti.com/tool/boostxl-sharp128
[boostxl-bassensors]: http://www.ti.com/tool/BOOSTXL-BASSENSORS