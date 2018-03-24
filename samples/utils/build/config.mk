#####################################################################
# config.mk - contains all definitions that customise the build,    #
# including both user-modifiable items and platform-dependent items #
#####################################################################

############################
# User-modifiable stuff here

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

ifeq ($(OS), xp) # Windows settings
  CC                := cl

ifeq ($(BITS), 64)
  CFLAGS            += /nologo /W3 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /WX /DBITS_64
else
  CFLAGS            += /nologo /W3 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /WX /DBITS_32
endif
  C_CPP_FLAG	    := /nologo /EP
  C_INCLUDE_FLAG    := /I
  C_DEBUG_FLAG      := /Zi /RTC1 /EHsc /Fd$(OBJDIR)\\
  ifeq ($(BITS), 64)
	C_RELEASE_FLAG    := /O2 /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  else
	C_RELEASE_FLAG    := /O2 /arch:SSE2 /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  endif
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
  MANIFEST_TOOL     := mt /nologo    # Only needed for Windows
else
ifeq ($(OS), lh) # Windows Vista settings
  CC                := cl
ifeq ($(BITS), 64)
  CFLAGS            += /nologo /W3 /Wp64 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /WX /FD # FIXME - No OpenMP for Vista64 at present
else
  CFLAGS            += /nologo /W3 /DWIN32 /DWINDOWS /EHsc /Zm500 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FD /WX /openmp
endif
  C_CPP_FLAG	    := /nologo /EP
  C_INCLUDE_FLAG    := /I
  C_DEBUG_FLAG      := /Zi /RTC1 /EHsc /Fd$(OBJDIR)\\
  ifeq ($(BITS), 64)
	C_RELEASE_FLAG    := /O2 /Ob2 /Oi /Ot /Oy /GL /GS- /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  else
	C_RELEASE_FLAG    := /O2 /Ob2 /Oi /Ot /Oy /GL /GS- /arch:SSE2 /EHsc /DNDEBUG /Fd$(OBJDIR)\\
  endif 	
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
  CALLIBPATH        := $(CALROOT)/lib/$(OS)$(BITS)
else 
ifeq ($(OS), lnx) # Linux settings 
  ifeq ($(BITS), 64)
  else
    CFLAGS += -m32
    LDFLAGS += -m32
  endif
  CC                := g++ 
  CFLAGS            += -Wfloat-equal -Wpointer-arith $(GCC_APP_FLAG)
  C_CPP_FLAG	    := -E
  C_INCLUDE_FLAG    := -I  
  C_DEBUG_FLAG      := -g3 -ffor-scope 
  C_RELEASE_FLAG    := -O3 -funroll-all-loops  -fexpensive-optimizations -ffast-math -finline-functions -frerun-loop-opt 
  C_STATIC_FLAG     := 
  C_OUTPUT_FLAG     := -o 
  C_COMPILE_FLAG    := -c 
  C_DEFINE_FLAG	    := -D
  LD                := g++
  LDFLAGS           += -lpthread -ldl -L/usr/X11R6/lib # FIXME libspecs shouldn't be here
  LD_LIBDIR_FLAG    := -L
  LD_SHARED_FLAG    := -shared   
  LD_OUTPUT_FLAG    := -o 
  LD_LIBLINK_PREFIX := -l
  LD_LIBLINK_SUFFIX := 
  LD_DEBUG_FLAG     := 
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
endif
endif
endif

# Common

PERL    := perl
MKDIR   := mkdir -p
RM	:= rm
MAKE	:= make
CP	:= cp -f
ECHO    := @echo

# Only define flex and bison if they are installed 

BISON_PRESENT := $(shell which bison$(BINSUFFIX))
ifneq ("$(BISON_PRESENT)", "")
  BISON := bison$(BINSUFFIX) 
endif

FLEX_PRESENT := $(shell which flex$(BINSUFFIX))
ifneq ("$(FLEX_PRESENT)", "")
  FLEX := flex$(BINSUFFIX) 
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
  LD    := @$(LD)
  MAKE  := @$(MAKE)
  MKDIR := @$(MKDIR)
  MANIFEST_TOOL := @$(MANIFEST_TOOL)
  PERL  := @$(PERL)
  RM    := @$(RM)
endif

ifeq ($(VERBOSE), 0)
  MAKEFLAGS += --no-print-directory
else
  MAKEFLAGS += --print-directory
endif
