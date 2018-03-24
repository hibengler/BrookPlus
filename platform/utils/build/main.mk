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

# Pull in configuration

include $(ROOTDIR)/platform/utils/build/config.mk

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

# Additional CAL include paths:

ifdef CALINCLUDE
CFLAGS += $(CALINCLUDE)
endif

# Construct paths

DEPDIR     := depends
FASTDEPS   := $(ROOTDIR)/platform/utils/build/fastdep.pl
INCLUDEDIR := $(ROOTDIR)/platform/include

# Work out which platform we're building for and construct path for that

BINTYPENAME := $(OS)_$(CPU)_$(BITS)$(BUILDTYPESUFFIX)

# Work out what type of file this directory is building and configure ourselves appropriately

ifdef GENERATE_STATIC_LIBRARY
OUTPUT := $(GENERATE_STATIC_LIBRARY)$(DBGSUFFIX)
OUTPUT := $(addprefix $(LIBPREFIX), $(OUTPUT))
OUTPUT := $(addsuffix $(STATIC_LIBSUFFIX), $(OUTPUT))
CFLAGS := $(C_STATIC_FLAG) $(CFLAGS)
else
ifdef GENERATE_SHARED_LIBRARY
OUTPUT  := $(GENERATE_SHARED_LIBRARY)$(DBGSUFFIX)
OUTPUT  := $(addprefix $(LIBPREFIX), $(OUTPUT))
OUTPUT  := $(addsuffix $(SHARED_LIBSUFFIX), $(OUTPUT))
CFLAGS  := $(C_SHARED_FLAG) $(CFLAGS)
LDFLAGS := $(LD_SHARED_FLAG) $(LDFLAGS) 
else
ifdef GENERATE_EXECUTABLE
TEMP2   := $(addprefix $(LD_LIBLINK_PREFIX), $(EXE_LIBRARIES))
LDFLAGS += $(addsuffix $(LD_LIBLINK_SUFFIX), $(TEMP2))
OUTPUT  := $(GENERATE_EXECUTABLE)$(DBGSUFFIX)
OUTPUT  := $(addsuffix $(BINSUFFIX), $(OUTPUT))
TEMP3   := $(addprefix $(LD_LIBLINK_PREFIX), $(SYSTEM_LIBS))
LDFLAGS += $(addsuffix $(LD_LIBLINK_SUFFIX), $(TEMP3))
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
ifdef ETAGS
	$(ETAGS) *.c *.h *.cpp *.hpp
endif
	$(MAKE) -C $@

# Cleanup the current directory and any files it has generated

clean:
ifdef SUBDIRS
	for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
endif
	$(RM) -rf $(OBJDIR)
	$(RM) -rf $(DEPDIR)
	$(RM) -f *~
