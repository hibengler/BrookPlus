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
 * brtdecl.cpp --
 * Stream types and gather types are actually printed and dealt with here.
 */

#include <cassert>
#include <cstring>
#include <iostream>


#include "janitorial.hpp"
#include "decl.h"
#include "express.h"
#include "stemnt.h"
#include "token.h"
#include "gram.h"
#include "project.h"
#include "brtdecl.h"
#include "main.h"


BrtStreamType::BrtStreamType(const ArrayType * t):Type(TT_BrtStream)
{
    LOG(LOG_FN_ENTRY, "BrtStreamType::BrtStreamType(%p)\n", t);
    const ArrayType *p;

    // First find the base type of the array;
    for (p = t; p->subType && p->subType->isArray(); p = (ArrayType *) p->subType)
    {

        //assert(p->size);
        if (p->size)
            dims.push_back(p->size->dup0());
    }

    /*
     * p->size is NULL when parsing s<> (i.e. a stream function argument or
     * some other case where no length information is given).  This is fine in
     * some cases, but not in others, so the super class tolerates it and lets
     * the children individually decide if that's okay for them.
     */

    if (p->size)
    {
        dims.push_back(p->size->dup0());
    }

    assert(p->subType);
    assert(p->subType->isBaseType());

    base = (BaseType *) p->subType->dup0();

    isIterator = (t->getQualifiers() & TQ_Iter) != 0;
}


BrtStreamType::BrtStreamType(const BaseType * _base, const ExprVector _dims):Type(TT_BrtStream)
{
    LOG(LOG_FN_ENTRY, "BrtStreamType(%p, %p)\n", _base, &_dims);
    isIterator = false;

    ExprVector::const_iterator i;

    base = (BaseType *) _base->dup();
    for (i = _dims.begin(); i != _dims.end(); i++)
    {
        dims.push_back((*i)->dup());
    }
}


BrtStreamType::~BrtStreamType()
{
    LOG(LOG_FN_ENTRY, "BrtStreamType::~BrtStreamType()\n");
    // Handled by deleting the global type list
    // delete subType;
    for (unsigned int i = 0; i < dims.size(); i++)
        delete dims[i];
}


void BrtStreamType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "BrtStreamType::findExpr(%p)\n", cb);
    if (base)
        base->findExpr(cb);

    for (unsigned int i = 0; i < dims.size(); i++)
    {
        dims[i] = (cb) (dims[i]);
        dims[i]->findExpr(cb);
    }
}


void BrtStreamParamType::printType(std::ostream & out, Symbol * name,
                                   bool showBase, int level, bool raw) const
{
    LOG(LOG_FN_ENTRY, "BrtStreamParamType(%p, %p, %d, %d, %d)\n", &out, name, showBase, level, raw);	
    if (isIterator)
        out << "::brt::iter ";
    else
        out << "::brook::Stream ";

    if (name)
        out << *name;
}

void BrtStreamParamType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "BrtStreamParamType::printBase(%p, %d)\n", &out, level);
    if (isIterator)
        out << "::brt::iter ";
    else
        out << "::brook::Stream ";
}

void BrtStreamParamType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtStreamParamType::printForm(%p)\n", &out);
    out << "-BrtStreamParam Type ";
    if (base)
        base->printBase(out, 0);
}

void BrtStreamParamType::printInitializer(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtStreamParamType::printInitializer(%p)\n", &out);
    /* Nothing.  There are no initializers for parameters to functions */
}


void BrtInitializedStreamType::printType(std::ostream & out, Symbol * name,
                                         bool showBase, int level, bool raw) const
{
    LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::printType(%p, %p, %d, %d, %d)\n",
        &out, name, showBase, level, raw);

    //declare dimension array of stream
    out << "unsigned int " << *name << "Dim[] = {";

    std::ostringstream declareDim;

    // set dimension of stream
    for (unsigned int i = 0; i < dims.size(); i++)
    {
        dims[i]->print(declareDim);
    }

    std::vector<std::string> tokens;
    tokenize(declareDim.str(), tokens, ",");

    for(unsigned int i = 0; i < tokens.size() - 1; ++i)
    {
        out << tokens[tokens.size() - i -1];
        out << ",";
    }

    out << tokens[0];

    out << "};" << std::endl;
    

    // stream type
    if (showBase)
    {
        indent(out, level);
        out << "::brook::Stream< ";
        base->printType(out, NULL, true, 0, raw);
        out << "> ";
    }

    // stream name with constructor
    if (name)
        out << *name;

    // set stream constructor parameters
    out << "(sizeof(" << *name << "Dim) / sizeof(unsigned int), " << *name << "Dim" << ")";
    
}

