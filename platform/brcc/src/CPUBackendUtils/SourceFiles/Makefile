ROOTDIR := ../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := b2cindex b2ctransform

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/CPUBackendUtils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Utils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Deprecated/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/Common/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/BrookHighLevelView/HeaderFiles/"


include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY := CPUBackendUtils
endif


ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := lexer.cpp gram.cpp gram.output gram.h \
                   ps2arb_lexer.cpp ps2arb_gram.cpp ps2arb_gram.h
endif
endif


include $(ROOTDIR)/platform/utils/build/main.mk
