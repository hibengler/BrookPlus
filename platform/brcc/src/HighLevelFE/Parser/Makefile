ROOTDIR := ../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := pplexer lexer gram 
         

ADDITIONAL_DEPENDENCIES := gram.h ps2arb_gram.h

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/Parser/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/HighLevelFE/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Deprecated/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Deprecated/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Utils/HeaderFiles/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/Codegen/Common/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/BrookHighLevelView/HeaderFiles/"

# We only want to clean the flex/bison generated stuff if we can rebuild it

include $(ROOTDIR)/platform/utils/build/config.mk
ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY := bHighLevelFEP
endif


ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := pplexer.cpp lexer.cpp gram.cpp gram.output gram.h 
endif
endif

include $(ROOTDIR)/platform/utils/build/main.mk

# Rules for generating grammar and lexer files


pplexer.cpp: pplexer.l
ifdef FLEX
	$(FLEX) -d -L $<
	rm -f pplexer.cpp
	sed -f massage-flex.sed < lex.ppyy.c > pplexer.cpp
	rm -f lex.ppyy.c
else
	echo "flex not found, not rebuilding pplexer.cpp"
endif


lexer.cpp: lexer.l gram.h
ifdef FLEX
	$(FLEX) -d -L $<
	rm -f lexer.cpp
	sed -f massage-flex.sed < lex.yy.c > lexer.cpp
	rm -f lex.yy.c
else
	echo "flex not found, not rebuilding lexer.cpp"
endif

gram.cpp gram.h:  gram.y
ifdef BISON
	bison -d $<
	-mv -f gram.tab.c gram.cpp
	-mv -f gram.tab.h gram.h
else
	echo "bison not found, not rebuilding gram.cpp"
endif
