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

#include "logger.hpp"
#include "brtvout.h"

#include "ctool.h"

/*
const double SENTINEL_VALUE = -18446321861244485632.0;
std::string DoubleToString(double a) {
   char str[1024];
   sprintf(str,"%lf",a);
   return str;
}
std::string SENTINEL_STRING= DoubleToString(SENTINEL_VALUE);
*/
VoutFunctionType voutFunctions;

FunctionDef *IdentifyVoutFunc(FunctionDef * fd)
{
    LOG(LOG_FN_ENTRY, "IdentifyVoutFunc(%p)\n", fd);
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);
    bool foundvout = false;
    std::set < unsigned int >vouts;

    for (unsigned int i = 0; i < ft->nArgs; ++i)
    {
        if ((ft->args[i]->form->getQualifiers() & TQ_Vout) != 0)
        {
            foundvout = true;
            vouts.insert(i);
        }
    }
    if (foundvout)
        voutFunctions.insert(VoutFunctionType::value_type(fd->FunctionName()->name, vouts));

    return NULL;
}

static Decl *createParam(const char *varname, Type * vartype)
{
    LOG(LOG_FN_ENTRY, "createParam(%s, %p)\n", varname, vartype);
    Symbol *sym = new Symbol;

    sym->name = varname;
    Decl *dcl = new Decl(sym);

    dcl->form = vartype;
    sym->entry = mk_paramdecl(sym->name, dcl);

    return dcl;
}

FunctionDef *TransformHeader(FunctionDef * fd)
{
    LOG(LOG_FN_ENTRY, "TransformHeader(%p)\n", fd);
    VoutFunctionType::iterator func = voutFunctions.find(fd->FunctionName()->name);
    if (func == voutFunctions.end())
        return NULL;
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);
    std::set < unsigned int >::iterator iter = func->second.begin();

    for (; iter != func->second.end(); ++iter)
    {
        DeclStemnt *ds = new DeclStemnt(fd->location);
        Decl *tmpVout = ft->args[*iter];

        ft->args[*iter] = ft->args[*iter]->dup();
        ft->args[*iter]->name = new Symbol();
        ft->args[*iter]->name->name = "__" + tmpVout->name->name + "_stream";
        Decl *streamDecl = new Decl(ft->args[*iter]->name);

        ft->args[*iter]->name->entry = mk_paramdecl(ft->args[*iter]->name->name, streamDecl);
        streamDecl->form = tmpVout->form;

        if (tmpVout->form->type == TT_Stream)
        {
            tmpVout->form = static_cast < ArrayType * >(tmpVout->form)->subType;
        }

        tmpVout->next = NULL;
        ds->addDecl(tmpVout);
        ds->next = fd->head;
        fd->head = ds;
    }

    Decl *voutCounter = createParam("__vout_counter", new BaseType(BT_Float));

    ft->addArg(voutCounter);

    Type *infType;

#ifdef INF_SENTINEL
    ArrayType *streamtype = new ArrayType(TT_Array, NULL);

    streamtype->extend(new BaseType(BT_Float));
    infType = streamtype;
#else
    infType = new BaseType(BT_Float);
#endif

    Decl *infDecl = createParam("__inf", infType);

    ft->addArg(infDecl);

    return NULL;
}

FunctionDef *TransformVoutToOut(FunctionDef * fd)
{
    LOG(LOG_FN_ENTRY, "TransformVoutToOut(%p)\n", fd);
    VoutFunctionType::iterator func = voutFunctions.find(fd->FunctionName()->name);
    if (func == voutFunctions.end())
        return NULL;
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);
    std::set < unsigned int >::iterator iter = func->second.begin();

    for (; iter != func->second.end(); ++iter)
    {
        BaseType *bat = ft->args[*iter]->form->getBase();

        bat->qualifier &= (~TQ_Vout);
        bat->qualifier |= TQ_Out;
    }
    return NULL;
}

inline Symbol *findSentinel(FunctionType * ft)
{
    LOG(LOG_FN_ENTRY, "findSentinel(%p)\n", ft);
    Decl *dcl = ft->findArg("__inf");

    if (dcl)
        return dcl->name;

    return NULL;
}

static Decl *createVar(const char *varname, Type * vartype)
{
    LOG(LOG_FN_ENTRY, "createVar(%s, %p)\n", varname, vartype);
    Symbol *sym = new Symbol;

    sym->name = varname;
    Decl *dcl = new Decl(sym);

    dcl->form = vartype;
    sym->entry = mk_vardecl(sym->name, dcl);

    return dcl;
}

Decl *findVoutCounter(FunctionType * ft, bool replacement = true)
{
    LOG(LOG_FN_ENTRY, "findVoutCounter(%p, %d)\n", ft, replacement);

    if (replacement)
    {
        static std::map < FunctionType *, Decl * >replacements;

        std::map < FunctionType *, Decl * >::iterator i = replacements.find(ft);
        if (i != replacements.end())
        {
            return (*i).second;
        }
        else
        {
            Decl *dcl = createVar("__vout_counter_rw", new BaseType(BT_Float));

            replacements[ft] = dcl;
            return dcl;
        }
    }

    return ft->findArg("__vout_counter");
}