void BrtInitializedStreamType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::printForm(%p)\n", &out);
    out << "-BrtInitializedStream Type ";
    if (base)
        base->printBase(out, 0);
}

void BrtInitializedStreamType::printInitializer(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::printInitializer(%p)\n", &out);
    /*
     * I think this is bogus.  I'm pretty sure BrtInitializedStreams don't
     * have initializers per se, instead they use constructor syntax. --Jeremy.
     */
    out << "__BRTCreateStream(\"";
    base->printBase(out, 0);
    out << "\",";
    for (unsigned int i = 0; i < dims.size(); i++)
    {
        dims[i]->print(out);
        out << ",";
    }
    out << "-1)";
}


BrtIterType::BrtIterType(const ArrayType * stream, const FunctionCall * f):BrtStreamType(stream)
{
    LOG(LOG_FN_ENTRY, "BrtIterType::BrtIterType(%p, %p)\n", stream, f);
    ExprVector::const_iterator i;

    assert(f->function->etype == ET_Variable);
    assert(strcmp(((Variable *) f->function)->name->name.c_str(), "iter") == 0);

    /*
     * Now fish the min / max out of 'f'.
     *
     * We impose the following constraints on dimensions
     *   - We support float, float2, float3, or float4 1-D streams
     *   - We float2 2-D streams
     */
    assert(f->args.size() == 2);

    for (int j = 0; j < 2; j++)
    {
      UNUSED(Type *expType) = f->args[j]->getType();

        //assert(expType && expType->getBase()->typemask == base->typemask);
        //possible typemask: int, float
    }

    assert(dims.size() == 1 || (dims.size() == 2 && base->typemask == BT_Float2));
    for (i = f->args.begin(); i != f->args.end(); i++)
    {
        args.push_back((*i)->dup0());
    }
}


BrtIterType::BrtIterType(const BaseType * _base, const ExprVector _dims,
                         const ExprVector _args):BrtStreamType(_base, _dims)
{
    LOG(LOG_FN_ENTRY, "BrtIterType::BrtIterType(%p, %p, %p)\n", _base, &_dims, &_args);
    ExprVector::const_iterator i;

    for (i = _args.begin(); i != _args.end(); i++)
    {
        args.push_back((*i)->dup());
    }
}


BrtIterType::~BrtIterType()
{
    LOG(LOG_FN_ENTRY, "BrtIterType::~BrtIterType()\n");
    ExprVector::iterator i;

    for (i = args.begin(); i != args.end(); i++)
    {
        delete *i;
    }
}


void BrtIterType::printType(std::ostream & out, Symbol * name,
                            bool showBase, int level, bool raw) const
{
    LOG(LOG_FN_ENTRY, "BrtIterType::printType(%p, %p, %d, %d, %d)\n", &out, name, showBase, level, raw);

    ExprVector::const_iterator i;

    if (showBase)
        out << "::brt::iter ";

    if (name)
        out << *name;

    // TIM: add initializer as constructor
    out << "(::brt::";

    if (base->getBase()->typemask & BT_Float4)
    {
        out << "__BRTFLOAT4, ";
    }
    else if (base->getBase()->typemask & BT_Float3)
    {
        out << "__BRTFLOAT3, ";
    }
    else if (base->getBase()->typemask & BT_Float2)
    {
        out << "__BRTFLOAT2, ";
    }
    else if (base->getBase()->typemask & BT_Float)
    {
        out << "__BRTFLOAT, ";
    }
    else
    {
        std::cerr << "Warning: Unsupported iterator type ";
        base->printBase(std::cerr, 0);
        std::cerr << std::endl;
        out << "__BRTFLOAT, ";
    }

    /* Now print the dimensions */
    for (i = dims.begin(); i != dims.end(); i++)
    {
        (*i)->print(out);
        out << ",";
    }
    out << " -1, ";

    /* Now print the min / max */
    for (i = args.begin(); i != args.end(); i++)
    {
        Type *expType = (*i)->getType();

        if (expType == NULL)
        {
            //iter(0.0, LENGTH), preprocess is not called to replace LENGTH ...
            out << "(float)";
            (*i)->print(out);
            out << ", ";
        }
        else
        {
            BaseType *baseType = expType->getBase();
            BaseTypeInfo *typeInfo = getBaseTypeInfo(baseType->typemask);

            if (baseType->typemask != BT_Float)
                out << "(float)";
            if (typeInfo->component < 2)
            {
                (*i)->print(out);
                out << ", ";
            }
            else
            {
                static const char xyzw[] = { 'x', 'y', 'z', 'w' };
                for (int j = 0; j < typeInfo->component; j++)
                {
                    out << '(';
                    (*i)->print(out);
                    out << ")." << xyzw[j] << ", ";
                }
            }
        }
    }
    out << "-1)";
}