ifdef OUTPUT
	$(RM) -f $(OUTPUTDIR)/$(OUTPUT)
	$(RM) -f $(OUTPUTDIR)/*.pdb
	$(RM) -f $(OUTPUTDIR)/*.manifest	
endif
ifdef REMOVE_ON_CLEAN
	rm -rf $(REMOVE_ON_CLEAN)
endif
ifdef EXTRA_SDK_CLEAN
	rm -rf $(EXTRA_SDK_CLEAN)
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
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(OBJDIR)/' $< > $@

$(DEPDIR)/%.depend: %.bri
	$(MKDIR) $(DEPDIR)
ifeq ($(VERBOSE), 1)
	$(ECHO) "Rebuilding dependencies for $<"
endif
	$(PERL) $(FASTDEPS) -I. -I$(INCLUDEDIR) --obj-suffix='$(OBJSUFFIX)' --obj-prefix='$(OBJDIR)/' $< > $@

ifdef FILES
-include $(DEPS)
endif


# Make list of object files from input filenames

OBJS := $(addprefix $(OBJDIR)/, $(FILES))
OBJS := $(addsuffix $(OBJSUFFIX), $(OBJS))

# Create output directory

output_dir:
	$(MKDIR) $(OUTPUTDIR)

# Additional libraries that we built (assumed to be sitting in the output dir)

ifdef LIBRARIES
  LD_ADDLIBS := $(foreach f,$(LIBRARIES),$(LD_LIBLINK_PREFIX) $f$(LD_LIBLINK_SUFFIX))
  AR_ADDLIBS := $(foreach f,$(LIBRARIES),$(AR_LIBLINK_PREFIX) $(OUTPUTDIR)/$(LIBPREFIX)$f$(STATIC_LIBSUFFIX))
else
  LD_ADDLIBS :=
  AR_ADDLIBS := 
endif

# Additional libraries for CAL

ifdef CALLIBS
  LD_ADDLIBS += $(foreach f,$(CALLIBS),$(LD_LIBLINK_PREFIX) $f$(LD_LIBLINK_SUFFIX))
  AR_ADDLIBS += $(foreach f,$(CALLIBS),$(AR_LIBLINK_PREFIX) $(CALLIBPATH)/$(LIBPREFIX)$f$(STATIC_LIBSUFFIX))
else
  LD_ADDLIBS +=
  AR_ADDLIBS += 
endif


# Link object files together to generate a binary output

$(OUTPUTDIR)/$(OUTPUT): output_dir $(OBJS)
ifdef GENERATE_EXECUTABLE
	$(ECHO) Building $@
	$(LD) $(LD_OUTPUT_FLAG)$@ $(OBJS) $(LDFLAGS) $(LD_ADDLIBS) $(LD_LIBDIR_FLAG)$(OUTPUTDIR) $(LD_LIBDIR_FLAG)$(CALLIBPATH)

ifneq ($(OS), lnx)
ifdef MANIFEST_TOOL
	$(MANIFEST_TOOL) -manifest $@.manifest "-outputresource:$@;1"
endif
endif
endif
ifdef GENERATE_STATIC_LIBRARY
$(OUTPUTDIR)/$(OUTPUT): output_dir $(OBJS)
	$(ECHO) Building $@
	$(AR) $(ARFLAGS) $(AR_OUTPUT_FLAG)$@ $(OBJS) $(AR_ADDLIBS)
endif
ifdef GENERATE_SHARED_LIBRARY

$(OUTPUTDIR)/$(OUTPUT): output_dir $(OBJS)
	$(ECHO) Building $@
	$(LD) $(LDFLAGS) $(LD_OUTPUT_FLAG)$@ $(OBJS) $(LD_ADDLIBS) $(LD_LIBDIR_FLAG)$(OUTPUTDIR) $(LD_OP_FLAGS) $(LD_LIBDIR_FLAG)$(CALLIBPATH)

ifneq ($(OS), lnx)
ifdef MANIFEST_TOOL
	$(MANIFEST_TOOL) -manifest $@.manifest "-outputresource:$@;2"
endif
endif
endif

ifdef SHIP_WITH_BINARY
	$(CP) $(SHIP_WITH_BINARY) $(OUTPUTDIR)
endif

# Implicit rules
#
# Note that here we generally use $(*F) rather than $< to work round a bug in certain older
# version of GNU make.

# .cpp -> object

$(OBJDIR)/%$(OBJSUFFIX): %.cpp $(DEPDIR)/%.depend 
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(C_OUTPUT_FLAG)$@ $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR) $<

# .c -> object

$(OBJDIR)/%$(OBJSUFFIX): %.c $(DEPDIR)/%.depend
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(C_OUTPUT_FLAG)$@ $(C_COMPILE_FLAG) $(C_INCLUDE_FLAG)$(INCLUDEDIR) $< 


# Build a distribution tree ready for packaging

DISTDIR := $(ROOTDIR)/dist/$(BINTYPENAME)

dist:
	$(RM) -rf $(DISTDIR)/platform
	$(MKDIR) $(DISTDIR)/platform
	sed -e 's/brcc runtime utils/brcc runtime/' < $(ROOTDIR)/platform/Makefile > $(DISTDIR)/platform/Makefile
	$(CP) $(ROOTDIR)/platform/brook.sln $(DISTDIR)/platform
	$(MKDIR) $(DISTDIR)/platform/brcc
	$(CP) $(ROOTDIR)/platform/brcc/LICENSE.cTool $(DISTDIR)/platform/brcc
	$(CP) $(ROOTDIR)/platform/brcc/Makefile $(DISTDIR)/platform/brcc
	$(CP) -rv $(ROOTDIR)/platform/brcc/src $(DISTDIR)/platform/brcc
	$(RM) -rf $(DISTDIR)/platform/brcc/src/built
	$(RM) -rf $(DISTDIR)/platform/brcc/src/built_d
	$(RM) -rf $(DISTDIR)/platform/brcc/src/depends
	$(CP) -rv $(ROOTDIR)/platform/include $(DISTDIR)/platform
	$(MKDIR) $(DISTDIR)/platform/runtime
	$(CP) $(ROOTDIR)/platform/runtime/Makefile $(DISTDIR)/platform/runtime
	$(CP) -rv $(ROOTDIR)/platform/runtime/CAL $(DISTDIR)/platform/runtime
	$(CP) -rv $(ROOTDIR)/platform/runtime/Common $(DISTDIR)/platform/runtime
	$(CP) -rv $(ROOTDIR)/platform/runtime/CPU $(DISTDIR)/platform/runtime
	for i in CAL/Managers CAL/Oracles CAL/Specials CAL/Wrappers \
		Common/Impls Common/Interfaces Common/Managers Common/Oracles \
		Common/Specials Common/Utils Common/Wrappers CPU ; do \
		$(RM) -rf $(DISTDIR)/platform/runtime/$$i/built ;\
		$(RM) -rf $(DISTDIR)/platform/runtime/$$i/built_d ;\
		$(RM) -rf $(DISTDIR)/platform/runtime/$$i/depends ;\
		done
	
	$(MKDIR) $(DISTDIR)/platform/utils
	$(CP) -rv $(ROOTDIR)/platform/utils/build $(DISTDIR)/platform/utils
	$(RM) -rf $(DISTDIR)/sdk
	$(CP) -rv $(ROOTDIR)/sdk $(DISTDIR)
	$(MKDIR) $(DISTDIR)/doc
	$(CP) $(ROOTDIR)/doc/brookspec-v1.pdf $(DISTDIR)/doc/BrookPlusSpec.pdf
	$(CP) $(ROOTDIR)/doc/BrookProgrammingGuide.pdf $(DISTDIR)/doc/BrookPlusProgrammingGuide.pdf
	$(CP) $(ROOTDIR)/doc/releasenotes/release-notes-mar-08.txt $(DISTDIR)/release-notes-mar-08.txt
	$(CP) -rv $(ROOTDIR)/utils $(DISTDIR)

# Check the current subtree for fixmes

fixme:
	@grep -Iirsn fixme *
	@grep -Iirsn 'fix me' *
	@grep -Iirsn fix_me *

# Mark the phony targets as such so that they always get triggered

.PHONY: clean $(SUBDIRS) depend output_dir $(DEPSDONE) fixme

