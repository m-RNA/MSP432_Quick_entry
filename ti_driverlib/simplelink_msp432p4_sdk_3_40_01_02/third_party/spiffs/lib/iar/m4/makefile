SIMPLELINK_SDK_INSTALL_DIR ?= $(abspath ../../../../../../)
include $(SIMPLELINK_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
AR = "$(IAR_ARMCOMPILER)/bin/iarchive"

OBJS = spiffs_cache.obj \
	spiffs_check.obj \
	spiffs_gc.obj \
	spiffs_hydrogen.obj \
	spiffs_nucleus.obj \
	SPIFFSNVS.obj

CFLAGS = -I../../../ -I$(SIMPLELINK_SDK_INSTALL_DIR)/source \
	"-I$(IAR_ARMCOMPILER)/inc" --cpu=Cortex-M4 --debug --silent -e --aeabi \
	--thumb --diag_suppress=Pa050,Go005 --endian=little -Ohs

vpath %.c ../../../

%.obj: %.c
	@ $(CC) $(CFLAGS) $< -o $@

all: spiffs.a

clean:
	@ $(RM) $(OBJS) spiffs.a > $(DEVNULL) 2>&1

spiffs.a: $(OBJS)
	@ echo "# Archiving $@"
	@ $(AR) $@ $^
