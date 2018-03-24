#
# Copyright (c) 2003, Stanford University
# All rights reserved.
#
# Copyright (c) 2008, Advanced Micro Devices, Inc.
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

ROOTDIR := ../../..

CALMANAGERSPATH := ./Managers
CALORACLESPATH := ./Oracles
CALSPACIALSPATH := ./Specials
CALUTILSPATH := ./Utils
CALWRAPPERSPATH := ./Wrappers
CALINTEROPERABLEPATH := ./Interoperable

vpath %.cpp $(CALMANAGERSPATH)
vpath %.cpp $(CALORACLESPATH)
vpath %.cpp $(CALSPACIALSPATH)
vpath %.cpp $(CALUTILSPATH)
vpath %.cpp $(CALWRAPPERSPATH)
vpath %.cpp $(CALINTEROPERABLEPATH)

OUTPUTBASE := /platform/runtime/lib

FILES := CALBufferMgr CALExecMgr \
         CALKernelOracle CALStreamOracle \
         CALKernelImpl \
         CALRuntime CALBuffer CALConstBuffer CALDevice CALProgram \
         CALD3D9Buffer CALD3D10Buffer

GENERATE_SHARED_LIBRARY := brook_cal

include $(ROOTDIR)/platform/utils/build/config.mk

ifeq ($(OS), lh) # Only vista allows D3D9 D3D10 interoperability
	CFLAGS += /DENABLE_D3D9_INTEROP /DENABLE_D3D10_INTEROP
	CFLAGS += $(C_INCLUDE_FLAG)"$(DXSDK_DIR)/Include"
endif

ifdef RELEASE
	LIBRARIES := brook
else
	LIBRARIES := brook_d
endif


ifeq ($(BITS), 32)
        CALLIBS += aticalcl aticalrt
else
        ifneq ($(OS), lnx)
                CALLIBS += aticalcl64 aticalrt64
        else
                CALLIBS += aticalcl aticalrt
        endif
endif


include $(ROOTDIR)/platform/utils/build/main.mk

CFLAGS += $(C_INCLUDE_FLAG)$(INCLUDEDIR)/brook
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Wrappers"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Utils"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Managers"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Oracles"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Specials"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/CAL/Interoperable"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/include/brook/CPU"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Wrappers"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Managers"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Oracles"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Utils"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Impls"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Interfaces"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/InterfaceData"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform/runtime/Common/Specials"
CFLAGS += $(C_INCLUDE_FLAG)"$(ROOTDIR)/platform\runtime\CPU"



