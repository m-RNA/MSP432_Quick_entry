SIMPLELINK_SDK_INSTALL_DIR ?= $(abspath ../../../../../../)
include $(SIMPLELINK_SDK_INSTALL_DIR)/imports.mak

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
AR = "$(CCS_ARMCOMPILER)/bin/armar"

OBJS = spiffs_cache.obj \
	spiffs_check.obj \
	spiffs_gc.obj \
	spiffs_hydrogen.obj \
	spiffs_nucleus.obj \
	SPIFFSNVS.obj

CFLAGS = -I../../../ -I$(SIMPLELINK_SDK_INSTALL_DIR)/source \
	"-I$(CCS_ARMCOMPILER)/include" --silicon_version=7M4 --code_state=16 \
	--little_endian --display_error_number --diag_warning=255 --diag_wrap=off \
	--gen_func_subsections=on --float_support=vfplib -O2 --symdebug:dwarf

ARFLAGS = -r

vpath %.c ../../../

%.obj: %.c
	@ $(CC) $(CFLAGS) $< --output_file=$@

all: spiffs.a

clean:
	@ $(RM) $(OBJS) spiffs.a > $(DEVNULL) 2>&1

spiffs.a: $(OBJS)
	@ echo "# Archiving $@"
	@ $(AR) $(ARFLAGS) $@ $^
