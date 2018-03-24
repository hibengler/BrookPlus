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
 * brtexpress.cpp -- 
 *  the actual code to convert gathers streams and indexof exprs
 */
#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

#include "janitorial.hpp"
#include "logger.hpp"

#include "brtexpress.h"
#include "main.h"
#include <stdio.h>

BrtGatherExpr::BrtGatherExpr(const IndexExpr * expr):Expression(ET_BrtGatherExpr, expr->location)
{
    LOG(LOG_FN_ENTRY, "BrtGatherExpr::BrtGatherExpr(%p)\n", expr);
    const IndexExpr *p;

    std::vector < Expression * >t;
    int i;

    t.push_back(expr->_subscript->dup0());

    // IAB:  Note that we have to reorder the arguments
    for (p = (IndexExpr *) expr->array; p && p->etype == ET_IndexExpr; p = (IndexExpr *) p->array)
        t.push_back(p->_subscript->dup0());

    for (i = static_cast < int >(t.size()) - 1; i >= 0; i--)
        dims.push_back(t[i]);

    base = (Variable *) p->dup0();

    assert(base->etype == ET_Variable);
    Variable *v = (Variable *) base;

    assert(v->name);
    assert(v->name->entry);
    assert(v->name->entry->uVarDecl);
    assert(v->name->entry->uVarDecl->form);
    assert(v->name->entry->uVarDecl->form->isArray());
    ArrayType *a = (ArrayType *) v->name->entry->uVarDecl->form;

    const ArrayType *pp;

    ndims = 1;
    for (pp = a; pp->subType && pp->subType->isArray(); pp = (ArrayType *) pp->subType)
        ndims++;

}

Expression *BrtGatherExpr::dup0() const
{
    BrtGatherExpr *ret = new BrtGatherExpr(location);
    for (int i = 0; i < static_cast<int>(dims.size()); i ++)
        ret->dims.push_back(dims[i]->dup());
    ret->location = location;
    ret->ndims = ndims;
    ret->type = type;
    ret->base = static_cast<Variable*>(base->dup());
    return ret;

}

void BrtGatherExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtGatherExpr::print(%p)\n", &out);
    #define AsBinaryExpr(e) static_cast<BinaryExpr*>(e)

    BaseType *b = getBaseType();

    if (ndims >= 4 || ndims <=0)
    {
        std::cerr << location << "GPU runtimes can't handle gathers greater than 4D.\n";
    }
    
    if(globals.enableGPUAddressTranslation == false || ndims == 2)
    {
        out << "__gather_";
        b->printBase(out, 0);
        out << "(";
        base->print(out);
        out << ", float2(";
        assert (dims.size() == 1);
        dims[0]->print(out);
        out << "))";
    }
    else
    {
        out << "__gather_";
        b->printBase(out, 0);
        out << "(";
        base->print(out);
        out << ", ";
        assert (dims.size() == 1);

        if(ndims == 1)
        {
            out << "__getGatherPosFrom1D(";
            dims[0]->print(out);
            out << ", __gatherStreamShape_";
            base->print(out);
            out << ", __gatherBufferShape_";
            base->print(out);
            out << ")";
        }
        else if(ndims == 3)
        {
            out << "__getGatherPosFrom3D(";
            dims[0]->print(out);
            out << ", __gatherStreamShape_";
            base->print(out);
            out << ", __gatherBufferShape_";
            base->print(out);
            out << ")";
            
        }
        else
        {
            assert(0);
        }
        out << ")";
    }
#if 0
    out << "__gather_";
    b->printBase(out, 0);
    out << "(";
    base->print(out);

    out << ",__gatherindex";
    out << ndims << "((";

    //dims.size() is always 1, the dimension info is in ndims
    assert (dims.size() == 1);
    dims[0]->print(out);

    out << ")";

    //amdhlsl can't handle 
    // (a.yx).xy 
    // (a-1).x
    //so avoid outputing such redundant swizzle
    bool skipswizzle = false;
    if (dims[0]->etype == ET_Variable)
    {
        if (dims[0]->getType() == NULL)
        {
            skipswizzle = true;
        }
        else
        {
            BaseType *etype = dims[0]->getType()->getBase();
            BaseTypeInfo *info = getBaseTypeInfo(etype->typemask);
            if (info == NULL || info->component == 1)
                skipswizzle = true;
        }
    } 
    else
    {
        skipswizzle = true;
    }

    if (skipswizzle == false)
    {
        if (ndims == 1)
        {
            out << ".x";
        }
        else if (ndims == 2)
        {
            out << ".xy";
        }
        else if (ndims == 3)
        {
            out << ".xyz";
        }
        else if (ndims == 4)
        {
            out << ".xyzw";
        }
    }

    if (globals.enableGPUAddressTranslation)
    {
        out << ", __gatherdomainmin_";
        base->print(out);
        out << ", __gatherlinearize_";
        base->print(out);
        out << ", __gathertexshape_";
        base->print(out);
        out << "))";
    }
    else
    {
        //remove supported constants for gather array
        
        out << ", _const_";
        base->print(out);
        out << "_scalebias))";
    }
