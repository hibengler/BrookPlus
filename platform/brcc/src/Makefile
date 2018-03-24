ROOTDIR := ../../..
SUBDIRS :=  amdhlsl Codegen CPUBackendUtils HighLevelFE BrookHighLevelView Utils 

# What we're building and where to put it

GENERATE_EXECUTABLE := brcc

OUTPUTBASE := platform/brcc/bin
COMMONPATH := $(ROOTDIR)/platform/brcc/src


ifdef RELEASE
	LIBRARIES:=brcc
else
	LIBRARIES:=brcc_d
endif

# What to build it from
FILES := main getopt

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Utils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/BrookHighLevelView/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/Common/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/CPUBackendUtils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/Parser/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/HeaderFiles/"

ifneq ($(OS), lnx)
        MANIFEST_TOOL := 1
endif

include $(ROOTDIR)/platform/utils/build/config.mk

include $(ROOTDIR)/platform/utils/build/main.mk
