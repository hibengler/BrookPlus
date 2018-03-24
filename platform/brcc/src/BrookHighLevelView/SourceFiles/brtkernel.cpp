/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.


The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

/*
 * brtkernel.cpp
 *
 *      Classes reflecting the body of Brook kernels for the different
 *      backends.  Each one knows how to build itself from a function
 *      definition and then how to emit C++ for itself.
 */
#ifdef _WIN32
#pragma warning(disable:4786)
//the above warning disables visual studio's annoying habit of warning when using the standard set lib
#endif
#include <stdio.h>
//someone removed the above header...it is needed for linux
#include <cstring>
#include <cassert>
#include <sstream>

#include "janitorial.hpp"
#include "logger.hpp"

#include "brtkernel.h"
#include "brtexpress.h"
#include "main.h"
#include "types.h"

#include "cgTool.h"

std::string whiteout(std::string s)
{
    LOG(LOG_FN_ENTRY, "whiteout(%s)\n", s.c_str());
    for (unsigned int i = 0; i < s.length(); ++i)
    {
        s[i] = ' ';
    }
    return s;
}

std::string whiteSpace(int count)
{
    LOG(LOG_FN_ENTRY, "whiteSpace(%d)\n", count);
    std::string s;
    for (int i = 0; i < count; ++i)
    {
        s.append(" ");
    }
    return s;
}
// checks if a function is a gather stream
extern bool recursiveIsGather(Type *);


std::ostream & operator<<(std::ostream & o, const BRTKernelCode & k)
{
    LOG(LOG_FN_ENTRY, "operator<<(%p, %p)\n", &o, &k);
    if (k.standAloneKernel())
        k.printCode(o);
    else
        k.onlyPrintInner(o);
    return o;
}


bool BRTKernelCode::standAloneKernel() const
{
    LOG(LOG_FN_ENTRY, " BRTKernelCode::standAloneKernel()\n");
    return fDef->returnsVoid();
}


BRTGPUKernelCode::BRTGPUKernelCode(const FunctionDef & _fDef):BRTKernelCode(_fDef)
{
    LOG(LOG_FN_ENTRY, "BRTGPUKernelCode::BRTGPUKernelCode(%p)\n", &_fDef);
}

#if BROOK_TO_IL_CODE_GEN_PATH
SymEntry *find_parameter_in_current_kernel_by_name(std::string const &pname);

static Variable *NewGatherArg(Variable * v)
{
    LOG(LOG_FN_ENTRY, " NewGatherArg(%p)\n", v);
    Symbol *s = new Symbol;

    s->name = "_const_" + v->name->name + "_scalebias";
    s->entry = find_parameter_in_current_kernel_by_name(s->name);
    return new Variable(s, v->location);
}

static Variable *NewAddressTransArg(Variable * v, const char *prefix)
{
    LOG(LOG_FN_ENTRY, " NewAddressTransArg(%p, %s)\n", v, prefix);
    Symbol *s = new Symbol;

    s->name = prefix + v->name->name;
    s->entry = find_parameter_in_current_kernel_by_name(s->name);
    return new Variable(s, v->location);
}

#endif

// TIM: HACK
int getGatherStructureSamplerCount(Type * form);
int getGatherRank(Type * form);


// This function prints the code of an internally callable kernel
// from within another kernel.
void BRTGPUKernelCode::printInnerCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTGPUKernelCode::printInnerCode(%p)\n", &out);
    unsigned int i;

    std::string myvoid("void  ");
    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    ft->printBase(out, 0);
    out << fDef->decl->name->name << " (";
    std::string blank(whiteout(myvoid + fDef->decl->name->name + " ("));

    for (i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
            out << "," << std::endl << blank;

        Symbol *nam = ft->args[i]->name;
        Type *t = ft->args[i]->form;

        if (recursiveIsGather(t))
        {
            int rank = getGatherRank(t);

            out << "_stype" << rank << " " << nam->name << "[";
            // TIM: HACK:
            out << getGatherStructureSamplerCount(t);
            out << "]";
        }
        else
        {
            if ((ft->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
                out << "inout ";

            if (ft->args[i]->isStream())
                t = static_cast < ArrayType * >(t)->subType;

            t->printType(out, nam, true, 0);
        }
    }

    out << ")" << std::endl;
    fDef->Block::print(out, 0);
}

#if BROOK_TO_IL_CODE_GEN_PATH
int getCurrentInstanceId();

void BRTGPUKernelCode::printILCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTGPUKernelCode::printILCode(%p)\n", &out);
    if ((globals.userTargets & TARGET_CAL) != 0)
    {
        out << IL_PREFIX_STRING << "func " << getCurrentInstanceId() << "\n";
        fDef->Block::print_il(out, 0);
        out << IL_PREFIX_STRING << "ret\n";
    }
}

static Decl *mk_indexof_param_decl(std::string const &name)
{
    LOG(LOG_FN_ENTRY, "mk_indexof_param_decl(%s)\n", name.c_str());
    Symbol *sym = new Symbol;

    sym->name = name;

    Decl *r = new Decl(sym);

    sym->entry = mk_paramdecl(name, r);

    BaseType *bt = new BaseType(BT_Float4);

    bt->qualifier |= TQ_GeneratedIndexOf;
    bt->qualifier |= TQ_Const;

    r->form = bt;

    return r;
}

static Decl *mk_gatherconst_param_decl(std::string const &name)
{
    LOG(LOG_FN_ENTRY, "mk_gatherconst_param_decl(%s)\n", name.c_str());
    Symbol *sym = new Symbol;

    sym->name = name;

    Decl *r = new Decl(sym);

    sym->entry = mk_paramdecl(name, r);

    BaseType *bt = new BaseType(BT_Float4);

    bt->qualifier |= TQ_GeneratedGatherConst;
    r->form = bt;

    return r;
}

