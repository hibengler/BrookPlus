ROOTDIR := ../../../..
SUBDIRS := SourceFiles



ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h


include $(ROOTDIR)/platform/utils/build/config.mk

# We only want to clean the flex/bison generated stuff if we can rebuild it
ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := lexer.cpp gram.cpp gram.output gram.h \
                   ps2arb_lexer.cpp ps2arb_gram.cpp ps2arb_gram.h
endif
endif


include $(ROOTDIR)/platform/utils/build/main.mk
