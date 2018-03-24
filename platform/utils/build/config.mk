#####################################################################
# config.mk - contains all definitions that customise the build,    #
# including both user-modifiable items and platform-dependent items #
#####################################################################

############################
# User-modifiable stuff here

# Which runtimes to build (0 = disabled, !0 = enabled)
#
# NB - Right now, CPU always gets built, DX9 and OGL are unavailable
# (use original BrookGPU for DX9 and OGL)

BUILD_CAL := 1
BUILD_CPU := 1
BUILD_DX9 := 0
BUILD_OGL := 0

# How noisy do you like your builds?

VERBOSE := 1

#########################################################################
# Automatic stuff here (you shouldn't need to modify anything below here
# except to add new platform types)

# What operating system?


UNAME := $(shell uname -a)

ifeq ($(findstring CYGWIN_NT, $(UNAME)), CYGWIN_NT)
  ifeq ($(findstring 6.0, $(UNAME)), 6.0)
    OS := lh
  else
    OS := xp
  endif
else 
ifeq ($(findstring Linux, $(UNAME)), Linux)
  OS := lnx
else 
ifeq ($(findstring Darwin, $(UNAME)), Darwin)
  OS := MacOSX
else
  $(error Unknown OS:$(UNAME))
endif
endif
endif

# What CPU and mode?

ifeq ($(findstring WOW64, $(UNAME)), WOW64)
  CPU = x86
  BITS = 64
  ifeq ($(ARCH_TYPE), x86)
    BITS = 32
  endif
else
ifeq ($(findstring i686, $(UNAME)), i686)
  CPU = x86
  BITS = 32
  ifeq ($(ARCH_TYPE), x64)
    BITS = 64
  endif
else
ifeq ($(findstring x86_64, $(UNAME)), x86_64)
  CPU = x86
  BITS = 64
  ifeq ($(ARCH_TYPE), x86)
    BITS = 32
  endif
else
  $(error Unknown CPU)
endif
endif
endif


###################################################
# Set up tool and target definitions based on above

ifeq ($(OS), xp) # Windows XP settings
  CC                := cl
ifeq ($(BITS), 64)
  CFLAGS            += /nologo /W3 /Wp64 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /DBITS_64
else
  CFLAGS            += /nologo /W3 /Wp64 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /DBITS_32
endif
  C_CPP_FLAG	    := /nologo /EP
  C_INCLUDE_FLAG    := /I
  C_DEBUG_FLAG      := /Zi /RTC1 /EHsc /Fd$(OBJDIR)\\ /DBROOK_LOGGER_ENABLED
  C_RELEASE_FLAG    := /O2 /Ob2 /Oi /Ot /Oy /GS- /arch:SSE2 /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  C_STATIC_FLAG     := 
  C_OUTPUT_FLAG     := /Fo
  C_COMPILE_FLAG    := /c
  C_DEFINE_FLAG     := /D
  LD                := link
  LDFLAGS           += /nologo /fixed:no /INCREMENTAL:NO
  LD_LIBDIR_FLAG    := /libpath:
  LD_SHARED_FLAG    := /DLL
  LD_OUTPUT_FLAG    := /out:
  LD_LIBLINK_PREFIX :=  
  LD_LIBLINK_SUFFIX := .lib 
  LD_DEBUG_FLAG     := /debug
  LD_DEF_FLAG       := /def:
  AR                := lib
  ARFLAGS           := /nologo /NODEFAULTLIB:LIBCMT
  AR_OUTPUT_FLAG    := /out:
  AR_LIBDIR_FLAG    := /libpath:
  AR_LIBLINK_PREFIX :=  
  AR_LIBLINK_SUFFIX := .lib 
  SYSTEM_LIBS       := kernel32 gdi32 user32 advapi32 Winmm
  OBJSUFFIX         := .obj
  LIBPREFIX         := 
  SHARED_LIBSUFFIX  := .dll
  STATIC_LIBSUFFIX  := .lib
  BINSUFFIX         := .exe
  MANIFEST_TOOL     := mt     # Only needed for Windows
  CALINCLUDE	    := $(C_INCLUDE_FLAG)"$(CALROOT)\include"
  CALLIBPATH        := "$(CALROOT)\lib\$(OS)$(BITS)"
else
ifeq ($(OS), lh) # Windows Vista settings
  CC	            := cl
ifeq ($(BITS), 64)
  CFLAGS            += /nologo /W3 /Wp64 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /DBITS_64 
else
  CFLAGS            += /nologo /W3 /Wp64 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /DBITS_32
endif
  C_CPP_FLAG	    := /nologo /EP
  C_INCLUDE_FLAG    := /I
  C_DEBUG_FLAG      := /Zi /RTC1 /EHsc /Fd$(OBJDIR)\\ /DBROOK_LOGGER_ENABLED
  C_RELEASE_FLAG    := /O2 /Ob2 /Oi /Ot /Oy /GS- /arch:SSE2 /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  C_STATIC_FLAG     := 
  C_OUTPUT_FLAG     := /Fo
  C_COMPILE_FLAG    := /c
  C_DEFINE_FLAG     := /D
  LD                := link
ifeq ($(BITS), 64)
  LDFLAGS           += /nologo /fixed:no /INCREMENTAL:NO /MACHINE:X64
