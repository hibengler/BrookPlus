ROOTDIR := ../../../../../../..

# What we're building and where to put it
OUTPUTBASE := platform/brcc/bin

COMMONPATH := $(ROOTDIR)/platform/brcc/src

# What to build it from

FILES := parser.tab preprocessor.yy parser.yy 

CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/Parser"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/GenericUtilities/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/ILCodeGen/HeaderFiles"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/include"
CFLAGS += $(C_INCLUDE_FLAG)"$(COMMONPATH)/amdhlsl/AMDhlsl/HighLevelFE/Parser/GeneratedFiles"


include $(ROOTDIR)/platform/utils/build/config.mk


ifeq ($(OS), lnx)
GENERATE_STATIC_LIBRARY :=brcc
else
GENERATE_STATIC_LIBRARY:= aHighLevelFEP
endif

ifdef BISON
ifdef FLEX
REMOVE_ON_CLEAN := parser.tab.cpp parser.tab.hpp preprocessor.yy.cpp parser.yy.cpp 
endif
endif

include $(ROOTDIR)/platform/utils/build/main.mk


# Rules for generating grammar and lexer files

parser.tab.cpp parser.tab.hpp: parser.y parser.hpp
ifdef BISON
	bison -o parser.tab.cpp -p zz --locations -d parser.y
else
	echo "bison not found, not rebuilding parser.tab.cpp"
endif

preprocessor.yy.cpp parser.yy.cpp: parser.l  preprocessor.l parser.hpp
ifdef FLEX
	$(FLEX) -Cfe -s -t parser.l > parser.yy.c
	rm -f parser.yy.cpp
	sed -f massage-flex.sed < parser.yy.c > parser.yy.cpp
	rm -f parser.yy.c
	$(FLEX) -Cfe -s -t preprocessor.l > preprocessor.yy.c
	rm -f preprocessor.yy.cpp
	sed -f massage-flex.sed < preprocessor.yy.c > preprocessor.yy.cpp
	rm -f preprocessor.yy.c
else
	echo "flex not found, not rebuilding ps2arb_lexer.cpp"
endif