void slide_indexof_names(std::string const &name, int count)
{
    LOG(LOG_FN_ENTRY, "slide_indexof_names(%s, %d)\n", name.c_str(), count);
    //int i = 0;
    std::set < unsigned int >::iterator iter = FunctionProp[name].begin();
    std::set < unsigned int >::iterator iterend = FunctionProp[name].end();

#if 0                           // FIXME - this appears to be nonsense, writes to const region
    for (; iter != iterend; ++iter, ++i)
        *iter = *iter + count;
#endif
}

void add_extra_kernel_parameters_for_the_gpu(FunctionDef * fd)
{
    LOG(LOG_FN_ENTRY, " add_extra_kernel_parameters_for_the_gpu(%p)\n", fd);
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);

    std::vector < Decl * >indexofs;

    std::set < unsigned int >::iterator iter = FunctionProp[fd->decl->name->name].begin();
    std::set < unsigned int >::iterator iterend = FunctionProp[fd->decl->name->name].end();

    for (; iter != iterend; ++iter)
    {
        //if( (ft->args[*iter]->form->getQualifiers() & TQ_Out) == 0 )
        indexofs.push_back(mk_indexof_param_decl("__indexof_" + ft->args[*iter]->name->name));
        //else
        //    indexofs.push_back( mk_function_param_decl( "__indexofoutput" ) );
    }

    for (u32 i = 0; i < ft->nArgs; ++i)
    {
        Symbol *nam = ft->args[i]->name;
        Type *t = ft->args[i]->form;

        if (recursiveIsGather(t))
        {
            if (globals.enableGPUAddressTranslation)
            {
                ft->insertArg(mk_gatherconst_param_decl("__gatherlinearize_" + nam->name), i + 1);
                ft->insertArg(mk_gatherconst_param_decl("__gathertexshape_" + nam->name), i + 2);
                ft->insertArg(mk_gatherconst_param_decl("__gatherdomainmin_" + nam->name), i + 3);
                // slide_indexof_names( fd->decl->name->name, 3 );
            }
            else
            {
                ft->insertArg(mk_gatherconst_param_decl("_const_" + nam->name + "_scalebias"), i + 1);
                // slide_indexof_names( fd->decl->name->name, 3 );
            }
        }
        else
        {
            if ((ft->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
            {
                // TODO
                // out << "inout ";
            }
        }
    }

    for (u32 i = 0; i < indexofs.size(); ++i)
        ft->addArg(indexofs[i]);
}


// This function converts gathers into scale and bias expressions
// This function converts function calls' gathers into two args.
// This function adds the indexof items to function calls requriing indexof

Expression *BRTGPUKernelCode::ConvertGathers(Expression * expr)
{
    LOG(LOG_FN_ENTRY, "BRTGPUKernelCode::ConvertGathers(%p)\n", expr);
    BrtGatherExpr *gather;

    /* Check function calls inside of kernels */

    if (expr->etype == ET_FunctionCall)
    {
        //now we have to convert gathers that are passed into functions
        FunctionCall *fc = static_cast < FunctionCall * >(expr);

        if (fc->function->etype == ET_Variable)
        {
            Variable *function = static_cast < Variable * >(fc->function);

            if (function->name->entry && function->name->entry->uVarDecl)
            {
                if (function->name->entry->uVarDecl->isKernel() && !function->name->entry->uVarDecl->isReduce())
                {

                    std::set < unsigned int >::iterator iter = FunctionProp[function->name->name].begin();
                    std::set < unsigned int >::iterator iterend = FunctionProp[function->name->name].end();

                    for (; iter != iterend; ++iter)
                    {
                        if (fc->args[*iter]->etype != ET_Variable)
                        {
                            std::cerr << "Error: ";
                            fc->args[*iter]->location.printLocation(std::cerr);
                            std::cerr << "Argument " << *iter + 1 << " not a stream where";
                            std::cerr << "indexof used in subfunction";
                        }
                        else
                        {
                            Variable *v = static_cast < Variable * >(fc->args[*iter]);

                            if (v->name->entry && v->name->entry->uVarDecl)
                            {
                                if (v->name->entry->uVarDecl->isStream())
                                {
                                    //Decl *indexofDecl = new Decl(new BaseType(BT_Float4));

                                    Symbol *indexofS = new Symbol;

                                    indexofS->name = "__indexof_" + v->name->name;
                                    //indexofS->entry = mk_vardecl(indexofS->name,indexofDecl);
                                    indexofS->entry = find_parameter_in_current_kernel_by_name(indexofS->name);
                                    fc->addArg(new Variable(indexofS, v->location));
                                }
                                else
                                {
                                    std::cerr << "Error: ";
                                    v->location.printLocation(std::cerr);
                                    std::cerr << " Argument " << *iter + 1 << " not a stream";
                                    std::cerr << "where indexof used in subfunction";
                                }
                            }
                        }
                    }

                    int i;

                    for (i = 0; i < fc->nArgs(); ++i)
                    {
                        if (fc->args[i]->etype == ET_Variable)
                        {
                            Variable *v = static_cast < Variable * >(fc->args[i]);

                            if (v->name->entry && v->name->entry->uVarDecl)
                            {
                                if (recursiveIsGather(v->name->entry->uVarDecl->form))
                                {
                                    if (globals.enableGPUAddressTranslation)
                                    {
                                        ++i;
                                        fc->args.insert(fc->args.begin() + i,
                                                        NewAddressTransArg(v, "__gatherlinearize_"));
                                        ++i;
                                        fc->args.insert(fc->args.begin() + i,
                                                        NewAddressTransArg(v, "__gathertexshape_"));
                                        ++i;
                                        fc->args.insert(fc->args.begin() + i,
                                                        NewAddressTransArg(v, "__gatherdomainmin_"));
                                    }
                                    else
                                    {
                                        ++i;
                                        fc->args.insert(fc->args.begin() + i, NewGatherArg(v));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return expr;
    }

    /* Convert gather expressions: a[i][j] */
    if (expr->etype == ET_IndexExpr)
    {

        if (globals.verbose)
        {
            std::cerr << "Found Index Expr: " << expr << std::endl;
        }

        // Check to see if the expression is from a gather stream
        IndexExpr *p = (IndexExpr *) expr;

        for (p = (IndexExpr *) p->array; p && p->etype == ET_IndexExpr; p = (IndexExpr *) p->array);

        // If things have gone horribly wrong
        if (!p)
            return expr;
        if (p->etype != ET_Variable)
            return expr;

        Variable *v = (Variable *) p;

        assert(v->name->entry);

        if (v->name->entry->type != ParamDeclEntry)
            return expr;

        // XXX Daniel: BrtGatherExpr asserts that it is
        //             indeed an array, not a TT_Stream
        if (v->name->entry->uVarDecl)
            if (v->name->entry->uVarDecl->form)
                if (v->name->entry->uVarDecl->form->type != TT_Array
                    || (v->name->entry->uVarDecl->form->getQualifiers() & TQ_Out) !=0)
                    return expr;
        gather = new BrtGatherExpr((IndexExpr *) expr);

        expr->replace(gather);

        // IAB: XXX For some reason I can't delete expr!!!
        //delete expr;
        return gather;
    }
    return expr;
}


void BRTFP30KernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTFP30KernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_FP30);
}


void BRTFP40KernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTFP40KernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_FP40);
}


void BRTARBKernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTARBKernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_ARB);
}


void BRTPS20KernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS20KernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_PS20);
}


void BRTPS2BKernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS2BKernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_PS2B);
}


