#####################################################################
# main.mk - all the build mechanics, uses the configuration set by  #
# "config.mk", nothing here should need editing                     #
#####################################################################

# Some basic sanity checks

ifndef ROOTDIR
$(error ROOTDIR not defined)
endif

ifdef GENERATE_EXECUTABLE
ifndef OUTPUTBASE
$(error OUTPUTBASE must be defined if we are generating an executable)
endif
endif

ifdef GENERATE_STATIC_LIBRARY
ifndef OUTPUTBASE
$(error OUTPUTBASE must be defined if we are generating a static library)
endif
endif

ifdef GENERATE_SHARED_LIBRARY
ifndef OUTPUTBASE
$(error OUTPUTBASE must be defined if we are generating a shared library)
endif
endif

ifndef GENERATE_EXECUTABLE
ifndef GENERATE_STATIC_LIBRARY
ifndef GENERATE_SHARED_LIBRARY
ifndef SUBDIRS
$(error Nothing to build at this node and no child nodes to build)
endif
endif
endif
endif

# Setup for debug/release builds

ifndef RELEASE
DEBUG := 1
endif

# Need this defined before pulling in config

ifdef DEBUG
DBGSUFFIX := _d
else
DBGSUFFIX := 
endif

OBJDIR := built$(DBGSUFFIX)

# define this to generate cpp file from .br to specific folder
ifndef GENFILESDIR
GENFILESDIR := $(OBJDIR)
endif

# Pull in configuration

include $(ROOTDIR)/samples/utils/build/config.mk

ifdef DEBUG
CFLAGS += $(C_DEBUG_FLAG)
LDFLAGS += $(LD_DEBUG_FLAG)
endif

ifdef RELEASE
CFLAGS += $(C_RELEASE_FLAG)
endif

ifneq ($(OS), lnx) 
ifdef NOT_CRT_DLL
 C_DEBUG_FLAG      += /MTd 
 C_RELEASE_FLAG    += /MT
 else
 C_DEBUG_FLAG      += /MDd 
 C_RELEASE_FLAG    += /MD
endif
endif

# Work out which platform we're building for and construct path for that

BINTYPENAME := $(OS)_$(CPU)_$(BITS)$(BUILDTYPESUFFIX)

# Construct the rest of the paths

OBJDIR_R   := built
OBJDIR_D   := built_d
DEPDIR     := depends
SDKDIR     := $(ROOTDIR)/sdk
SDKBINDIR  := $(SDKDIR)/bin
SDKLIBDIR  := $(SDKDIR)/lib
FASTDEPS   := $(ROOTDIR)/samples/utils/build/fastdep.pl
INCLUDEDIR := $(ROOTDIR)/sdk/include

# Where's brcc?

BRCC := $(ROOTDIR)/sdk/bin/brcc$(DBGSUFFIX)$(BINSUFFIX)
ifeq ($(VERBOSE), 0)
  BRCC := $(BRCC)
endif

# Work out what type of file this directory is building and configure ourselves appropriately

ifdef GENERATE_STATIC_LIBRARY
OUTPUTNAME := $(GENERATE_STATIC_LIBRARY)$(DBGSUFFIX)
OUTPUT := $(addprefix $(LIBPREFIX), $(OUTPUTNAME))
OUTPUT := $(addsuffix $(STATIC_LIBSUFFIX), $(OUTPUT))
CFLAGS := $(C_STATIC_FLAG) $(CFLAGS)
else
ifdef GENERATE_SHARED_LIBRARY
OUTPUTNAME  := $(GENERATE_SHARED_LIBRARY)$(DBGSUFFIX)
OUTPUT  := $(addprefix $(LIBPREFIX), $(OUTPUTNAME))
OUTPUT  := $(addsuffix $(SHARED_LIBSUFFIX), $(OUTPUT))
CFLAGS  := $(C_SHARED_FLAG) $(CFLAGS)
LDFLAGS := $(LD_SHARED_FLAG) $(LDFLAGS) 
else
ifdef GENERATE_EXECUTABLE
TEMP2   := $(addprefix $(LD_LIBLINK_PREFIX), $(EXE_LIBRARIES))
LDFLAGS += $(addsuffix $(LD_LIBLINK_SUFFIX), $(TEMP2))
OUTPUTNAME  := $(GENERATE_EXECUTABLE)$(DBGSUFFIX)
OUTPUT  := $(addsuffix $(BINSUFFIX), $(OUTPUTNAME))
TEMP3   := $(addprefix $(LD_LIBLINK_PREFIX), $(SYSTEM_LIBS))
LDFLAGS += $(addsuffix $(LD_LIBLINK_SUFFIX), $(TEMP3))
CPPFLAGS += $(CFLAGS) $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR)
endif
endif
endif

# Work out where to put the results

OUTPUTDIR := $(ROOTDIR)/$(OUTPUTBASE)/$(BINTYPENAME)

# Default behaviour is to recursively build all the subdirectories (if any)
# then build whatever targets are defined for this node (if any)

