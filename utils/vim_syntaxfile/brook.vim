" Vim syntax file
" Language:	Brook
" Maintainer:	
" Last Change:	

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Read the C syntax to start with
if version < 600
  so <sfile>:p:h/cpp.vim
else
  runtime! syntax/cpp.vim
  unlet b:current_syntax
endif

"Brook extensions
syn keyword brKernel            kernel reduce iter out 
syn keyword brFunctions         streamWrite streamRead indexof instance streamDomain streamGroupEx streamGroup streamStencilEx streamStencil streamFlatte streamStride streamReplicate streamRepeate streamCat streamMerge streamGather streamScatter streamRead streamWrite streamToD3D streamFromD3D streamToGL streamFromGL
syn keyword brNumber            int2 int3 int4 uint uint2 uint3 uint4 float2 float3 float4 double2 short2 short3 short4 ushort ushort2 ushort3 ushort4 char2 char3 char4 uchar uchar2 uchar3 uchar4

" The minimum and maximum operators in GNU C++
syn match cppMinMax "[<>]?"

" Default highlighting
if version >= 508 || !exists("did_cpp_syntax_inits")
  if version < 508
    let did_cpp_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif
  HiLink brKernel               cppStatement
  HiLink brFunctions            cppStatement
  HiLink brNumber               Type
  delcommand HiLink
endif

let b:current_syntax = "cpp"

" vim: ts=8
