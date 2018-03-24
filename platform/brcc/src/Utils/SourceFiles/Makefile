ROOTDIR := ../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := logger 

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

# We only want to clean the flex/bison generated stuff if we can rebuild it

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Utils/HeaderFiles/"
include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY :=brcc
#Combine the Static Libraries Into One Static Lib
ifdef RELEASE
LIBRARIES:= aHighLevelFES aHighLevelFEP ILCodeGen BlockAllocator GenericUtilities InterfaceUtils \
            Codegen_Common Codegen_IL CPUBackendUtils  bHighLevelFES bHighLevelFEP BrookHighLevel
else
LIBRARIES:= aHighLevelFES_d aHighLevelFEP_d ILCodeGen_d BlockAllocator_d GenericUtilities_d InterfaceUtils_d \
            Codegen_Common_d Codegen_IL_d CPUBackendUtils_d  bHighLevelFES_d bHighLevelFEP_d BrookHighLevel_d
endif
endif


ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := lexer.cpp gram.cpp gram.output gram.h \
                   ps2arb_lexer.cpp ps2arb_gram.cpp ps2arb_gram.h
endif
endif

#CALLIBS := calutAMDhlslCompiler

include $(ROOTDIR)/platform/utils/build/main.mk

# Rules for generating grammar and lexer files