ifdef OUTPUT
all: $(SUBDIRS) depend $(OUTPUTDIR)/$(OUTPUT)
else
all: $(SUBDIRS)
endif

# Build subdirectories

$(SUBDIRS):
	$(MAKE) -C $@

# Cleanup the current directory and any files it has generated

clean:
ifdef SUBDIRS
	for i in $(SUBDIRS); do make -C $$i clean; done
endif
	$(RM) -rf $(OBJDIR_R)
	$(RM) -rf $(OBJDIR_D)
	$(RM) -rf $(GENFILESDIR)
	$(RM) -rf $(DEPDIR)
	$(RM) -f *~
ifdef OUTPUT
	$(RM) -f $(OUTPUTDIR)/$(OUTPUT)
	$(RM) -f $(OUTPUTDIR)/$(OUTPUT).manifest
	$(RM) -f $(OUTPUTDIR)/$(basename $(OUTPUT)).pdb
endif
ifdef REMOVE_ON_CLEAN
	rm -rf $(REMOVE_ON_CLEAN)
endif

# How to make dependencies if required for this target

DEPS      := $(FILES)
DEPS      := $(addprefix $(DEPDIR)/, $(DEPS))
DEPS      := $(addsuffix .depend, $(DEPS))

depend: $(DEPS)

$(DEPDIR)/%.depend: %.c
	$(MKDIR) $(DEPDIR)
ifeq ($(VERBOSE), 1)
	$(ECHO) "Rebuilding dependencies for $<"
endif
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(OBJDIR)/' $< > $@

$(DEPDIR)/%.depend: %.cpp
	$(MKDIR) $(DEPDIR)
ifeq ($(VERBOSE), 1)
	$(ECHO) "Rebuilding dependencies for $<"
endif
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(OBJDIR)/' $< > $@

$(DEPDIR)/%.depend: %.br
	$(MKDIR) $(DEPDIR)
ifeq ($(VERBOSE), 1)
	$(ECHO) "Rebuilding dependencies for $<"
endif
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(GENFILESDIR)/' $< > $@

$(DEPDIR)/%.depend: %.bri
	$(MKDIR) $(DEPDIR)
ifeq ($(VERBOSE), 1)
	$(ECHO) "Rebuilding dependencies for $<"
endif
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(GENFILESDIR)/' $< > $@

ifdef FILES
-include $(DEPS)
endif

# Make list of object files from input filenames

OBJS := $(addprefix $(OBJDIR)/, $(FILES))
OBJS := $(addsuffix $(OBJSUFFIX), $(OBJS))

# Create output directory

output_dir:
	$(MKDIR) $(OUTPUTDIR)

# If the output specifies some libraries from the sdk, set things up appropriately

ifdef SDKLIBS
  ADDSDKLIBS := $(foreach f, $(SDKLIBS), $(LD_LIBLINK_PREFIX) $(f)$(DBGSUFFIX)$(LD_LIBLINK_SUFFIX))
  LDFLAGS := $(LDFLAGS) $(LD_LIBDIR_FLAG)$(SDKLIBDIR) $(ADDSDKLIBS)
  SDKDEPS := $(foreach f, $(SDKLIBS), $(SDKLIBDIR)/$(LIBPREFIX)$(f)$(DBGSUFFIX)$(SHARED_LIBSUFFIX))
endif

# Link object files together to generate a binary output

$(OUTPUTDIR)/$(OUTPUT): output_dir $(OBJS) $(SDKDEPS)
ifdef GENERATE_EXECUTABLE
	$(ECHO) Building $@
	$(LD) $(LD_OUTPUT_FLAG)$@ $(OBJS) $(LDFLAGS) $(ADDLIBS) $(LD_LIBDIR_FLAG)$(OUTPUTDIR)
ifneq ($(OS), lnx)
	$(MANIFEST_TOOL) -manifest $@.manifest "-outputresource:$@;1"
endif
endif
ifdef GENERATE_STATIC_LIBRARY
$(OUTPUTDIR)/$(OUTPUT): output_dir $(OBJS)
	$(ECHO) Building $@
	$(AR) $(ARFLAGS) $(AR_LIBDIR_FLAG)$(OUTPUTDIR) $(AR_OUTPUT_FLAG)$@ $(OBJS) $(ADDLIBS)
endif
ifdef SHIP_WITH_BINARY
	$(CP) $(SHIP_WITH_BINARY) $(OUTPUTDIR)
endif

# .cpp -> object

$(OBJDIR)/%$(OBJSUFFIX): %.cpp $(DEPDIR)/%.depend
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(C_OUTPUT_FLAG)$@ $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR) $< 

# .c -> object

$(OBJDIR)/%$(OBJSUFFIX): %.c $(DEPDIR)/%.depend 
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(C_OUTPUT_FLAG)$@ $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR) $< 

# .br -> .cpp