#endif
}

SymEntry *find_parameter_in_current_kernel_by_name(std::string const &pname);

#if BROOK_TO_IL_CODE_GEN_PATH
extern void
printILCalculatetexpos(std::ostream & shader, bril::store * texpos,
                       const std::string & index, const std::string & domainmin,
                       const std::string & linearize, const std::string & textureshape);

void BrtGatherExpr::printILGatherIndex(std::ostream & outs, bril::store * dst) const
{
    LOG(LOG_FN_ENTRY, " BrtGatherExpr::printILGatherIndex(%p, %p)\n", &outs, dst);
    if (globals.enableGPUAddressTranslation)
    {
        /* this is the hlsl code
           float2 __gatherindex4( float4 index, float4 domainConst, 
           float4 linearizeConst, float4 reshapeConst ) {
           return __calculatetexpos( floor(index+domainConst), 0, linearizeConst, reshapeConst ); }
         */
        bril::store * domain = find_parameter_in_current_kernel_by_name
            ("__gatherdomainmin_" + base->name->name)->get_il_store();
        bril::store * linearize = find_parameter_in_current_kernel_by_name
            ("__gatherlinearize_" + base->name->name)->get_il_store();
        bril::store * texshape = find_parameter_in_current_kernel_by_name
            ("__gathertexshape_" + base->name->name)->get_il_store();

        bril::store tmp1;
        bril::allocate_store(&tmp1, 4);

        //float4(floor(index + domain), 0, ...
        bril::emitOp(outs, "add", &tmp1, dims[0]->get_il_store(), domain);
        std::string srcswizzle;
        switch (ndims)
        {
        case 1:
            srcswizzle = ".x000";
            break;
        case 2:
            srcswizzle = ".xy00";
            break;
        case 3:
            srcswizzle = ".xyz0";
            break;
        default:
            srcswizzle = ".xyzw";
            break;
        }
        outs << IL_PREFIX_STRING << "flr "
            << get_gpr_name(&tmp1, bril::rs_dst_sel)
            << ", " << get_gpr_name(&tmp1, bril::rs_base) << srcswizzle << "\n";

        bril::store zero;
        bril::allocate_store(&zero, 4);
        outs << IL_PREFIX_STRING << "mov " << get_gpr_name(&zero, bril::rs_dst_sel) << ", r1.0000\n";

        printILCalculatetexpos(outs, dst,
                               bril::get_gpr_name(&tmp1, bril::rs_base),
                               bril::get_gpr_name(&zero, bril::rs_base),
                               bril::get_gpr_name(linearize, bril::rs_base),
                               bril::get_gpr_name(texshape, bril::rs_base));
    }
    else
    {
#if 0
        outs << IL_PREFIX_STRING << "mov "
            << get_gpr_name(dst, bril::rs_dst_sel) << ", "
            << dims[0]->getILRegName(bril::rs_src_sel) << "\n";
#else
        SymEntry *sb = find_parameter_in_current_kernel_by_name("_const_" + base->name->name + "_scalebias");
        outs << IL_PREFIX_STRING << "add "
            << get_gpr_name(dst, bril::rs_dst_sel) << ", "
            << dims[0]->getILRegName(bril::rs_src_sel) << ", " << sb->getILRegName(bril::rs_base);

        //print swizzling
        if (ndims == 1)
        {
            outs << ".z\n";     //this swizzling match that in the HLSL __gatherindex1
        }
        else if (ndims == 2)
        {
            outs << ".zw\n";    //this swizzling match that in the HLSL __gatherindex1
        }
        else
        {
            outs << "\n";       //for ndims = 3 or 4, not swizzling is needed
        }
#endif
    }
}

