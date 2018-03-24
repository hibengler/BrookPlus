ROOTDIR := ../../../../../../..

# What we're building and where to put it



OUTPUTBASE := /platform/brcc/bin
COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := astbuiltinfunctions astcontrol astdatatype \
        astdeclaration astdelayedlookup astelement astmodifier  \
	astoperators astroot astsymbol astuserdatatype \
	hlfrontendapi parservariant preprocessorstate

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/ILCodeGen/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/include"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/Parser"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/Parser/GeneratedFiles"


include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY:= aHighLevelFES
endif

include $(ROOTDIR)/platform/utils/build/main.mk
