ROOTDIR := ../../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := calutAMDhlslCompiler amuABIMultiBinary

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/include"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/InterfaceUtils/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/ILCodeGen/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles"
CFLAGS += $(CALINCLUDE)



include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY := InterfaceUtils
endif

include $(ROOTDIR)/platform/utils/build/main.mk