void BRTPS2AKernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS2AKernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_PS2A);
}


void BRTPS30KernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS30KernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_PS30);
}
#endif 

void BRTCALKernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCALKernelCode::printCode(%p)\n", &out);
    printCodeForType(out, CODEGEN_CAL);
}


void BRTGPUKernelCode::printCodeForType(std::ostream & out, CodeGenTarget target) const
{
    LOG(LOG_FN_ENTRY, " BRTGPUKernelCode::printCodeForType(%p, %d)\n", &out, target);
    FunctionType *fType;

    std::ostringstream wrapOut;
    char *fpcode = "Null";

    if (globals.verbose)
    {
        fDef->Block::print(wrapOut, 0);
        std::cerr << "***Wrapping***\n";
        fDef->decl->print(std::cerr, true);
        std::cerr << std::endl << wrapOut.str() << "\n**********\n";
    }

    assert(fDef->decl->form->type == TT_Function);
    fType = (FunctionType *) fDef->decl->form;
    //! commentted while removing Deprecated folder from project
    //fpcode = CodeGen_GenerateCode(fType->subType,
    //                              fDef->FunctionName()->name.c_str(), fType->args, fType->nArgs, NULL, target);
    out << fpcode;
    free(fpcode);
}

#if 0
// These functions no longer seem to be used


// This function prints out the type of a variable from a stream passed in
// it may optionally add indirection.
static void printType(std::ostream & out, Type * t, bool addIndirection, std::string name = "")
{
    LOG(LOG_FN_ENTRY, " printType(%p, %p, %d, %s)\n", &out, t, addIndirection, name.c_str());
    Symbol sym;

    sym.name = name;
    if (addIndirection)
        sym.name = std::string("*") + sym.name;
    t->printBase(out, 0);
    t->printBefore(out, &sym, 0);
    t->printAfter(out);
}


static std::string tostring(unsigned int i)
{
    LOG(LOG_FN_ENTRY, " tostring(%d)\n", i);
    char c[1024];

    c[1023] = 0;
    sprintf(c, "%d", i);
    return std::string(c);
}


static Symbol getSymbol(std::string in)
{
    LOG(LOG_FN_ENTRY, " getSymbol(%s)\n", in.c_str());
    Symbol name;

    name.name = in;
    return name;
}
#endif


bool recursiveIsArrayType(Type * form)
{
    LOG(LOG_FN_ENTRY, " recursiveIsArrayType(%p)\n", form);
    if ((form->getQualifiers() & TQ_Reduce) != 0)
    {
        return form->type == TT_Array;
    }
    return form->type == TT_Stream && (static_cast < ArrayType * >(form)->subType->type == TT_Array);
}

// Ian:  I'm so sorry about this...
bool horrible_horrible_indexof_hack;