$(GENFILESDIR)/%.cpp: %.br $(DEPDIR)/%.depend $(BRCC)
	$(MKDIR) $(GENFILESDIR)
	$(BRCC) $(BRCCFLAGS) -o $(GENFILESDIR)/$* $<

.PRECIOUS: $(GENFILESDIR)/%.cpp

# .cpp (generated) -> .o/.obj

$(OBJDIR)/%$(OBJSUFFIX): $(GENFILESDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(C_OUTPUT_FLAG)$@ $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR) $(C_INCLUDE_FLAG). $< 

# Regression testing (both running on this machine and packaging tests for running on other machines)

ifndef TDIRNAME
TDIRNAME:=brook-tests-$(shell date +%s)
endif
export TDIRNAME

ifdef REGRESSIONDIRS

regression:
	@for i in $(REGRESSIONDIRS); do \
		make --no-print-directory -C $$i regression || k=$$k" "$$i; \
	done; test -z "$$k" || echo "FAILED: "$$k; test -z "$$k"

# testpackage:
# 	@for i in $(REGRESSIONDIRS); do \
# 		$(MAKE) -C $$i testpackage; done

else

## We're at a leaf node, so run/package the test script living in the test dir

regression: test/$(GENERATE_EXECUTABLE)-test.sh
	@echo -n "Testing $(OUTPUTNAME) $(BRT_RUNTIME) ... "
	@$(ROOTDIR)/platform/utils/printpassfail.sh $(ROOTDIR) $(GENERATE_EXECUTABLE) $(OUTPUTDIR) test/ $(DBGSUFFIX)

# testpackage: arch $(ROOTDIR)/$(TDIRNAME)/platform/utils test/$(OUTPUTNAME)-test.sh
# 	echo "Packaging tests for $(OUTPUTNAME)"
# 	mkdir -p $(ROOTDIR)/$(TDIRNAME)
# 	echo "export OUTPUTNAME=$(OUTPUTNAME)" >> $(ROOTDIR)/$(TDIRNAME)/testlist.sh
# 	echo 'echo -n $$BRT_RUNTIME / $(OUTPUTNAME) ' >> $(ROOTDIR)/$(TDIRNAME)/testlist.sh
# 	echo 'sh platform/utils/printpassfail.sh . $(OUTPUTNAME) bin/$(BINTYPENAME)/ ./' >> $(ROOTDIR)/$(TDIRNAME)/testlist.sh
# 	cp test/* $(ROOTDIR)/$(TDIRNAME)	

# $(ROOTDIR)/$(TDIRNAME)/platform/utils: 
# 	echo "Copying over support files"
# 	mkdir -p $(ROOTDIR)/$(TDIRNAME)/platform/utils
# 	cp -a $(ROOTDIR)/platform/utils/printpassfail.sh $(ROOTDIR)/$(TDIRNAME)/platform/utils
# 	cp -a $(ROOTDIR)/platform/utils/justrunit.sh $(ROOTDIR)/$(TDIRNAME)/platform/utils
# 	cp -a $(ROOTDIR)/platform/utils/check.sh $(ROOTDIR)/$(TDIRNAME)/platform/utils
# 	cp -a $(ROOTDIR)/platform/utils/checkall.sh $(ROOTDIR)/$(TDIRNAME)/platform/utils
# 	echo "Copying over binaries"
# 	mkdir -p $(ROOTDIR)/$(TDIRNAME)/bin/$(BINTYPENAME)
# 	cp -a $(ROOTDIR)/platform/brcc/bin/$(BINTYPENAME)/brcc* $(ROOTDIR)/$(TDIRNAME)/bin/$(BINTYPENAME)  # and runtime...
# 	cp -a $(ROOTDIR)/samples/bin/$(BINTYPENAME)/* $(ROOTDIR)/$(TDIRNAME)/bin/$(BINTYPENAME)

endif

# Build a distribution tree ready for packaging

DISTDIR := $(ROOTDIR)/dist/$(BINTYPENAME)

dist:
	$(RM) -rf $(DISTDIR)/samples
	$(MKDIR) $(DISTDIR)
	$(CP) -r $(ROOTDIR)/samples $(DISTDIR)
	for i in $(DISTDIR)/samples/bin/$(BINTYPENAME)/*.manifest; do rm -f $$i; done
	for i in $(DISTDIR)/samples/bin/$(BINTYPENAME)/*_d.*; do rm -f $$i; done
	for i in $(DISTDIR)/samples/*/*/test; do rm -rf $$i; done
	for i in $(DISTDIR)/samples/*/*/built; do rm -rf $$i; done
	for i in $(DISTDIR)/samples/*/*/built; do rm -rf $$i; done
	for i in $(DISTDIR)/samples/*/*/depends; do rm -rf $$i; done
	$(RM) -f $(DISTDIR)/samples/*.pl
	$(RM) -f $(DISTDIR)/samples/template.br	
	$(RM) -f $(DISTDIR)/samples/utils/*.pl

# Mark the phony targets as such so that they always get triggered

.PHONY: all clean $(SUBDIRS) depend build_dir output_dir dist