void BrtGatherExpr::printILGatherFloat(std::ostream & outs, bril::store * dst, bril::store * indexVar, u32 sampler) const
{
    LOG(LOG_FN_ENTRY, " BrtGatherExpr::printILGatherFloat(%p, %p, %p, %d)\n", &outs, dst, indexVar, sampler);
    outs << IL_PREFIX_STRING << "sample_l_resource(" << sampler << ")_sampler(" << sampler << ") "
        << get_gpr_name(dst, bril::rs_dst_sel);

    //print first source
    outs << ", " << get_gpr_name(indexVar, bril::rs_base);

    if (ndims <= 2)
    {
        outs << ".xy00";        //set field y=0 to achieve 1d
    }
    else
    {
        outs << ".xyz0";
    }

    //print second source, which should be 0, for gradient value
    outs << ", " << get_gpr_name(indexVar, bril::rs_base) << ".0000\n";
}

void BrtGatherExpr::printIL(std::ostream & out, bril::store * r, int *sampler_offset) const
{
    LOG(LOG_FN_ENTRY, " BrtGatherExpr::printIL(%p, %p, %p)\n", &out, r, sampler_offset);
    if (is_struct(r))
    {
        for (u32 i = 0; i < r->elts.size(); ++i)
            printIL(out, &r->elts[i], sampler_offset);
    }
    else
    {
        Variable *var = (Variable *) base;
        u32 sampler = bril::getTextureId(var->name->name) + *sampler_offset;

        ++*sampler_offset;

        bril::store indexVar;
        bril::allocate_store(&indexVar, 4);
        printILGatherIndex(out, &indexVar);
        printILGatherFloat(out, r, &indexVar, sampler);
    }
}
void BrtGatherExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtGatherExpr::print_il(%p)\n", &out);
    // Force allocation.
    getILRegName(bril::rs_base);

    int sampler_offset = 0;

    dims[0]->print_il(out);
    printIL(out, &il_store, &sampler_offset);
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

BrtStreamInitializer::BrtStreamInitializer(const BrtStreamType * type,
                                           const Location loc):Expression(ET_BrtStreamInitializer, loc), l(loc)
{
    LOG(LOG_FN_ENTRY, "BrtStreamInitializer::BrtStreamInitializer(%p, %p)\n", type, &loc);
    t = (BrtStreamType *) type->dup0();
}

BrtStreamInitializer::~BrtStreamInitializer()
{
    LOG(LOG_FN_ENTRY, "BrtStreamInitializer::~BrtStreamInitializer()\n");
    // Handled by global type list
    //   delete t;
}

Expression *BrtStreamInitializer::dup0() const
{
    LOG(LOG_FN_ENTRY, "BrtStreamInitializer::dup0()\n");
    return new BrtStreamInitializer(t, l);
}

void BrtStreamInitializer::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtStreamInitializer::print(%p)\n", &out);
    t->printInitializer(out);
}


BrtIndexofExpr::BrtIndexofExpr(Variable * operand, const Location l):Expression(ET_BrtIndexofExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtIndexofExpr::BrtIndexofExpr(%p, %p)\n", operand, &l);
    expr = operand;
    type = new BaseType(BT_Float4);
}


BrtIndexofExpr::~BrtIndexofExpr()
{
    LOG(LOG_FN_ENTRY, "BrtIndexofExpr::~BrtIndexofExpr()\n");
    // delete sizeofType;
    delete expr;
}


Expression *BrtIndexofExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, " *BrtIndexofExpr::dup0()\n");
    BrtIndexofExpr *ret;

    ret = new BrtIndexofExpr(static_cast < Variable * >(expr->dup()), location);
    ret->type = type;
    return ret;
}

// Ian:  I'm so sorry about this...
extern bool horrible_horrible_indexof_hack;


#define isScatterOut(var) \
    (var->name && var->name->entry && var->name->entry->uVarDecl \
     && var->name->entry->uVarDecl->form->isArray() \
     && (var->name->entry->uVarDecl->form->getQualifiers() & TQ_Out) != 0)

void BrtIndexofExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtIndexofExpr::print(%p)\n", &out);

    // Ian:  I'm so sorry about this...
    if (horrible_horrible_indexof_hack)
    {
        std::string bak = expr->name->name;
        out << "indexof";
        if (isScatterOut(expr) == false)
            out <<"(";
        expr->print(out);
        if (isScatterOut(expr) == false)
            out << ")";
    }
    else
    {
        std::string bak = expr->name->name;
        
        //! Add __indexof_ to variable name only for code generation path
        if(globals.enableSematicCheck == false)
        {
            expr->name->name = "__indexof_" + bak;
        }
        else
        {
            expr->name->name = "indexof(" + bak + ")";
        }
        expr->print(out);
        expr->name->name = bak;
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


#if BROOK_TO_IL_CODE_GEN_PATH
void BrtIndexofExpr::print_il(std::ostream & UNUSED(out)) const
{
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
BrtIndexofExpr::semanticCheck(bool isKernel)
{
    if(isKernel)
    {
        if(expr->name == NULL)
        {
            assert(0);
        }
        else if(expr->name->entry == NULL || expr->name->entry->uVarDecl == NULL)
        {
            //undefined variable
            RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "variable not defined");
        }
        else
        {
            Decl *dcl = expr->name->entry->uVarDecl;
            if(dcl->form->isArray() || dcl->form->isStream())
            {
                if(IsScatterOutType(dcl->form))
                {
                    RecordSemanticWarn(this, SemanticChecker::SCWStream,
                                       "Strongly recommended to use instance() operator on scatter streams");
                }
                else if(IsScatterOutType(dcl->form) == false && dcl->form->isArray())
                {
                    RecordSemanticError(this, SemanticChecker::SCEKExprOther,
                                        "indexof() operator is not allowed on gather array/local array");
                }
			}
            else
            {
                RecordSemanticError(this, SemanticChecker::SCEKExprOther, 
                                    "indexof()operator allowed only on stream/scatter");
            }
            
        }
    }
    else
    {
        //! Indexof operator has no meaning inside Non-kernel function
        RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "Indexof()operator has no meaning inside Non-kerenl functions");
    }
}
#endif

void BrtIndexofExpr::findExpr(fnExprCallback cb)
{
    (cb) (this);
    if (expr != NULL)
    {
        //Expression* e=(cb)(expr);
        //assert (e->etype==ET_Variable);
        //expr = (Variable *) e;
        expr->findExpr(cb);
    }
}

BrtInstanceExpr::BrtInstanceExpr(const Location l):Expression(ET_BrtInstanceExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtInstanceExpr::BrtInstanceExpr(%p)\n", &l);
    expr = new Variable(NULL, l);
    type = new BaseType(BT_Int4);
}


BrtInstanceExpr::BrtInstanceExpr(Variable * operand, const Location l):Expression(ET_BrtInstanceExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtInstanceExpr::BrtInstanceExpr(%p, %p)\n", operand, &l);
    expr = operand;
    type = new BaseType(BT_Int4);
}


BrtInstanceExpr::~BrtInstanceExpr()
{
    LOG(LOG_FN_ENTRY, "BrtInstanceExpr::~BrtInstanceExpr()\n");
    // delete sizeofType;
    delete expr;
}


Expression *BrtInstanceExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, " *BrtInstanceExpr::dup0()\n");
    BrtInstanceExpr *ret;

    ret = new BrtInstanceExpr(static_cast < Variable * >(expr->dup()), location);
    ret->type = type;
    return ret;
}

void BrtInstanceExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtInstanceExpr::print(%p)\n", &out);

    if (horrible_horrible_indexof_hack)
    {
        std::string bak = expr->name->name;
        out << "indexof";
        if (isScatterOut(expr) == false)
            out <<"(";
        expr->print(out);
        if (isScatterOut(expr) == false)
            out << ")";
    }
    else
    {
        if(expr->name)
        {
            std::string bak = expr->name->name;

            //! Add __indexof_ to variable name only for code generation path
            if(globals.enableSematicCheck == false)
            {
                expr->name->name = "__indexof_" + bak;
            }
            else
            {
                expr->name->name = "instance()";
            }
            expr->print(out);
            expr->name->name = bak;
        }
        else if(globals.enableSematicCheck)
        {
            out << "instance()";
        }
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


#ifdef SEMANTIC_CHECK
void 
BrtInstanceExpr::semanticCheck(bool isKernel)
{
    if(!isKernel)
    {
        //! Indexof operator has no meaning inside Non-kernel function
        RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "Instance()operator has no meaning inside Non-kerenl functions");
    }
}
#endif

void BrtInstanceExpr::findExpr(fnExprCallback cb)
{
    (cb) (this);
    if (expr != NULL)
    {
        //Expression* e=(cb)(expr);
        //assert (e->etype==ET_Variable);
        //expr = (Variable *) e;
        expr->findExpr(cb);
    }
}