// This function prints the function that is called from the inner loop
// of the CPU. This function may also be called from within other kernels.
void BRTCPUKernelCode::printInnerCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::printInnerCode(%p)\n", &out);

    Symbol enhanced_name;

    std::string myvoid("void  ");
    unsigned int i;

    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());

    Brook2Cpp_ConvertKernel(fDef);

    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    enhanced_name.name = "__" + fDef->decl->name->name + "_cpu_inner";
    std::string white = whiteout("void " + enhanced_name.name + "(");

    if (!(ft->getBase()->typemask & BT_Void))
    {
        out << "static ";
        white = "       " + white;
    }
    ft->printBase(out, 0);
    out << " ";
    ft->printBefore(out, &enhanced_name, 0);
    out << "(";

    // Print the function arguments
    for (i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
        {
            out << "," << std::endl << white;
        }
        Type *t = ft->args[i]->form;
        std::string varname = ft->args[i]->name->name;
        Symbol *nam = ft->args[i]->name;

        if (0)
        {                       // Uses addressable to get copy-semantics between kernels
            if (ft->args[i]->form->isArray() || ((ft->args[i]->form->getQualifiers()) & (TQ_Reduce | TQ_Out)) != 0)
            {
                nam->name = "&" + nam->name;
                if (ft->args[i]->form->isArray())
                    out << "const ";
            }

            // if it is a stream, loose the "<>" 
            if (ft->args[i]->isStream())
            {
                nam->name = "> " + nam->name;
                t = static_cast < ArrayType * >(t)->subType;
                out << "Addressable <";
            }
        }
        else
        {
            nam->name = "&" + nam->name;

            // if it is a stream, loose the "<>" 
            if (ft->args[i]->isStream())
            {
                t = static_cast < ArrayType * >(t)->subType;
            }

            // if it is not an out/reduce arg, make it const
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) == 0)
            {
                out << "const ";
            }

        }
        // if it is not an out/reduce arg, make it const NO LONGER 
        /*
           if ((ft->args[i]->form->getQualifiers() 
           & (TQ_Reduce | TQ_Out)) == 0) {
           out << "const ";
           } */

        // if it is a gather
        if (recursiveIsGather(t) || 
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, nam, true, 0);
            
            
            if (t->isArray() && (t->getQualifiers() & TQ_Out) != 0)
            {
                if (IsScatterOutType(t))
                {
                    out << ", const __BrtInt4 &indexof" << varname;
                    if(fDef->getShaderType() == ComputeShader)
                    {
                        out << ", const __BrtInt4 &__instanceInGroup";
                    }
                }

                else
                    out << ", const __BrtFloat4 &indexof" << varname;
            }
            continue;
        }

        // print the argument
        t->printType(out, nam, true, 0);

    }

    out << ")" << std::endl;

    // Ian:  I'm so sorry about this...
    horrible_horrible_indexof_hack = true;

    // Print the body
    fDef->Block::print(out, 0);

    horrible_horrible_indexof_hack = false;
}

void BRTCPUKernelCode::onlyPrintInner(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::onlyPrintInner(%p)\n", &out);
    printInnerCode(out);
}

static void printFetchElement(std::ostream & out, Decl * decl)
{
    LOG(LOG_FN_ENTRY, " printFetchElement(%p, %p)\n", &out, decl);
    Type *t = decl->form;

    t = static_cast < ArrayType * >(t)->subType;
    out << "(";
    t->printType(out, NULL, true, 0, true);
	// OpenMP CPU optimization
    out << "*) __k->FetchElem("
    << decl->name->name
    << ", __brt_idx)";
}

static void printFetchElementCompute(std::ostream & out, Decl * decl)
{
    LOG(LOG_FN_ENTRY, " printFetchElement(%p, %p)\n", &out, decl);
    Type *t = decl->form;

    t = static_cast < ArrayType * >(t)->subType;
    out << "(";
    t->printType(out, NULL, true, 0, true);
	// OpenMP CPU optimization
    out << "*) __k->FetchElem("
    << decl->name->name
    << ", __abs_idx)";
}

#define FuncRelLine -2
void BRTCPUKernelCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::printCode(%p)\n", &out);

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    Symbol enhanced_name;

    enhanced_name.name = "__" + fDef->decl->name->name + "_cpu";
    std::string myvoid("void  ");
    unsigned int i;

    std::string indent("  ");

    // Print the core function
    printInnerCode(out);

    // Print the function decl
    out << myvoid;
    fDef->decl->form->printBefore(out, &enhanced_name, 0);
	//OpenMP CPU optimization
	out << "(::brt::KernelC *__k, int __brt_idxstart, int __brt_idxend, bool __brt_isreduce)" 
      << std::endl;

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);
    //    out << "#line 1 \"" << globals.cppOutputName << "\"" << std::endl;

    out << "{" << std::endl;
    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    out << indent;

    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());

    Brook2Cpp_ConvertKernel(fDef);

    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    // Print the function arguments
    for (i = 0; i < ft->nArgs; ++i)
    {
        Symbol *nam = ft->args[i]->name;
        Type *t = ft->args[i]->form;

        nam->name = "arg_" + nam->name;

        // Print the arg declaration
        if (ft->args[i]->isStream())
        {
            out << "::brt::StreamInterface ";
        }
        else if (recursiveIsGather(t) ||
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, NULL, true, 0);
        }
        else
        {
            t->printType(out, NULL, true, 0);
        }

        out << "*" << nam->name << " = (";

        // Print the arg initialization
        if (ft->args[i]->isStream())
        {
            out << "::brt::StreamInterface ";
        }
        else if (recursiveIsGather(t) ||
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, NULL, true, 0);
        }
        else
        {
            t->printType(out, NULL, true, 0);
        }

        out << "*) __k->getVectorElement(" << i << ");" << std::endl;
        if (globals.printLineDirectives)
            fDef->location.printLineDirective(out, FuncRelLine);

        out << indent;
    }
    out << std::endl; 
    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);
    out << indent;

#if 0   // Disabled for now to support VS express edition
    // OpenMP CPU optimization - use for loop 
    out << "#ifdef _OPENMP" << std::endl;
    out << "#pragma omp parallel for schedule(dynamic, 16) if(!__brt_isreduce)" << std::endl;
    out << "#endif" << std::endl;
