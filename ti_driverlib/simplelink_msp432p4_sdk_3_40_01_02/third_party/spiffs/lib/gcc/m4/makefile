SIMPLELINK_SDK_INSTALL_DIR ?= $(abspath ../../../../../../)
include $(SIMPLELINK_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
AR = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc-ar"

OBJS = spiffs_cache.obj \
	spiffs_check.obj \
	spiffs_gc.obj \
	spiffs_hydrogen.obj \
	spiffs_nucleus.obj \
	SPIFFSNVS.obj

CFLAGS= -I../../../ -I$(SIMPLELINK_SDK_INSTALL_DIR)/source -mcpu=cortex-m4 \
	-march=armv7e-m -mthumb -std=c99 -mfloat-abi=soft -ffunction-sections \
	-fdata-sections -g -O2 -gstrict-dwarf -Wall --specs=nano.specs

ARFLAGS = -r

vpath %.c ../../../

%.obj: %.c
	@ $(CC) $(CFLAGS) $< -c -o $@

all: spiffs.a

clean:
	@ $(RM) $(OBJS) spiffs.a > $(DEVNULL) 2>&1

spiffs.a: $(OBJS)
	@ echo "# Archiving $@"
	@ $(AR) $(ARFLAGS) $@ $^