DeclStemnt *MakeVoutCounter(std::string fname, FunctionType * ft, const Location & location)
{
    LOG(LOG_FN_ENTRY, "MakeVoutCounter(%s, %p, %p)\n", fname.c_str(), ft, &location);
    //return new ExpressionStemnt (new FloatConstant(31337,location),location);
    //DeclStemnt * ds = new DeclStemnt (location);
    Decl *VoutCounter = findVoutCounter(ft, true);

    VoutCounter->form = new BaseType(BT_Float);
    VoutCounter->initializer = new Variable(findVoutCounter(ft, false)->name->dup(), location);

    DeclStemnt *ds = new DeclStemnt(location);
    ds->addDecl(VoutCounter);

    return ds;
}

Statement *InitialInfSet(std::string fname, FunctionType * ft, const Location & location)
{
    LOG(LOG_FN_ENTRY, "InitialInfSet(%s, %p, %p)\n", fname.c_str(), ft, &location);

    VoutFunctionType::iterator func = voutFunctions.find(fname);
    if (func == voutFunctions.end())
        return NULL;
    std::set < unsigned int >::iterator iter = func->second.begin();
    Expression *expression;

    if (iter != func->second.end())
    {
        Symbol *vout_sym = ft->args[*iter]->name->dup();

        expression = new AssignExpr(AO_Equal, new Variable(vout_sym, location),
#ifdef INF_SENTINEL
                                    new IndexExpr(new Variable(findSentinel(ft),
                                                               location), new FloatConstant(0.0, location), location),
#else
                                    new Variable(findSentinel(ft), location),
#endif
                                    location);
        ++iter;
    }
    else
        return NULL;
    for (; iter != func->second.end(); ++iter)
    {
        Symbol *vout_sym = ft->args[*iter]->name->dup();
        Symbol *Why = new Symbol;

        Why->name = "y";
        expression = new BinaryExpr(BO_Comma, expression, new AssignExpr(AO_Equal, new Variable(vout_sym, location),
#ifdef INF_SENTINEL
                                                                         new IndexExpr(new Variable
                                                                                       (findSentinel(ft),
                                                                                        location),
                                                                                       new FloatConstant
                                                                                       (0.0, location), location),
#else
                                                                         new Variable(findSentinel(ft), location),
#endif
                                                                         location), location);

    }
    return new ExpressionStemnt(expression, location);
}

static FunctionType *pushFunctionType = NULL;

#if 0
static Statement *PushToIfStatement(Statement * ste)
{
    LOG(LOG_FN_ENTRY, "PushToIfStatement(%p)\n", ste);
    Statement *newstemnt = NULL;
    FunctionType *ft = pushFunctionType;
    Decl *vout_counter = findVoutCounter(ft);

    if (ste->type == ST_ExpressionStemnt)
    {
        ExpressionStemnt *es = static_cast < ExpressionStemnt * >(ste);

        if (es->expression->etype == ET_FunctionCall)
        {
            FunctionCall *fc = static_cast < FunctionCall * >(es->expression);

            if (fc->args.size() == 1
                && fc->function->etype == ET_Variable && static_cast < Variable * >(fc->function)->name->name == "push")
            {
                if (fc->args[0]->etype != ET_Variable)
                {
                    std::cerr << "Error: ";
                    fc->args[0]->location.printLocation(std::cerr);
                    std::cerr << " Push called without specific vout stream.\n";
                    return NULL;
                }
                std::string voutname = static_cast < Variable * >(fc->args[0])->name->name;
                Decl *streamDecl = NULL;

                for (unsigned int i = 0; i < (unsigned int) ft->nArgs; ++i)
                {
                    if (ft->args[i]->name->name == std::string("__") + voutname + "_stream")
                    {
                        streamDecl = ft->args[i];
                        break;
                    }
                }
                if (streamDecl == NULL)
                {
                    std::cerr << "Error: ";
                    fc->args[0]->location.printLocation(std::cerr);
                    std::cerr << " Push called on var that is not a vout arg.\n";
                    return NULL;
                }
                Symbol *Eks = new Symbol;

                Eks->name = "x";
                Symbol *counter = vout_counter->name->dup();

                counter->name = vout_counter->name->name;
                counter->entry = mk_paramdecl(counter->name, vout_counter);
                Symbol *stream = new Symbol;

                stream->name = "__" + voutname + "_stream";
                stream->entry = mk_paramdecl(stream->name, streamDecl);
                Block *AssignStream = new Block(fc->location);

                AssignStream->add
                    (new ExpressionStemnt
                     (new AssignExpr(AO_Equal,
                                     new Variable(stream, fc->location),
                                     fc->args[0]->dup(), fc->location), fc->location));

                newstemnt = new IfStemnt
                    (new RelExpr(RO_Equal,
                                 new FloatConstant(-1, fc->location),
                                 new AssignExpr(AO_MinusEql,
                                                new Variable(counter,
                                                             fc->location),
                                                new IntConstant(1, fc->location),
                                                fc->location), fc->location), AssignStream, fc->location);

            }
        }
    }
    return newstemnt;
}
#endif

