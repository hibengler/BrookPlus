ROOTDIR := ../../../../../../..

# What we're building and where to put it
COMMONPATH := /usr/local/amdbrook/platform/brcc/src



OUTPUTBASE := platform/brcc/bin
COMMONPATH := $(ROOTDIR)/platform/brcc/src

FILES:= ilregister iltexttranscriber iltokentranscriber \
        iltranscriber iltranslator inputpatcher shaderdescription
#Header Files Inclusion
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/ILCodeGen/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/include"
include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY := ILCodeGen
endif

include $(ROOTDIR)/platform/utils/build/main.mk