BrtInstanceInGroupExpr::BrtInstanceInGroupExpr(const Location l):Expression(ET_BrtInstanceInGroupExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtInstanceInGroupExpr::BrtInstanceInGroupExpr(%p)\n", &l);
    expr = new Variable(NULL, l);
    type = new BaseType(BT_Int4);
}


BrtInstanceInGroupExpr::BrtInstanceInGroupExpr(Variable * operand, const Location l):Expression(ET_BrtInstanceInGroupExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtInstanceInGroupExpr::BrtInstanceInGroupExpr(%p, %p)\n", operand, &l);
    expr = operand;
    type = new BaseType(BT_Int4);
}


BrtInstanceInGroupExpr::~BrtInstanceInGroupExpr()
{
    LOG(LOG_FN_ENTRY, "BrtInstanceInGroupExpr::~BrtInstanceInGroupExpr()\n");
    // delete sizeofType;
    delete expr;
}


Expression *BrtInstanceInGroupExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, " *BrtInstanceInGroupExpr::dup0()\n");
    BrtInstanceInGroupExpr *ret;

    ret = new BrtInstanceInGroupExpr(static_cast < Variable * >(expr->dup()), location);
    ret->type = type;
    return ret;
}

void BrtInstanceInGroupExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtInstanceInGroupExpr::print(%p)\n", &out);

    if (horrible_horrible_indexof_hack)
    {
        //std::string bak = expr->name->name;
        out << "__instanceInGroup";
        //if (isScatterOut(expr) == false)
        //    out <<"(";
        //expr->print(out);
        //if (isScatterOut(expr) == false)
        //    out << ")";
    }
    else
    {
        if(expr->name)
        {
            std::string bak = expr->name->name;

            //! Add __indexof_ to variable name only for code generation path
            if(globals.enableSematicCheck == false)
            {
                expr->name->name = "__indexof_" + bak;
            }
            else
            {
                expr->name->name = "instanceInGroup()";
            }
            expr->print(out);
            expr->name->name = bak;
        }
        else if(globals.enableSematicCheck)
        {
            out << "instanceInGroup()";
        }
        else
        {
            out << "__instanceInGroup";
        }
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


#ifdef SEMANTIC_CHECK
void 
BrtInstanceInGroupExpr::semanticCheck(bool isKernel)
{
    if(!isKernel)
    {
        //! instanceInGroup() operator has no meaning inside Non-kernel function
        RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "instanceInGroup()operator has no meaning inside Non-kerenl functions");
    }
}
#endif

void BrtInstanceInGroupExpr::findExpr(fnExprCallback cb)
{
    (cb) (this);
    if (expr != NULL)
    {
        //Expression* e=(cb)(expr);
        //assert (e->etype==ET_Variable);
        //expr = (Variable *) e;
        expr->findExpr(cb);
    }
}


BrtSyncGroupExpr::BrtSyncGroupExpr(const Location l):Expression(ET_BrtSyncGroupExpr, l)
{
    LOG(LOG_FN_ENTRY, "BrtSyncGroupExpr::BrtInstanceInGroupExpr(%p)\n", &l);
    type = new BaseType(BT_Void);
}

BrtSyncGroupExpr::~BrtSyncGroupExpr()
{
    LOG(LOG_FN_ENTRY, "BrtSyncGroupExpr::~BrtInstanceInGroupExpr()\n");
}


Expression *BrtSyncGroupExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, " *BrtInstanceInGroupExpr::dup0()\n");
    BrtSyncGroupExpr *ret;

    ret = new BrtSyncGroupExpr(location);
    ret->type = type;
    return ret;
}
std::string getCurrentKernelName();
void BrtSyncGroupExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtSyncGroupExpr::print(%p)\n", &out);

    if (horrible_horrible_indexof_hack)
    {
        out << "__" << getCurrentKernelName() << "_cond_var.syncThreads()";
    }
    else
    {
        if(globals.enableSematicCheck)
        {
            out << "syncGroup()";
        }
        else
        {
            out << "SyncThreads()";
        }
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


#ifdef SEMANTIC_CHECK
void 
BrtSyncGroupExpr::semanticCheck(bool isKernel)
{
    if(!isKernel)
    {
        //! instanceInGroup() operator has no meaning inside Non-kernel function
        RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "syncGroup()operator has no meaning inside Non-kerenl functions");
    }
}
#endif

void BrtSyncGroupExpr::findExpr(fnExprCallback cb)
{
    (cb) (this);
}
