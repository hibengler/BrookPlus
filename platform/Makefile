#
# Copyright (c) 2003, Stanford University
# All rights reserved.
#
# Copyright (c) 2007, Advanced Micro Devices, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of Stanford University nor the names of any contributors 
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

ROOTDIR := ..
SUBDIRS := brcc runtime
SDKDIR := $(ROOTDIR)/sdk

# We don't use the default "all" rule here because we want to build the Brook
# platform then copy it over to the sdk

.PHONY: updatesdk

sdk: all updatesdk

EXTRA_SDK_CLEAN := $(ROOTDIR)/sdk

include $(ROOTDIR)/platform/utils/build/main.mk

# These are the files the SDK should contain

# BRCC := $(SDKDIR)/bin/brcc$(BINSUFFIX) $(SDKDIR)/bin/brcc$(BINSUFFIX)

# $(BRCC):
# 	$(MKDIR) $(SDKDIR)/bin
# 	$(CP) $(ROOTDIR)/platform/brcc/bin/$(BINTYPENAME)/brcc$(DBGSUFFIX)$(BINSUFFIX) $(SDKDIR)/bin/

# BROOKLIB := $(SDKDIR)/lib/brook$(DBGSUFFIX)$(STATIC_LIBSUFFIX)

# $(BROOKLIB):
# 	$(MKDIR) $(SDKDIR)/lib
# 	$(CP) $(ROOTDIR)/platform/runtime/lib/$(BINTYPENAME)/brook$(DBGSUFFIX)$(STATIC_LIBSUFFIX) $(SDKDIR)/lib/

# INCLUDE_SRC := $(wildcard $(ROOTDIR)/platform/include/brook/*.hpp)
# INCLUDE_DST := $(notdir $(INCLUDE_SRC))
# INCLUDE_DST := $(foreach f, $(INCLUDE_DST), $(SDKDIR)/include/brook/$(f))

# $(INCLUDE_DST): # This is a little overkill, in that in some circumstances it may result in redundant copy operations. At least it's safe.
# 	$(MKDIR) $(SDKDIR)/include/brook
# 	$(CP) $(INCLUDE_SRC) $(SDKDIR)/include/brook


# SDKFILES := $(BRCC) $(BROOKLIB) $(INCLUDE_DST)

updatesdk: 
	$(MKDIR) $(SDKDIR)/bin
	$(MKDIR) $(SDKDIR)/lib
	$(MKDIR) $(SDKDIR)/include/brook
	-$(CP) $(ROOTDIR)/platform/brcc/bin/$(BINTYPENAME)/brcc$(DBGSUFFIX)$(BINSUFFIX) $(SDKDIR)/bin
	-$(CP) $(ROOTDIR)/platform/runtime/lib/$(BINTYPENAME)/$(LIBPREFIX)brook$(DBGSUFFIX)$(SHARED_LIBSUFFIX) $(SDKDIR)/lib
	-$(CP) $(ROOTDIR)/platform/runtime/lib/$(BINTYPENAME)/$(LIBPREFIX)brook_cal$(DBGSUFFIX)$(SHARED_LIBSUFFIX) $(SDKDIR)/lib
ifneq ($(OS), lnx)
	-$(CP) $(ROOTDIR)/platform/runtime/lib/$(BINTYPENAME)/$(LIBPREFIX)brook$(DBGSUFFIX)$(LD_LIBLINK_SUFFIX) $(SDKDIR)/lib
endif
	-$(CP) -av $(ROOTDIR)/platform/include/brook/* $(SDKDIR)/include/brook