#endif

    out << indent << "for(int __brt_idx=__brt_idxstart; __brt_idx<__brt_idxend; __brt_idx++) {" << std::endl;
    out << indent << "if(!(__k->isValidAddress(__brt_idx))){ continue; }" << std::endl;

    for (i = 0; i < ft->nArgs; ++i)
    {
        if (ft->args[i]->isStream())
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                if (globals.printLineDirectives)
                    fDef->location.printLineDirective(out, FuncRelLine);

                Type *t = static_cast < ArrayType * >(ft->args[i]->form)->subType;

                out << indent << indent << "Addressable <";
                Symbol sym;

                sym.name = "> __out_" + ft->args[i]->name->name;
                t->printType(out, &sym, true, 0, false);
                out << "(";
                printFetchElement(out, ft->args[i]);
                out << ");" << std::endl;
            }
        }
    }

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    std::string white = whiteout(enhanced_name.name + "_inner (");

    out << indent << indent << enhanced_name.name << "_inner (" << std::endl;

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);
    for (i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0) {
            out << "," << std::endl;
            if (globals.printLineDirectives)
                fDef->location.printLineDirective(out, FuncRelLine);
        }

        out << indent << indent << white;

        if (ft->args[i]->isStream())
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                out << "__out_" + ft->args[i]->name->name;
            }
            else
            {
                out << "Addressable <";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, false);
                out << ">(";
                printFetchElement(out, ft->args[i]);
                out << ")";
            }
        }
        else
        {
            out << "*" << ft->args[i]->name->name;
            if (ft->args[i]->isArray() && (ft->args[i]->form->getQualifiers() & TQ_Out) != 0)
            {
                if (IsScatterOutType(ft->args[i]->form))
                {
                    out << ", __get_scatter__address(__BrtInt1((int)__brt_idx))";
                    if(fDef->getShaderType() == ComputeShader)
                    {
                        out << ", __get_threadId__InGroup(__BrtInt1((int)__brt_idx))";
                    }
                }
                else
                    out << ", __BrtFloat4((float)__brt_idx, 0.0f, 0.0f, 0.0f)";
            }
        }
    }
    out << ");" << std::endl;
    for (i = 0; i < ft->nArgs; ++i)
    {

        if (ft->args[i]->isStream())
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                if (globals.printLineDirectives)
                    fDef->location.printLineDirective(out, FuncRelLine);

                out << indent << indent << "*reinterpret_cast<";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, true);
                out << "*>(__out_" << ft->args[i]->name->name << ".address)" << " = __out_" << ft->args[i]->name->
                    name << ".castToArg(*reinterpret_cast<";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, true);
                out << "*>(__out_" << ft->args[i]->name->name << ".address));" << std::endl;
            }
        }
    }
	// OpenMP CPU optimization
    out << indent<< "}"
    << std::endl 
    << "}" 
    << std::endl;

}

//! Prints the condition variable code to synchronize threads in a group
void BRTCPUKernelCode::printThreadConditionVariable(std::ostream& out) const
{
    out << "static CondVar __" << fDef->decl->name->name << "_cond_var;\n\n";
}

//! Prints structure for thread data required to call core function of kernel
void BRTCPUKernelCode::printThreadDataStruct(std::ostream& out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::printThreadDataStruct(%p)\n", &out);

    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());
    Brook2Cpp_ConvertKernel(fDef);
    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    out << "struct __" << fDef->decl->name->name << "_thread_data " << std::endl 
        << "{" << std::endl;

    std::string intent = "    ";
    std::string newLine = "\n";

    // Print the function arguments
    for (unsigned int i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
        {
            out << ";" << newLine << intent;
        }
        else
        {
            out << intent;
        }

        Type *t = ft->args[i]->form;
        std::string varname = ft->args[i]->name->name;
        Symbol *nam = ft->args[i]->name;
        std::string oldName = nam->name;
        nam->name = "*" + nam->name;

        if (ft->args[i]->isStream())
        {
            t = static_cast < ArrayType * >(t)->subType;
            nam->name = oldName;
        }

        // Gather and Scatter arrays
        if (recursiveIsGather(t) || (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);
            gt.printType(out, nam, true, 0);
            
            if (IsScatterOutType(t))
            {
                out << ";" << newLine << intent << "__BrtInt4 indexof" << varname;
                if(fDef->getShaderType() == ComputeShader)
                {
                    out << ";" << newLine << intent << "__BrtInt4 __instanceInGroup";
                }
            }
            continue;
        }

        //! Print element
        t->printType(out, nam, true, 0);
    }

    //! Shared array
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    ArrayType *arrayType = static_cast<ArrayType *>(dclType);
                    CPUGatherType gt(*(ArrayType *) arrayType, false);
                    out << ";" << newLine << intent; 
                    std::string oldName = localDecl->name->name;
                    localDecl->name->name = "*" + localDecl->name->name;
                    gt.printType(out, localDecl->name, true, 0);
                    localDecl->name->name = oldName;
                }
            }
        }
    }
    out << ";" << newLine;
    out << "};" << newLine << newLine;
}