void BrtIterType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtIterType::printForm(%p)\n", &out);
    out << "-BrtIter Type ";
    if (base)
        base->printBase(out, 0);
}


void BrtIterType::printInitializer(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BrtIterType::printInitializer(%p)\n", &out);
    out << "__BRTCreateIter(\"";
    base->printBase(out, 0);
    out << "\",";
    for (unsigned int i = 0; i < dims.size(); i++)
    {
        dims[i]->print(out);
        out << ",";
    }
    out << "-1)";
}


void BrtIterType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, " BrtIterType::findExpr(%p)\n", cb);
    ExprVector::iterator i;

    BrtStreamType::findExpr(cb);

    for (i = args.begin(); i != args.end(); i++)
    {
        *i = (cb) (*i);
        (*i)->findExpr(cb);
    }
}


CPUGatherType::CPUGatherType(ArrayType & t, bool copy_on_write)
{
    LOG(LOG_FN_ENTRY, "CPUGatherType::CPUGatherType(%p, %d)\n", &t, copy_on_write);
    dimension = 0;
    raw = false;
    at = &t;
    this->copy_on_write = copy_on_write;
    subtype = at;
    while (subtype->type == TT_Array)
    {
        dimension += 1;
        subtype = static_cast < const ArrayType *>(subtype)->subType;
    }
}

void CPUGatherType::printSubtype(std::ostream & out, Symbol * name, bool showBase, int level) const
{
    LOG(LOG_FN_ENTRY, " CPUGatherType::printSubtype(%p, %p, %d, %d)\n", &out, name, showBase, level);
    subtype->printType(out, name, showBase, level, this->raw);
}

void CPUGatherType::printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw) const
{
    LOG(LOG_FN_ENTRY, " CPUGatherType::printType(%p, %p, %d, %d, %d)\n", &out, name, showBase, level, raw);
    this->raw = raw;
    printBefore(out, name, level);
    printAfter(out);
}

void CPUGatherType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, " CPUGatherType::printBefore(%p, %p, %d)\n", &out, name, level);
    if (!copy_on_write)
    {
        Symbol nothing;

        nothing.name = "";
        
        out << "__BrtArray<";
        //subtype->printType(out, &nothing, true, level,
        //                   (subtype->getBase()->
        //                    typemask & (BT_ShortFixed | BT_Fixed | BT_Fixed2 | BT_ShortFixed2 | BT_ShortFixed3 |
        //                                BT_Fixed3 | BT_ShortFixed4 | BT_Fixed4)) ? true : this->raw);

        subtype->printType(out, &nothing, true, level, this->raw);


    }
    else
    {
        out << "Array" << dimension << "d<";
        
        at->printBase(out, level);

        const Type *t = at;

        for (unsigned int i = 0; i < dimension && i < 3; i++)
        {
            if (i != 0)
                out << "         ";
            out << ", ";
            const ArrayType *a = static_cast < const ArrayType * >(t);

            a->size->print(out);
            t = a->subType;
        }
    }
    
    out << "> ";
    
    if (name)
        out << *name;
}

void CPUGatherType::printAfter(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " CPUGatherType::printAfter(%p)\n", &out);
    //nothing happens
    //you fail to obtain anything
    //...
}

BrtKernelType::BrtKernelType(FunctionType * functionType):_functionType(functionType)
{
    LOG(LOG_FN_ENTRY, "BrtKernelType::BrtKernelType(%p)\n", functionType);
    unsigned int i;

    extend((*_functionType->getSubType())->dup0());
    for (i = 0; i < _functionType->nArgs; i++)
        addArg(_functionType->args[i]->dup0());

    for (i = 0; i < nArgs; i++)
    {
        TypeQual q = args[i]->form->getQualifiers();

        if ((q & TQ_Out) == 0)
        {
            BaseType *b = args[i]->form->getBase();

            b->qualifier |= TQ_Const;
        }
    }
}

BrtKernelType::~BrtKernelType()
{
    LOG(LOG_FN_ENTRY, "BrtKernelType::~BrtKernelType()\n");
}