bool isFilter(Expression * vout)
{
    LOG(LOG_FN_ENTRY, "isFilter(%p)\n", vout);
    if (vout && vout->etype == ET_Constant)
    {
        Constant *cons = static_cast < Constant * >(vout);

        if (cons->ctype == CT_UInt)
        {
            if (static_cast < UIntConstant * >(cons)->ulng == 1)
            {
                return true;
            }
        }
        if (cons->ctype == CT_Int)
        {
            if (static_cast < IntConstant * >(cons)->lng == 1)
            {
                return true;
            }
        }
        if (cons->ctype == CT_Char)
        {
            if (static_cast < CharConstant * >(cons)->ch == 1)
            {
                return true;
            }
        }
    }

    return false;

}
static Expression *TransformExprVoutPush(Expression * expression)
{
    LOG(LOG_FN_ENTRY, "TransformExprVoutPush(%p)\n", expression);
    FunctionType *ft = pushFunctionType;
    Decl *vout_counter = findVoutCounter(ft);

    if (expression->etype == ET_FunctionCall)
    {
        FunctionCall *fc = static_cast < FunctionCall * >(expression);

        if (fc->args.size() == 1
            && fc->function->etype == ET_Variable && static_cast < Variable * >(fc->function)->name->name == "push")
        {
            if (fc->args[0]->etype != ET_Variable)
            {
                std::cerr << "Error: ";
                fc->args[0]->location.printLocation(std::cerr);
                std::cerr << " Push called without specific vout stream.\n";
                return expression;
            }
            std::string voutname = static_cast < Variable * >(fc->args[0])->name->name;
            Decl *streamDecl = ft->findArg((std::string("__") + voutname + "_stream").c_str());

            if (streamDecl == NULL)
            {
                std::cerr << "Error: ";
                fc->args[0]->location.printLocation(std::cerr);
                std::cerr << " Push called on var that is not a vout arg.\n";
                return expression;
            }

            Symbol *counter = vout_counter->name;
            Symbol *stream = streamDecl->name;

            bool filter = isFilter(streamDecl->form->getQualifiers().vout);

            if (filter)
            {
                for (unsigned int i = 0; i < ft->nArgs; ++i)
                {
                    if ((ft->args[i]->form->getQualifiers() & TQ_Vout) != 0
                        || (ft->args[i]->form->getQualifiers() & TQ_Out) != 0)
                    {
                        if (!isFilter(ft->args[i]->form->getQualifiers().vout))
                        {
                            filter = false;
                        }
                    }
                }
            }
            Expression *newExp;

            if (filter)
            {
                newExp = new AssignExpr(AO_Equal, new Variable(stream, fc->location), fc->args[0]->dup(), fc->location);
            }
            else
            {
                newExp = new AssignExpr
                    (AO_Equal,
                     new Variable(stream, fc->location),
                     new TrinaryExpr(new RelExpr(RO_Equal,
                                                 new FloatConstant(-1, fc->location),
                                                 new AssignExpr(AO_MinusEql,
                                                                new Variable(counter,
                                                                             fc->location),
                                                                new IntConstant(1,
                                                                                fc->location),
                                                                fc->location),
                                                 fc->location),
                                     fc->args[0]->dup(),
                                     new Variable(stream->dup(), fc->location), fc->location), fc->location);
            }
            expression->replace(newExp);
        }
    }
    return expression;
}

FunctionDef *TransformVoutPush(FunctionDef * fd)
{
    LOG(LOG_FN_ENTRY, "TransformVoutPush(%p)\n", fd);
    VoutFunctionType::iterator func = voutFunctions.find(fd->FunctionName()->name);
    if (func == voutFunctions.end())
        return NULL;
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);

    pushFunctionType = ft;
    fd->findExpr(&TransformExprVoutPush);
    //if (0) TransformStemnt(fd,&PushToIfStatement);
    Block *MainFunction = new Block(fd->location);
    Statement *tmp = MainFunction->head;

    MainFunction->head = fd->head;
    fd->head = tmp;
    tmp = MainFunction->tail;
    MainFunction->tail = fd->tail;
    fd->tail = tmp;
    fd->addDeclStemnts(MakeVoutCounter(fd->FunctionName()->name, ft, fd->location));
    fd->add(InitialInfSet(fd->FunctionName()->name, ft, fd->location));
    fd->add(MainFunction);
    return NULL;
}

void transform_vout(TransUnit * tu)
{
    LOG(LOG_FN_ENTRY, "transform_vout(%p)\n", tu);
    tu->findFunctionDef(IdentifyVoutFunc);
    tu->findFunctionDef(TransformHeader);
    tu->findFunctionDef(TransformVoutToOut);
    tu->findFunctionDef(TransformVoutPush);
    //transform push calls
    //transform function calls

}