//! Prints core function of kernel
void BRTCPUKernelCode::printThreadedInnerCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::printInnerCode(%p)\n", &out);

    Symbol enhanced_name;

    std::string myvoid("void  ");
    unsigned int i;

    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());

    Brook2Cpp_ConvertKernel(fDef);

    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    enhanced_name.name = "__" + fDef->decl->name->name + "_cpu_inner";
    std::string white = whiteout("void " + enhanced_name.name + "(");
    std::string newLine = "\n";

    if (!(ft->getBase()->typemask & BT_Void))
    {
        out << "static ";
        white = "       " + white;
    }
    ft->printBase(out, 0);
    out << " ";
    ft->printBefore(out, &enhanced_name, 0);
    out << "(";

    // Print the function arguments
    for (i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
        {
            out << "," << newLine << white;
        }
        Type *t = ft->args[i]->form;
        std::string varname = ft->args[i]->name->name;
        Symbol *nam = ft->args[i]->name;

        if (0)
        {                       // Uses addressable to get copy-semantics between kernels
            if (ft->args[i]->form->isArray() || ((ft->args[i]->form->getQualifiers()) & (TQ_Reduce | TQ_Out)) != 0)
            {
                nam->name = "&" + nam->name;
                if (ft->args[i]->form->isArray())
                    out << "const ";
            }

            // if it is a stream, loose the "<>" 
            if (ft->args[i]->isStream())
            {
                nam->name = "> " + nam->name;
                t = static_cast < ArrayType * >(t)->subType;
                out << "Addressable <";
            }
        }
        else
        {
            nam->name = "&" + nam->name;

            // if it is a stream, loose the "<>" 
            if (ft->args[i]->isStream())
            {
                t = static_cast < ArrayType * >(t)->subType;
            }

            // if it is not an out/reduce arg, make it const
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) == 0)
            {
                out << "const ";
            }

        }

        // if it is a gather
        if (recursiveIsGather(t) || 
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, nam, true, 0);
            
            
            if (t->isArray() && (t->getQualifiers() & TQ_Out) != 0)
            {
                if (IsScatterOutType(t))
                {
                    out << "," << newLine << white << "const __BrtInt4 &indexof" << varname;
                    if(fDef->getShaderType() == ComputeShader)
                    {
                        out << "," << newLine << white << "const __BrtInt4 &__instanceInGroup";
                    }
                }

                else
                    out << ", " << newLine << white << "const __BrtFloat4 &indexof" << varname;
            }
            continue;
        }

        // print the argument
        t->printType(out, nam, true, 0);

    }

    //! Shared array as core function parameter
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    ArrayType *arrayType = static_cast<ArrayType *>(dclType);
                    CPUGatherType gt(*(ArrayType *) arrayType, false);
                    Symbol *nam = localDecl->name;
                    nam->name = "&" + nam->name;
                    out << ", " << newLine << white; 
                    gt.printType(out, nam, true, 0);
                }
            }
        }
    }
    out << ")" << std::endl;

    // Ian:  I'm so sorry about this...
    horrible_horrible_indexof_hack = true;

    //! set kernel name for syncGroup() function
    setCurrentKernelName(fDef->decl->name->name);

    // Print the body
    fDef->Block::print(out, 0);

    //! reset kernel name for syncGroup() function
    resetCurrentKernelName();
    
    horrible_horrible_indexof_hack = false;

    out << newLine;
}

//! Prints Thread Entry point function required to call core function
void BRTCPUKernelCode::printThreadEntryPointFunc(std::ostream& out) const
{

    Symbol enhanced_name;
    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());

    Brook2Cpp_ConvertKernel(fDef);

    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    enhanced_name.name = "__" + fDef->decl->name->name + "_cpu_inner";
    std::string white = whiteout(enhanced_name.name + "(");

    std::string newLine = "\n";
    std::string intent = "    ";
    std::string threadDataPtr = "threadData->";
    
    out << "void * __" << fDef->decl->name->name << "_thread_entry_point(void * data)" << newLine
        << "{" << newLine;

    out << intent << "__" << fDef->decl->name->name << "_thread_data * threadData = " 
        << "(__" << fDef->decl->name->name << "_thread_data*) data;" << newLine;

    out << intent << enhanced_name.name << "(";

    // Print the function arguments
    for (unsigned int i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
        {
            out << "," << newLine << white << intent;
        }

        Type *t = ft->args[i]->form;
        std::string varname = ft->args[i]->name->name;
        Symbol *nam = ft->args[i]->name;

        if (ft->args[i]->isStream())
        {
            t = static_cast < ArrayType * >(t)->subType;
            out << threadDataPtr << varname;
            continue;
        }

        // Gather and Scatter arrays
        if (recursiveIsGather(t) || (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            out << "*" << threadDataPtr << varname;
            
            if (IsScatterOutType(t))
            {
                out << "," << newLine << white << intent << threadDataPtr << "indexof" << varname;
                if(fDef->getShaderType() == ComputeShader)
                {
                    out << "," << newLine << white << intent << threadDataPtr << "__instanceInGroup";
                }
            }
            continue;
        }

        //! Print element
        out << "*" << threadDataPtr << varname;
    }

    //! Shared array
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    out << "," << newLine << white << intent; 
                    out << "*" << threadDataPtr << localDecl->name->name;
                }
            }
        }
    }
    out << ");" << newLine;

    out << intent << "return NULL;" << newLine;
    out << "}" << newLine << newLine;
}

