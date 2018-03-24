/* ============================================================
Copyright (c) 2007 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the 
following conditions:
 
Redistributions must retain the above copyright notice and all terms of 
this license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE 
REVOCATION OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS 
FREE FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - 
WHETHER EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, 
ANY IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE, ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR 
NON-INFRINGEMENT. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY 
COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, PUNITIVE, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
REVENUE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED 
ON ANY THEORY OF LIABILITY ARISING IN ANY WAY RELATED TO THIS MATERIAL, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE 
LIABILITY OF ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS (US $10.00). ANYONE REDISTRIBUTING 
OR ACCESSING OR USING THIS MATERIAL ACCEPTS THIS ALLOCATION OF RISK AND AGREES 
TO RELEASE ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS FROM ANY AND ALL LIABILITIES, OBLIGATIONS, CLAIMS, OR DEMANDS IN 
EXCESS OF TEN DOLLARS (US $10.00). THE FOREGOING ARE ESSENTIAL TERMS OF THIS 
LICENSE AND, IF ANY OF THESE TERMS ARE CONSTRUED AS UNENFORCEABLE, FAIL IN 
ESSENTIAL PURPOSE, OR BECOME VOID OR DETRIMENTAL TO ADVANCED MICRO DEVICES, 
INC. OR ANY COPYRIGHT HOLDERS OR CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS 
TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL SHALL TERMINATE IMMEDIATELY. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF THIS 
LICENSE OR ANY AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS 
SUCH AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS 
AS DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS,
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is provided 
with “RESTRICTED RIGHTS” and/or “LIMITED RIGHTS” as applicable to computer 
software and technical data, respectively. Use, duplication, distribution or 
disclosure by the U.S. Government and/or DOD agencies is subject to the full 
extent of restrictions in all applicable regulations, including those found at 
FAR52.227 and DFARS252.227 et seq. and any successor regulations thereof. Use 
of this material by the U.S. Government and/or DOD agencies is acknowledgment 
of the proprietary rights of any copyright holders and contributors, including 
those of Advanced Micro Devices, Inc., as well as the provisions of FAR52.227-14 
through 23 regarding privately developed and/or commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, 
title, or interest in, or relating to, this material. No terms of this license 
can be modified or waived, and no breach of this license can be excused, unless 
done so in a writing signed by all affected parties. Each term of this license 
is separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.
============================================================ */

/********************************************************************

	purpose:	This file implements a method to aggregate lots of 
                small allocations into larger allocations.  This can improve
                speed a lot depending on program.
*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <iostream>

//Comment this line to revert to using malloc/free
#ifndef DISABLE_BALLOC
#define BALLOC_ENABLED
#endif

#ifdef DEBUG
//Put a magic token in BufferHdr to validate calls to free()
#define BALLOC_ERROR_CHECKING

//Fill new buffers with 0xEE to help find uninitialized memory
#define BALLOC_POISON_BUFFERS
#endif 

//Count the number of allocations (for profiling)
//#define BALLOC_COUNT_ALLOCS

//We don't use threads yet, so THREADSAFE is disabled
//#define THREADSAFE

#ifdef BALLOC_ENABLED

//drop in replacement for new/malloc
void* amd_block_alloc_malloc(size_t sz);

//drop in replacement for delete/free
void amd_block_alloc_free(void* ptr);

#else //BALLOC_ENABLED
#  define amd_block_alloc_malloc malloc
#  define amd_block_alloc_free free
#  undef BALLOC_COUNT_ALLOCS
#endif//BALLOC_ENABLED

#ifndef BALLOC_COUNT_ALLOCS
#define PrintDebugAllocReport()
#endif

#ifdef BALLOC_ENABLED

namespace amd_block_allocator
{

/**
 * A struct containing metadata that we will prefix before every allocated 
 * buffer given to the program
 */
struct BufferHdr
{
  ///IF(free)      A pointer to the next buffer in the unallocated buffer pool
  ///IF(allocated) A pointer to the root of the pool we should be returned to
  ///IF(large buffer) NULL
  BufferHdr* next;
 
#ifdef BALLOC_ERROR_CHECKING
  ///Secret token to ensure we don't go poking at memory not belonging to us
  unsigned int magic_token;
  enum { MAGIC_TOKEN = 0xA110CA1E };
#endif 

  ///Setup all members with the given values, used to initialize
  void construct(BufferHdr* n);

  ///Test if our magic_token bit is correct
  bool isValid() const;

  ///Conversion macro
  char* toBuffer() { return ((char*)this)+sizeof(BufferHdr); }

  ///Conversion macro
  static BufferHdr* fromBuffer(char* b){ return (BufferHdr*)(b-sizeof(BufferHdr));}

  ///Remove this buffer from the stack of free buffers
  void markUsed(BufferHdr* root);

  ///Return this buffer to the stack of free buffers
  void markFree();

  ///
  ///Fall back cased used for very large allocations
  static BufferHdr* AllocateSingleBuffer(size_t sz);
};

/**
 * Main allocator class.  Exactly one instance for each set of unique template
 * parameters. 
 * - BUFSIZE:  The size of the buffer returned by Allocate()
 * - BUFCNT :  The number of buffers allocated in each block
 */
template< int _BUFSIZE, int BUFCNT >
class BlockAllocator
{
public:
  enum { BUFSIZE = _BUFSIZE };

protected:

  enum { HDRBUFSIZE = sizeof(BufferHdr)+BUFSIZE };

  /**
   * A single unit that will be allocated contains BUFCNT buffers and headers                                                                    
   */
  struct Block
  {
    char buffer[ HDRBUFSIZE * BUFCNT ];

    ///
    /// Setup a linked list of BufferHdrs in buffer.  Start the linked list at prev
    void construct(BufferHdr* prev);
  };

  /// Allocate a new block of BUFCNT buffers.  May only be called when mFreeBufferList->next == 0.
  void allocateNewBlock();

public:

  ///
  /// Allocate a single buffer of size BUFSIZE
  BufferHdr* Allocate();

  /// Get reference to singleton instance of this class (with these template args)
  static BlockAllocator& Instance();

  ///
  /// Print number of allocations to stderr
  void debugPrint();
private:
  BufferHdr mUnallocatedBufferStack;
  
#ifdef BALLOC_COUNT_ALLOCS
  int      mNumBlocks;
  int      mAllocCnt;
#endif 
};

}

//print allocation counts to stderr
#ifdef BALLOC_COUNT_ALLOCS
void PrintDebugAllocReport();
#endif

#endif //BALLOC_ENABLED

