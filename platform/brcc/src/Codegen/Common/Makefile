ROOTDIR := ../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src
# What to build it from

FILES := cgBuiltin cgKernel cgProgram cgResource cgSemantic cgTool cgFunction


ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/Common/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/IL/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/CU/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Utils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Deprecated/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/InterfaceUtils/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/BrookHighLevelView/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/CPUBackendUtils/HeaderFiles/"

include $(ROOTDIR)/platform/utils/build/config.mk

# We only want to clean the flex/bison generated stuff if we can rebuild it
ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := lexer.cpp gram.cpp gram.output gram.h \
                   ps2arb_lexer.cpp ps2arb_gram.cpp ps2arb_gram.h
endif
endif

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY :=Codegen_Common
endif


include $(ROOTDIR)/platform/utils/build/main.mk