void BRTCPUKernelCode::printMultiThreadedCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::printCode(%p)\n", &out);

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    Symbol enhanced_name;

    enhanced_name.name = "__" + fDef->decl->name->name + "_cpu";
    std::string myvoid("void ");
    unsigned int i;

    //! Print condtion variable code
    out << "//! Condition variable stub for kernel \"" << fDef->decl->name->name << "\"" << std::endl;
    printThreadConditionVariable(out);

    std::string indent("    ");
    std::string newLine = "\n";

    //! Print Thread data structure declaration 
    out << "//! Thread data structure declaration for kernel \"" << fDef->decl->name->name << "\"" << std::endl;
    printThreadDataStruct(out);

    // Print the core function
    out << "//! Core function of kernel \"" << fDef->decl->name->name << "\"" << std::endl;
    printThreadedInnerCode(out);

    //! Print Thread Entry point function to call core function of kernel
    out << "//! Thread Entry point function to call core function of kernel \"" << fDef->decl->name->name << "\"" << std::endl;
    printThreadEntryPointFunc(out);

    // Print the Main function which calls the core function of kernel
    out << "//! Main function which calls the core function of kernel\"" << fDef->decl->name->name << "\"" << std::endl;
    out << myvoid;
    fDef->decl->form->printBefore(out, &enhanced_name, 0);
	out << "(::brt::KernelC *__k, int __brt_idxstart, int __brt_idxend, bool __brt_isreduce)" 
        << newLine;

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);
    //    out << "#line 1 \"" << globals.cppOutputName << "\"" << std::endl;

    out << "{" << std::endl;
    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    out << indent;

    FunctionDef *fDef = (FunctionDef *) (this->fDef->dup());

    Brook2Cpp_ConvertKernel(fDef);

    FunctionType *ft = static_cast < FunctionType * >(fDef->decl->form);

    // Print the function arguments
    for (i = 0; i < ft->nArgs; ++i)
    {
        Symbol *nam = ft->args[i]->name;
        Type *t = ft->args[i]->form;

        std::string oldName = nam->name;

        nam->name = "arg_" + nam->name;

        // Print the arg declaration
        if (ft->args[i]->isStream())
        {
            out << "::brt::StreamInterface ";
        }
        else if (recursiveIsGather(t) ||
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, NULL, true, 0);
        }
        else
        {
            t->printType(out, NULL, true, 0);
        }

        out << "*" << nam->name << " = (";

        // Print the arg initialization
        if (ft->args[i]->isStream())
        {
            out << "::brt::StreamInterface ";
        }
        else if (recursiveIsGather(t) ||
            (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            CPUGatherType gt(*(ArrayType *) t, false);

            gt.printType(out, NULL, true, 0);
        }
        else
        {
            t->printType(out, NULL, true, 0);
        }

        out << "*) __k->getVectorElement(" << i << ");" << std::endl;
        if (globals.printLineDirectives)
            fDef->location.printLineDirective(out, FuncRelLine);

        out << indent;

        nam->name = oldName;
    }
    out << newLine;

    //! Shared array declaration 
    out << indent << "//! Shared array declaration and Local data shared components" << newLine;
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    ArrayType *arrayType = static_cast<ArrayType *>(dclType);
                    BaseType* baseType = arrayType->getBase();

                    BTDetails details = getTypeDetails(baseType->getBase()->typemask);
                    out << whiteSpace(4) << "unsigned int __lds_dim = " << arrayType->getDimensions() << ";" << newLine;
                    out << whiteSpace(4) << "unsigned int __lds_dim_value[" << arrayType->getDimensions() << "] = {";
                    for(int i = 0; i < arrayType->getDimensions(); ++i)
                    {
                        out << arrayType->getDimensionsSize(arrayType->getDimensions() - i);
                    }
                    out << "};" << newLine;
                    std::string scalarStr = getScalarTypeStr(details.category);
                    out << whiteSpace(4) << "unsigned int __lds_element_size = " << "sizeof(" << scalarStr << ") * " << details.components << ";" << newLine;
                    out << whiteSpace(4) << scalarStr << "*__lds_data = new " << scalarStr << "[sizeof(" << scalarStr << ") * " 
                        << details.components << " * " << arrayType->getTotalElementCount() << "];" << newLine;
                    out << whiteSpace(4) << "__BrtArray<" << scalarStr << "> *__lds_array = new __BrtArray< " << scalarStr << ">("
                        << "__lds_data, __lds_dim, __lds_element_size, __lds_dim_value);" << newLine;

                    CPUGatherType gt(*(ArrayType *) arrayType, false);
                    out << whiteSpace(4) ;
                    gt.printType(out, NULL, true, 0);
                    out << "* " << localDecl->name->name << " = (";
                    gt.printType(out, NULL, true, 0);
                    out << "*) __lds_array;" << newLine << newLine;
                }
            }
        }
    }
    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);

    out << whiteSpace(4) << "int __actual_group_size = (int)__get_GroupSize();" << newLine;
    out << whiteSpace(4) << "int __group_count = __brt_idxend / __actual_group_size;" << newLine;
    out << whiteSpace(4) << "if(__brt_idxend % __actual_group_size != 0)" << newLine;
    out << whiteSpace(8) << "__group_count++;" << newLine;

    out << whiteSpace(4) << "int __group_size = 0;" << newLine;
    out << whiteSpace(4) << "for(int __brt_idx = __brt_idxstart; __brt_idx < __group_count; __brt_idx++)" << newLine
        << whiteSpace(4) << "{" << newLine;

    out << whiteSpace(8) << "//! Calculate actual threads in present group" << newLine;
    out << whiteSpace(8) << "if(__brt_idx == __group_count - 1)" << newLine;
    out << whiteSpace(12) << "__group_size = __brt_idxend - __brt_idx * __actual_group_size;" << newLine;
    out << whiteSpace(8) << "else" << newLine;
    out << whiteSpace(12) << "__group_size = __actual_group_size;" << newLine << newLine;

    out << whiteSpace(8) << "if(!(__k->isValidAddress(__brt_idx * __group_size))){ continue; }" << newLine << newLine;

#if 0 //! Output never be ordinary stream
    for (i = 0; i < ft->nArgs; ++i)
    {
        if (ft->args[i]->isStream())
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                if (globals.printLineDirectives)
                    fDef->location.printLineDirective(out, FuncRelLine);

                Type *t = static_cast < ArrayType * >(ft->args[i]->form)->subType;

                out << indent << indent << "Addressable <";
                Symbol sym;

                sym.name = "> __out_" + ft->args[i]->name->name;
                t->printType(out, &sym, true, 0, false);
                out << "(";
                printFetchElement(out, ft->args[i]);
                out << ");" << std::endl;
            }
        }
    }
