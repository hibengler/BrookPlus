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

#include "blockallocator.hpp"
#ifdef BALLOC_ENABLED
//#include "../GenericUtilities/oscompatibility.hpp" Earlier version uses this
#include "oscompatibility.hpp"
#ifdef THREADSAFE
//for short and likely uncontested locks, spinlocks outperform mutexes
inline static void spinlock(ATOMIC_COUNTER_TYPE& i)
{
  if(ATOMIC_INCREMENT(i)!=1)
  { //failed to acquire the lock
    ATOMIC_DECREMENT(i);
    while(i>0);
    spinlock(i);
  }
}
inline static void spinunlock(ATOMIC_COUNTER_TYPE& i)
{
  ATOMIC_DECREMENT(i);
}

static ATOMIC_COUNTER_TYPE theLock = 0;
#define GLOBAL_LOCK() spinlock(theLock)
#define GLOBAL_UNLOCK() spinunlock(theLock)

#else //THREADSAFE
#define GLOBAL_LOCK()
#define GLOBAL_UNLOCK()
#endif //THREADSAFE


#ifndef BALLOC_REAL_MALLOC
#define BALLOC_REAL_MALLOC malloc
#endif
#ifndef BALLOC_REAL_FREE
#define BALLOC_REAL_FREE   free
#endif 


using namespace amd_block_allocator;

#ifdef BALLOC_COUNT_ALLOCS
static int theOverflowCount = 0;
static int theAllocCnt = 0;
static int theFreeCnt = 0;
#endif

//oh == overhead
#define OH sizeof(BufferHdr)

//here we configure our different levels of blocked-based allocation
//this has been tuned to compile leather_dx10sdk_hlsl fast
typedef BlockAllocator< 16-OH,  2048 > BlockAllocatorA; //32kb per Block
typedef BlockAllocator< 32-OH,  2048 > BlockAllocatorB; //64kb per Block
typedef BlockAllocator< 48-OH,  1536 > BlockAllocatorC; //64kb per Block
typedef BlockAllocator< 64-OH,  1024 > BlockAllocatorD; //64kb per Block
typedef BlockAllocator< 128-OH, 256  > BlockAllocatorE; //32kb per Block
typedef BlockAllocator< 256-OH, 64   > BlockAllocatorF; //16kb per Block

//drop in replacement for new/malloc
void* 
amd_block_alloc_malloc(size_t sz)
{
#ifdef BALLOC_COUNT_ALLOCS
  theAllocCnt++;
#endif

  //dispatch to the correct BlockAllocator

  if(sz <= BlockAllocatorA::BUFSIZE)
    return BlockAllocatorA::Instance().Allocate()->toBuffer();

  if(sz <= BlockAllocatorB::BUFSIZE)
    return BlockAllocatorB::Instance().Allocate()->toBuffer();

  if(sz <= BlockAllocatorC::BUFSIZE)
    return BlockAllocatorC::Instance().Allocate()->toBuffer();

  if(sz <= BlockAllocatorD::BUFSIZE)
    return BlockAllocatorD::Instance().Allocate()->toBuffer();

  if(sz <= BlockAllocatorE::BUFSIZE)
    return BlockAllocatorE::Instance().Allocate()->toBuffer();

  if(sz <= BlockAllocatorF::BUFSIZE)
    return BlockAllocatorF::Instance().Allocate()->toBuffer();

#ifdef BALLOC_COUNT_ALLOCS
  theOverflowCount++;
#endif

  //fallback for really large blocks
  return BufferHdr::AllocateSingleBuffer(sz)->toBuffer();
}

//drop in replacement for delete/free
void 
amd_block_alloc_free(void* ptr)
{
  if(ptr==0) return;
#ifdef BALLOC_COUNT_ALLOCS
  theFreeCnt++;
#endif

  BufferHdr* hdr = BufferHdr::fromBuffer((char*)ptr);
  if(hdr->isValid())
    hdr->markFree(); //return block to pool of free blocks
  else
    std::cerr << "WARNING: invalid call to free(" << std::hex << ptr << ')' << std::endl;
}

#ifdef BALLOC_COUNT_ALLOCS
void 
PrintDebugAllocReport()
{
  std::cerr << std::endl << "Allocator Report (num_overflow=" << theOverflowCount << " || allocated=" << theAllocCnt << "-" << theFreeCnt << "=" << (theAllocCnt-theFreeCnt) << ')' << std::endl;
  BlockAllocatorA::Instance().debugPrint();
  BlockAllocatorB::Instance().debugPrint();
  BlockAllocatorC::Instance().debugPrint();
  BlockAllocatorD::Instance().debugPrint();
  BlockAllocatorE::Instance().debugPrint();
  BlockAllocatorF::Instance().debugPrint();
}
#endif 

