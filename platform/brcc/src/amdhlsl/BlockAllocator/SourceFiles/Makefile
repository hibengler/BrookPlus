ROOTDIR := ../../../../../..

# What we're building and where to put it
OUTPUTBASE :=platform/brcc/bin


COMMONPATH := $(ROOTDIR)/platform/brcc/src


# What to build it from

FILES :=blockallocator #operatornew

#ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/BlockAllocator/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/include"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/Utils/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/HighLevelFE/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/HLFrontend/CodeGen/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles"
CFLAGS += $(CALINCLUDE)


include $(ROOTDIR)/platform/utils/build/config.mk
ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY:= BlockAllocator
endif


#CALLIBS := calutAMDhlslCompiler

include $(ROOTDIR)/platform/utils/build/main.mk