#endif //! Output never be ordinary stream

    if (globals.printLineDirectives)
        fDef->location.printLineDirective(out, FuncRelLine);
    std::string kernelName = "__" + fDef->decl->name->name;

    out << whiteSpace(8) << "//! Thread pointers and thread data pointers" << newLine;
    out << whiteSpace(8) << "CPUThread* " << kernelName << "_threads  = "
        << "new CPUThread[__group_size];" << newLine;

    out << whiteSpace(8) << kernelName << "_thread_data * __thread_data = "
        << "new " << kernelName << "_thread_data[__group_size];" << newLine << newLine;

    //! Set thread_data 

    out << whiteSpace(8) << "//! Initialize thread data for all thread in a group" << newLine;
    out << whiteSpace(8) << "for(int i = 0; i < __group_size; ++i)" << newLine
        << whiteSpace(8) << "{" << newLine;
    out << whiteSpace(12) << "int __abs_idx = __brt_idx * __actual_group_size + i;" << newLine;
    
    //! Initialize thread data for thread one by one
    // Print the function arguments
    for (unsigned int i = 0; i < ft->nArgs; ++i)
    {
        if (i != 0)
        {
            out << ";" << newLine;
        }

        Type *t = ft->args[i]->form;
        std::string varname = ft->args[i]->name->name;

        if (ft->args[i]->isStream())
        {
            t = static_cast < ArrayType * >(t)->subType;
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                out << "__out_" + ft->args[i]->name->name;
            }
            else
            {
                out << whiteSpace(12) << "__thread_data[i]." << varname << " = ";
                out << "Addressable <";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, false);
                out << ">(";
                ft->args[i]->name->name = "arg_" + ft->args[i]->name->name;
                printFetchElementCompute(out, ft->args[i]);
                out << ")";
            }
            continue;
        }

        // Gather and Scatter arrays
        if (recursiveIsGather(t) || (t->isArray() && (t->getQualifiers() & TQ_Out) != 0))  //scatter out
        {
            out << whiteSpace(12) << "__thread_data[i]." << varname << " = arg_" << varname;
            
            if (IsScatterOutType(t))
            {
                out << ";" << newLine 
                    << whiteSpace(12) << "__thread_data[i]." << "indexof" << varname << " = __get_scatter__address(__BrtInt1((int)__abs_idx))";
                if(fDef->getShaderType() == ComputeShader)
                {
                    out << ";" << newLine 
                        << whiteSpace(12) << "__thread_data[i]." << "__instanceInGroup " << " = __get_threadId__InGroup(__BrtInt1((int)__abs_idx))";
                }
            }
            continue;
        }

        //! Print element
        out << whiteSpace(12) << "__thread_data[i]." << varname << " = arg_" << varname;
    }

    //! Shared array
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    out << ";" << newLine; 
                    out << whiteSpace(12) << "__thread_data[i]." << localDecl->name->name << " = " << localDecl->name->name;
                }
            }
        }
    }
    
    
    out << ";" << newLine;

    out << whiteSpace(8) << "}" << newLine << newLine;

    //! Initialize condition variable
    out << whiteSpace(8) << "//! Initialize condition variable" << newLine;
    out << whiteSpace(8) << kernelName << "_cond_var.init((uint)__group_size);" << newLine << newLine;

    //! Dispatch threads
    out << whiteSpace(8) << "//! Dispatch all threads in group" << newLine;
    out << whiteSpace(8) << "for(int i = 0; i < __group_size; ++i)" << newLine
        << whiteSpace(8) << "{" << newLine;
    out << whiteSpace(12) << kernelName << "_threads[i].create(" << kernelName << "_thread_entry_point, (void*)&"
        << "__thread_data[i]);" << newLine;
    out << whiteSpace(8) << "}" << newLine << newLine;

    //! Join threads
    out << whiteSpace(8) << "//! Wait for all threads to join " << newLine;
    out << whiteSpace(8) << "for(int i = 0; i < __group_size; ++i)" << newLine
        << whiteSpace(8) << "{" << newLine
        << whiteSpace(12) << kernelName << "_threads[i].join();" << newLine;
    out << whiteSpace(8) << "}" << newLine << newLine;

    //! Destroy condition variable
    out << whiteSpace(8) << "//! Destroy condition variable" << newLine;
    out << whiteSpace(8) << kernelName << "_cond_var.destroy();" << newLine << newLine;

    //! Delete threads and thread_data
    out << whiteSpace(8) << "delete[] __" << fDef->decl->name->name << "_threads;" << newLine;
    out << whiteSpace(8) << "delete[] __thread_data;" << newLine;

#if 0 //! Output never be ordinary stream
    for (i = 0; i < ft->nArgs; ++i)
    {

        if (ft->args[i]->isStream())
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Reduce | TQ_Out)) != 0)
            {
                if (globals.printLineDirectives)
                    fDef->location.printLineDirective(out, FuncRelLine);

                out << indent << indent << "*reinterpret_cast<";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, true);
                out << "*>(__out_" << ft->args[i]->name->name << ".address)" << " = __out_" << ft->args[i]->name->
                    name << ".castToArg(*reinterpret_cast<";
                static_cast < ArrayType * >(ft->args[i]->form)->subType->printType(out, NULL, true, 0, true);
                out << "*>(__out_" << ft->args[i]->name->name << ".address));" << std::endl;
            }
        }
    }
#endif //! Output never be ordinary stream

    out << indent<< "}" << newLine; 
    
    //! delete shared array 
    for (Statement *stmt = fDef->head; stmt; stmt = stmt->next)
    {
        if(stmt->isDeclaration())
        {
            DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
            for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
            {
                Decl *localDecl = (*k);
                Type *dclType = localDecl->form;
                if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                {
                    out << whiteSpace(4) << "delete[] __lds_data;" << newLine;
                    out << whiteSpace(4) << "delete __lds_array;" << newLine;
                }
            }
        }
    }

    out << "}" << newLine << std::endl;

}

