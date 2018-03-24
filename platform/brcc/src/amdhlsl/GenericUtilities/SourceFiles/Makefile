ROOTDIR := ../../../../../..

# What we're building and where to put it
OUTPUTBASE := /platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := assertlogging conststring parameter positionaware smartpointer timer 

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles"


# We only want to clean the flex/bison generated stuff if we can rebuild it

include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY := GenericUtilities
endif


include $(ROOTDIR)/platform/utils/build/main.mk