else
  LDFLAGS           += /nologo /fixed:no /INCREMENTAL:NO
endif
  LD_LIBDIR_FLAG    := /libpath:
  LD_SHARED_FLAG    := /DLL
  LD_OUTPUT_FLAG    := /out:
  LD_LIBLINK_PREFIX :=  
  LD_LIBLINK_SUFFIX := .lib 
  LD_DEBUG_FLAG     := /debug
  LD_DEF_FLAG       := /def:
  AR                := lib
  ARFLAGS           := /nologo /NODEFAULTLIB:LIBCMT
  AR_OUTPUT_FLAG    := /out:
  AR_LIBDIR_FLAG    := /libpath:
  AR_LIBLINK_PREFIX :=  
  AR_LIBLINK_SUFFIX := .lib 
  SYSTEM_LIBS       := kernel32 gdi32 user32 advapi32 Winmm
  OBJSUFFIX         := .obj
  LIBPREFIX         := 
  SHARED_LIBSUFFIX  := .dll
  STATIC_LIBSUFFIX  := .lib
  BINSUFFIX         := .exe
  MANIFEST_TOOL     := mt     # Only needed for Windows
  CALINCLUDE	    := $(C_INCLUDE_FLAG)"$(CALROOT)\include"
  CALLIBPATH        := "$(CALROOT)\lib\$(OS)$(BITS)"
else 
ifeq ($(OS), lnx) # Linux settings 
  ifeq ($(BITS), 64)
  else
    CFLAGS += -m32
    LDFLAGS += -m32
  endif
  CC                := g++ 
ifeq ($(BITS), 64)
  CFLAGS            += -Wfloat-equal -Wpointer-arith -fpic -DBITS_64
  CALLIBPATH        := "$(CALROOT)/lib64"
else
  CFLAGS            += -Wfloat-equal -Wpointer-arith -fpic -DBITS_32
  CALLIBPATH        := "$(CALROOT)/lib"
endif
  C_CPP_FLAG	    := -E
  C_INCLUDE_FLAG    := -I  
  C_DEBUG_FLAG      := -g3 -ffor-scope -DBROOK_LOGGER_ENABLED -D_DEBUG
#  C_RELEASE_FLAG    := -O3 -funroll-all-loops  -fexpensive-optimizations -ffast-math -finline-functions -frerun-loop-opt 
# remove the optimization flags to work around the release brcc issue on linux64
  C_STATIC_FLAG     := 
  C_OUTPUT_FLAG     := -o 
  C_COMPILE_FLAG    := -c 
  C_DEFINE_FLAG	    := -D
  LD                := g++
  LDFLAGS           += -lpthread -L/usr/X11R6/lib64 -L/usr/X11R6/lib # FIXME libspecs shouldn't be here
  LD_LIBDIR_FLAG    := -L
  LD_SHARED_FLAG    := -shared   
  LD_OUTPUT_FLAG    := -o 
  LD_LIBLINK_PREFIX := -l
  LD_LIBLINK_SUFFIX := 
  LD_DEBUG_FLAG     := 
  LD_OP_FLAGS	    := -Wl,--no-whole-archive	
  AR                := ar
  ARFLAGS           := -rsc
  AR_OUTPUT_FLAG    := 
  AR_LIBDIR_FLAG    := 
  AR_LIBLINK_PREFIX :=  
  AR_LIBLINK_SUFFIX := 
  SYSTEM_LIBS      := 
  RUNTIME_LIBS     := brook 
  OBJSUFFIX        := .o
  LIBPREFIX        := lib
  SHARED_LIBSUFFIX := .so
  STATIC_LIBSUFFIX := .a
  BINSUFFIX        := 
  CALINCLUDE	    := $(C_INCLUDE_FLAG)"$(CALROOT)/include"
endif
endif
endif

# Common

PERL    := perl
MKDIR   := -mkdir -p
RM	:= rm
MAKE	:= make
CP	:= cp -f
ECHO    := @echo

# Only define flex, bison and etags if they are installed 
ifdef USE_BISON_FLEX
BISON_PRESENT := $(shell which bison$(BINSUFFIX))
ifneq ("$(BISON_PRESENT)", "")
  BISON := bison$(BINSUFFIX) 
endif

FLEX_PRESENT := $(shell which flex$(BINSUFFIX))
ifneq ("$(FLEX_PRESENT)", "")
  FLEX := flex$(BINSUFFIX) 
endif
endif

ETAGS_PRESENT := $(shell which etags$(BINSUFFIC))
ifneq ("$(ETAGS_PRESENT)", "")
  ETAGS := etags$(BINSUFFIX)
endif


#######################################
# Make tools quieter if requested above

ifeq ($(VERBOSE), 0)
  AR    := @$(AR)
  ifdef BISON
    BISON := @$(BISON)
  endif
  CC    := @$(CC)
  ifdef FLEX
    FLEX  := @$(FLEX)
  endif
  ifdef ETAGS
    ETAGS  := @$(ETAGS)
  endif
  LD    := @$(LD)
  MKDIR := @$(MKDIR)
  PERL  := @$(PERL)
  RM    := @$(RM)
endif

ifeq ($(VERBOSE), 0)
  MAKEFLAGS += --no-print-directory
else
  MAKEFLAGS += --print-directory
endif