template< int BUFSIZE, int BUFCNT >
void 
BlockAllocator<BUFSIZE, BUFCNT>::Block::construct( BufferHdr* prev )
{
  for(int i=0; i<BUFCNT; ++i)
  {
    BufferHdr* hdr = (BufferHdr*)(buffer+(i*HDRBUFSIZE));
    //prev element points to this element
    prev->construct(hdr);
    prev = hdr;
  }
  //last element points to NULL
  prev->construct(NULL);
}

template< int BUFSIZE, int BUFCNT >
void 
BlockAllocator<BUFSIZE, BUFCNT>::allocateNewBlock()
{
  //Note that we do not maintain a pointer to the allocated blocks. These will 
  //never be freed back to the OS.  This is a deliberate choice done for three
  //reasons:
  // 1) To avoid thrashing memory between the app an the OS
  // 2) Since this system is used only for small blocks of memory, cost is small
  //   (Program allocations >256 bytes will be handled by malloc/free directly)
  // 3) Since we block thousands of small allocations together the chance that 
  //    ALL of those small buffers will be free at the same time is very small.
 
  Block* tmp = (Block*)BALLOC_REAL_MALLOC(sizeof(Block));
  tmp->construct(&mUnallocatedBufferStack);
#ifdef BALLOC_COUNT_ALLOCS
  mNumBlocks++;
#endif
}

template< int BUFSIZE, int BUFCNT >
BufferHdr* 
BlockAllocator<BUFSIZE, BUFCNT>::Allocate()
{
  GLOBAL_LOCK();
  
  if(mUnallocatedBufferStack.next == NULL) allocateNewBlock();
  BufferHdr* block = mUnallocatedBufferStack.next;
  block->markUsed(&mUnallocatedBufferStack);

#ifdef BALLOC_COUNT_ALLOCS
  mAllocCnt++;
#endif 
  
  GLOBAL_UNLOCK();

#ifdef BALLOC_POISON_BUFFERS
  memset(block->toBuffer(), 0xEE, BUFSIZE);
#endif 

  return block;
}

template< int BUFSIZE, int BUFCNT >
BlockAllocator<BUFSIZE, BUFCNT>& 
BlockAllocator<BUFSIZE, BUFCNT>::Instance()
{

  static BlockAllocator* inst = 0;
  if(inst == 0) //quick check without a lock (will be false all but once)
  { 
    GLOBAL_LOCK();
    if(inst == 0) //real check now that we have the lock 
    {
      //we leak this on purpose so our allocator wont break during program shutdown
      inst = (BlockAllocator*)BALLOC_REAL_MALLOC(sizeof(BlockAllocator));
      memset(inst, 0, sizeof(BlockAllocator));
      inst->allocateNewBlock();
    }
    GLOBAL_UNLOCK();
  }  
  return *inst;
}

template< int BUFSIZE, int BUFCNT >
void 
BlockAllocator<BUFSIZE, BUFCNT>::debugPrint()
{
  #ifdef BALLOC_COUNT_ALLOCS
  std::cerr << "BlockAllocator" << BUFSIZE << " num_allocs=" << mAllocCnt << " num_blocks="  << mNumBlocks << std::endl;
  #endif
}

void 
amd_block_allocator::BufferHdr::construct( BufferHdr* n )
{
  next = n;
#ifdef BALLOC_ERROR_CHECKING
  magic_token = 0;
#endif
}

bool 
amd_block_allocator::BufferHdr::isValid() const
{
#ifdef BALLOC_ERROR_CHECKING
  return magic_token==MAGIC_TOKEN; 
#else
  return true;
#endif
}

void 
amd_block_allocator::BufferHdr::markUsed( BufferHdr* root )
{
  root->next = next;
  next = root;
#ifdef BALLOC_ERROR_CHECKING
  magic_token=MAGIC_TOKEN; 
#endif
}

void 
amd_block_allocator::BufferHdr::markFree()
{
  //handle special case for very large blocks (allocs by allocateSingleBlock())
  if(next == NULL)
  {
    BALLOC_REAL_FREE(this);
  }
  else
  {
#ifdef BALLOC_ERROR_CHECKING
    magic_token=0; 
#endif

    //while allocated, our "next" pointer pointed to the top of the free block stack
    BufferHdr* root = next;
    GLOBAL_LOCK();
    next = root->next;    
    root->next = this;
    GLOBAL_UNLOCK();
  }
}

BufferHdr* 
amd_block_allocator::BufferHdr::AllocateSingleBuffer( size_t sz )
{
  BufferHdr* block = (BufferHdr*)BALLOC_REAL_MALLOC(sizeof(BufferHdr) + sz);
  block->construct(NULL);
#ifdef BALLOC_ERROR_CHECKING
  block->magic_token=MAGIC_TOKEN; 
#endif
  return block;
}

#endif //BALLOC_ENABLED




