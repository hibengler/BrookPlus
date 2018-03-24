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
 * brtstemnt.cpp
 *
 *      Brook extensions to cTool's stemnt.cpp.  Specifically, contains the
 *      BRTKernelDef class, which represents a kernel definition.
 */
//#include "brtvout.h"
//above file must be included first so the #pragma warning:disable is included
#include <cstring>
#include <cassert>
#include <sstream>

#include "logger.hpp"
#include "brtstemnt.h"
#include "brtreduce.h"
#include "brtdecl.h"
#include "brtexpress.h"
#include "brtscatter.h"
#include "project.h"
#include "main.h"

//FIXME eventually we'll want to code-transform to do the following 2 functions
bool recursiveIsGather(Type * form)
{
    LOG(LOG_FN_ENTRY, "recursiveIsGather(%p)\n", form);

    if ((form->getQualifiers() & TQ_Out) != 0)
        return false;

    bool ret = (form->type == TT_Array) && (form->getQualifiers() & TQ_Reduce) == 0;
    bool isarray = ret;
    Type *t = form;

    while (isarray)
    {
        t = static_cast < ArrayType * >(t)->subType;
        isarray = (t->type == TT_Array);
    }
    return ret && t->type != TT_Stream;
}

bool recursiveIsStream(Type * form)
{
    LOG(LOG_FN_ENTRY, "recursiveIsStream(%p)\n", form);
    return (form->type == TT_Stream);
}

bool isScatterOut(Type * form)
{
    return (form->isArray() && (form->getQualifiers() & TQ_Out )!=0);
}

BRTKernelDef::BRTKernelDef(const FunctionDef & fDef):FunctionDef(fDef.location)
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::BRTKernelDef(%p)\n", &fDef);
    LabelVector::const_iterator j;

    type = ST_BRTKernel;
    decl = fDef.decl->dup();
    kernelAttribute = fDef.kernelAttribute->dup();

    for (Statement * stemnt = fDef.head; stemnt; stemnt = stemnt->next)
    {
        add(stemnt->dup());
    }

    for (j = fDef.labels.begin(); j != fDef.labels.end(); j++)
    {
        addLabel((*j)->dup());
    }
    if (!CheckSemantics())
    {
        assert(false);
    }
}


void BRTKernelDef::print(std::ostream & out, int dummy) const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::print(%p, %d)\n", &out, dummy);
    char name[1024];

    if (Project::gDebug)
    {
        out << "/* BRTKernelDef:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    assert(FunctionName());
    assert(FunctionName()->entry);
    assert(FunctionName()->entry->scope);

    /* If the symbol for the generated assembly code already 
     ** exists, don't generate the assembly.  This allows the user
     ** to hand generate the code.
     */

#define PRINT_CODE(a,b) \
   sprintf (name, "__%s_%s", FunctionName()->name.c_str(), #b);      \
   if (!FunctionName()->entry->scope->Lookup(name)) {                \
      if (globals.userTargets & TARGET_##a) {                             \
         BRTKernelCode *var;                                         \
            var = decl->isReduce() ? new BRT##a##ReduceCode(*this) : \
                                     new BRT##a##KernelCode(*this);  \
         out << *var << std::endl;                                   \
         delete var;                                                 \
      } else {                                                       \
         out << "static const char *__"                              \
             << *FunctionName() << "_" << #b << "= NULL;\n";         \
      }                                                              \
   }

    PRINT_CODE(CAL, cal);
    //PRINT_CODE(PS20, ps20);
    //PRINT_CODE(PS2B, ps2b);
    //PRINT_CODE(PS2A, ps2a);
    //PRINT_CODE(PS30, ps30);
    //PRINT_CODE(FP30, fp30);
    //PRINT_CODE(FP40, fp40);
    //PRINT_CODE(ARB, arb);
    PRINT_CODE(CPU, cpu);
#undef PRINT_CODE

    //Fix me: sending dummy kernel header
    assert(0);
    std::ofstream dummyKernelHeader;
    printStub(out, dummyKernelHeader);
}

bool incrementBoolVec(std::vector < bool > &vec)
{
    LOG(LOG_FN_ENTRY, "incrementBoolVec(%p)\n", &vec);
    if (vec.empty())
        return false;
    bool carry = true;

    for (std::vector < bool >::iterator i = vec.begin(); carry && i != vec.end(); ++i)
    {
        carry = *i;
        *i = !(*i);
    }
    return !carry;
}

static std::string getDeclStream(Decl * vout, std::string append = "_stream")
{
    LOG(LOG_FN_ENTRY, "getDeclStream(%p, %s)\n", vout, append.c_str());
    std::string temp = vout->name->name;
    size_t i = temp.find("_stream");

    if (i == std::string::npos)
    {
        return temp + "_stream";
    }
    return temp.substr(0, i) + append;
}

#if KEEP_VOUT
void BRTKernelDef::PrintVoutPrefix(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::PrintVoutPrefix(%p)\n", &out);
    FunctionType *ft = static_cast < FunctionType * >(decl->form);
    std::set < unsigned int >*vouts = &voutFunctions[FunctionName()->name];
    std::set < unsigned int >::iterator iter;

    out << "  float __vout_counter=0.0f;" << std::endl;
#ifdef INF_SENTINEL
    out << "  brook::Stream *__inf = *brook::sentinelStream(1);";
#else
    out << "  float __inf = getSentinel();";
#endif //INF_SENTINEL
    out << std::endl;
    out << "  unsigned int maxextents[2]={0,0};" << std::endl;

    unsigned int i = 0;

    for (bool found = 0; i < ft->nArgs; ++i)
    {
        if ((ft->args[i]->form->getQualifiers() & TQ_Out) == 0
            && ft->args[i]->isStream() && vouts->find(i) == vouts->end())
        {
            std::string name = ft->args[i]->name->name;
            if (!found)
            {
                out << "  unsigned int __dimension = " << name << "->getDimension();";
                out << std::endl;
            }
            out << "  assert (" << name << "->getDimension()<=2);" << std::endl;
            out << "  brook::maxDimension(maxextents," << name << "->getExtents(),";
            out << name << "->getDimension());" << std::endl;
            found = true;
        }
    }

    for (iter = vouts->begin(); iter != vouts->end(); ++iter)
    {
        out << "  std:: vector <brook::StreamType> ";
        std::string typevector = getDeclStream(ft->args[*iter], "_types");
        out << typevector << ";" << std::endl;
        std::string streamiter = getDeclStream(ft->args[*iter], "_iter");
        out << "  for (unsigned int " << streamiter << " = 0; ";
        out << streamiter << " < " << ft->args[*iter]->name->name;
        out << "->getFieldCount(); ++";
        out << streamiter << ") \n";
        out << "    " << typevector << ".push_back(" << ft->args[*iter]->name->name;
        out << "->getIndexedFieldType(" << streamiter << "));\n";
        out << "  " << typevector << ".push_back(brook::__BRTNONE);\n";
        out << "  std::vector<brook::stream*> ";
        out << getDeclStream(ft->args[*iter], "_outputs") << ";";
        out << std::endl;
        out << "  bool " << getDeclStream(ft->args[*iter], "_values") << " = true;";
        out << std::endl;
    }

    out << "  while (";
    iter = vouts->begin();
    out << getDeclStream(ft->args[*iter++], "_values");
    for (; iter != vouts->end(); ++iter)
    {
        out << " || " << getDeclStream(ft->args[*iter], "_values");
    }

    out << ") {" << std::endl;
    for (iter = vouts->begin(); iter != vouts->end(); ++iter)
    {
        std::string typevector = getDeclStream(ft->args[*iter], "_types");
        out << "    if (" << getDeclStream(ft->args[*iter], "_values") << ")";
        out << std::endl;
        out << "      " << getDeclStream(ft->args[*iter], "_outputs");
        out << ".push_back (new ::brook::Stream (maxextents, ";
        out << "__dimension, &" << typevector << "[0]));";
        out << std::endl;
    }
}

std::string undecoratedBase(Decl * decl)
{
    LOG(LOG_FN_ENTRY, "undecoratedBase(%p)\n", decl);
    BaseType *base = decl->form->getBase();
    BaseTypeSpec typemask = base->typemask;

    std::string res = getBaseTypeName(typemask, BaseTypeBRName);
    return res;
}

std::string getDimensionString(int dim)
{
    LOG(LOG_FN_ENTRY, "getDimensionString(%d)\n", dim);
    std::string dimensionstring;
    if (dim != 2)
    {
        dimensionstring += dim + '0';
        dimensionstring += 'd';
    }

    return dimensionstring;
}

void BRTKernelDef::PrintVoutDimensionalShift(std::ostream & out, Decl * decl, unsigned int dim) const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::PrintVoutDimensionalShift(%p, %p, %d)\n", &out, decl, dim);

    std::string type = undecoratedBase(decl);
    unsigned int i;

    std::string dimensionstring = getDimensionString(dim);
    out << "    ::brook::Stream " << getDeclStream(decl, "_temp") << "(&";
    std::string typevector = getDeclStream(decl, "_types");
    out << typevector << "[0],";
    for (i = 0; i < dim; ++i)
    {
        out << "1, ";
    }
    out << "-1);" << std::endl;
    out << "    combineStreams" << dimensionstring;
    out << type << " (&" << getDeclStream(decl, "_outputs") << "[0],";
    out << std::endl;
    out << "                   " << getDeclStream(decl, "_outputs") << ".size()-1,";
    out << std::endl;
    out << "                   maxextents[0],";
    out << std::endl;
    out << "                   maxextents[1],";
    out << std::endl;
    out << "                   &" << getDeclStream(decl, "_temp") << ");";
    out << std::endl;
    out << "    shiftValues" << dimensionstring;
    out << type << "(" << getDeclStream(decl, "_temp") << ",";
    out << std::endl;
    out << "                &" << decl->name->name << ",";
    out << std::endl;
    for (i = 0; i < dim; ++i)
    {
        out << "                " << getDeclStream(decl, "_temp");
        out << "->getExtents()[" << i << "],";
        out << std::endl;
    }
    for (; i < 2; ++i)
        out << "                 1,";
    out << "                -1);" << std::endl;
}

extern bool isFilter(Expression *);

void BRTKernelDef::PrintVoutPostfix(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::PrintVoutPostfix(%p)\n", &out);
    out << "    __vout_counter+=1.0f;" << std::endl;
    FunctionType *ft = static_cast < FunctionType * >(decl->form);
    std::set < unsigned int >::iterator beginvout = voutFunctions[FunctionName()->name].begin();
    std::set < unsigned int >::iterator endvout = voutFunctions[FunctionName()->name].end();
    std::set < unsigned int >::iterator iter;
    std::set < unsigned int >::iterator inneriter;
    bool limited_vout = false;
    bool allone = true;
    unsigned int limited_vout_counter = 0;
    unsigned int numlimits = 0;

    for (iter = beginvout; iter != endvout; ++iter)
    {
        Decl *decl = ft->args[*iter];
        Expression *vout_limit = decl->form->getQualifiers().vout;

        if (vout_limit)
        {
            bool filter = isFilter(vout_limit);

            allone = (allone && filter);

            if (limited_vout || beginvout == iter)
            {
                limited_vout = true;
                numlimits++;
            }
            else
                limited_vout = false;
        }
    }
    if (numlimits > 1 && !allone)
    {
        numlimits = 0;
        limited_vout = false;
    }
    for (iter = beginvout; iter != endvout; ++iter)
    {
        Decl *decl = ft->args[*iter];
        Expression *vout_limit = decl->form->getQualifiers().vout;

        if (vout_limit && limited_vout)
        {
            if (limited_vout_counter == 0)
                out << "     if (";
#if 0                           // FIXME - dgladdin - the call to RelExpr below causes a NULL pointer dereference,
            //                    for now I've locked useparen to true on the assumption that
            //                    excess parens are ugly but harmless
            bool useparen = (vout_limit->precedence() < RelExpr(RO_Less,
                                                                NULL,
                                                                NULL,
                                                                vout_limit->location).precedence());
#else
            bool useparen = true;
#endif
            // the above is a simple check for the common expressions.
            // no need to get fancy here for parens are ok in this case.
            out << "(__vout_counter >= ";
            if (useparen)
                out << "(";
            vout_limit->print(out);
            if (useparen)
                out << ")";
            out << ")";
            limited_vout_counter++;
            if (limited_vout_counter == numlimits)
            {
                out << ") {" << std::endl;
                for (inneriter = beginvout; inneriter != endvout; ++inneriter)
                {
                    Decl *decl = ft->args[*inneriter];

                    out << "      ";
                    out << getDeclStream(decl, "_outputs") << ".push_back(0);" << std::endl;
                }
                out << "      ";
                out << "break;" << std::endl;
                out << "    }" << std::endl;
            }
            else
            {
                out << "&&";
            }
        }
        else
        {
            out << "    " << getDeclStream(decl, "_values") << " = ";

            out << "(";
            out << decl->name->name << "->getDimension()==2?";
            out << "finiteValueProduced" << getDimensionString(2) << undecoratedBase(decl);
            out << ":finiteValueProduced" << getDimensionString(1);
            out << undecoratedBase(decl) << ")(*" << getDeclStream(decl, "_outputs");
            out << ".back())?1:0;" << std::endl;
        }
    }
    out << "  }" << std::endl;
    for (iter = beginvout; iter != endvout; ++iter)
    {

        Decl *decl = ft->args[*iter];

        out << "  if (" << decl->name->name << "->getDimension()==2) {" << std::endl;
        PrintVoutDimensionalShift(out, decl, 2);
        out << "  }else {" << std::endl;
        PrintVoutDimensionalShift(out, decl, 1);
        out << "  }" << std::endl;
        out << "  while (" << getDeclStream(decl, "_outputs") << ".size()) {";
        out << std::endl;
        out << "    if (" << getDeclStream(decl, "_outputs") << ".back())" << std::endl;
        out << "      delete " << getDeclStream(decl, "_outputs") << ".back();";
        out << std::endl;
        out << "    " << getDeclStream(decl, "_outputs") << ".pop_back();" << std::endl;
        out << "  }" << std::endl;
    }
}


static void printPrototypes(std::ostream & out, std::string type)
{
    LOG(LOG_FN_ENTRY, "printPrototypes(%p, %s)\n", &out, type.c_str());
    int i;

    for (i = 1; i <= 2; ++i)
    {
        std::string dimensionstring = getDimensionString(i);
        out << "extern int finiteValueProduced" << dimensionstring;
        out << type << " (brook::stream input);\n";
        out << "extern float shiftValues" << dimensionstring;
        out << type << "(brook::stream list_stream,\n"
            "                         brook::stream *output_stream,\n"
            "                         int WIDTH, \n"
            "                         int LENGTH, \n"
            "                         int sign);\n" "void combineStreams" << dimensionstring;
        out << type << "(brook::stream **streams,\n"
            "                     unsigned int num,\n"
            "                     unsigned int width, \n"
            "                     unsigned int length,\n" "                     brook::stream *output) ;\n";

    }
}

#endif // KEEP_VOUT

#define FuncRelLine -2 
void BRTKernelDef::printStub(std::ostream & out, std::ostream &kernelDeclHeader) const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::printStub(%p)\n", &out);
    if (!returnsVoid())
        return;
    FunctionType *fType;
    unsigned int i, NumArgs;

#if KEEP_VOUT
    bool vout = voutFunctions.find(FunctionName()->name) != voutFunctions.end();
#endif // KEEP_VOUT

    if (globals.printLineDirectives)
        location.printLineDirective(out, FuncRelLine);
#if KEEP_VOUT
    if (vout)
    {
        printPrototypes(out, "float");
        printPrototypes(out, "float2");
        printPrototypes(out, "float3");
        printPrototypes(out, "float4");
    }
#endif // KEEP_VOUT
    assert(decl->form->type == TT_Function);
    fType = (FunctionType *) decl->form;
    std::vector < bool > streamOrVal;
    NumArgs = fType->nArgs;
#if KEEP_VOUT
    if (vout)
    {
        for (int i = NumArgs - 1; i >= 0; --i)
        {
            if (fType->args[i]->name->name == "__inf" || fType->args[i]->name->name == "__vout_counter")
                NumArgs--;
            else
                break;
        }
    }
#endif // KEEP_VOUT
    for (i = 0; i < NumArgs; i++)
    {
        if ((fType->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
        {
            if (fType->args[i]->form->isStream())
                streamOrVal.push_back(false);
            else
                streamOrVal.push_back(true);
        }
    }

    kernelDeclHeader << "class __" << *FunctionName() <<"\n"
                     << "{\n"
                     << "    public:\n"
                     << "        ";

    do
    {
        unsigned int reducecount = 0;

        fType->subType->printType(kernelDeclHeader, NULL, true, 0);
        kernelDeclHeader << "operator()(";

        fType->subType->printType(out, NULL, true, 0);
        out << "__" << *FunctionName() << "::" <<"operator()(";

        for (i = 0; i < NumArgs; i++)
        {
            if (i) {
                out << ",\n";
                kernelDeclHeader << ", ";
                if (globals.printLineDirectives)
                    location.printLineDirective(out, FuncRelLine);

                out << "\t\t";
            }

            if ((fType->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
            {
                if (streamOrVal[reducecount])
                {
                    Symbol name;

                    name.name = "& " + fType->args[i]->name->name;
                    Type *t = fType->args[i]->form;

                    if (fType->args[i]->isStream())
                        t = static_cast < ArrayType * >(fType->args[i]->form)->subType;
                    t->printType(out, &name, true, 0);
                    t->printType(kernelDeclHeader, &name, true, 0);
                }
                else
                {
                    out << "const ::brook::Stream< ";
                    kernelDeclHeader << "const ::brook::Stream< ";

                    fType->args[i]->form->printBase(out, 0);
                    fType->args[i]->form->printBase(kernelDeclHeader, 0);

                    out << ">& " << *fType->args[i]->name;
                    kernelDeclHeader << ">& " << *fType->args[i]->name;
                }
            }
            else if ((fType->args[i]->form->getQualifiers() & TQ_Iter) != 0)
            {
                out << "const __BRTIter& " << *fType->args[i]->name;
                kernelDeclHeader << "const __BRTIter& " << *fType->args[i]->name;
            }
            else if (recursiveIsStream(fType->args[i]->form) 
                || recursiveIsGather(fType->args[i]->form)
                || isScatterOut(fType->args[i]->form))
            {
                //! For Gather array used as constant buffer
                if(fType->args[i]->isArray() && fType->args[i]->form->getQualifiers() != TQ_Out)
                {
                    ArrayType* arrayType = AsArrayType(fType->args[i]->form);
                    if(arrayType->resourceType == ConstBufferType)
                    {
                        out << "const ";
                        kernelDeclHeader << "const ";
                        fType->args[i]->print(out, true);
                        fType->args[i]->print(kernelDeclHeader, true);
                        continue;
                    }
                }
                // get the type name of argument including qualifier
                std::ostringstream typeNameStream;
                fType->args[i]->form->printBase(typeNameStream, 0);
                std::string typeNameStr = typeNameStream.str();
                std::string typeQualifier = "";
                std::string baseTypeName = "";
               
                //divide qualifier and type
                size_t spacePos = typeNameStr.find_last_of (" ", typeNameStr.length());
                spacePos = typeNameStr.find_last_of (" ", spacePos - 1);
                if(spacePos > 0 && spacePos < typeNameStr.length())
                {
                    typeQualifier = typeNameStr.substr(0, spacePos);
                    baseTypeName = typeNameStr.substr(spacePos, typeNameStr.length());
                    if(typeQualifier.find("out") < 0 && typeQualifier.find("out") > typeQualifier.length())
                    {
                        out << typeQualifier.c_str() << " ";
                        kernelDeclHeader << typeQualifier.c_str() << " ";
                    }
                    out << "const ::brook::Stream< ";
                    kernelDeclHeader << "const ::brook::Stream< ";
                }
                else
                {
                    baseTypeName = typeNameStr.substr(0, typeNameStr.length());
                    out << "const ::brook::Stream< ";
                    kernelDeclHeader << "const ::brook::Stream< ";
                }
                out << baseTypeName.c_str();
                kernelDeclHeader << baseTypeName.c_str();
                out << ">& ";
                kernelDeclHeader << ">& ";
#if KEEP_VOUT
                if ((voutFunctions[FunctionName()->name].find(i) != voutFunctions[FunctionName()->name].end()))
                {
                    out << "&";
                    // Vout changes dimension and must be passed by ref
                    // Optionally we could make streamSwap work properly--but this
                    // is tricky with all the behind-the-scenes inheritance going on
                    // if you change, please talk to danielrh at graphics 
                    // first  Otherwise he'll have to fix all his vout tests.
                }
#endif // KEEP_VOUT
                out << *fType->args[i]->name;
                kernelDeclHeader << *fType->args[i]->name;
            }
            else
            {
                out << "const ";
                kernelDeclHeader << "const ";
                Symbol name;

                name.name = fType->args[i]->name->name;
                //XXX -- C++ backend needs values to be passed by value...
                // It's a one time per kernel call hit--worth it to keep
                // Values from being aliased --Daniel
                //hence we only do the & for reduction vars
                fType->args[i]->form->printType(out, &name, true, 0);
                fType->args[i]->form->printType(kernelDeclHeader, &name, true, 0);
            }
        }
        out << ")\n";
        kernelDeclHeader << ");\n";

        out << "{\n";           
        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
        //    out << "#line 2 \"" << globals.cppOutputName << "\"" << std::endl;


        out << "  static const void *__" << *FunctionName() << "_fp[] = {";
        out << std::endl;

        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
        out << "     \"cal\", __" << *FunctionName() << "_cal," << std::endl;
        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
        out << "     \"cpu\", (void *) __" << *FunctionName() << "_cpu," << std::endl;
        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
        out << "     NULL, NULL };\n" << std::endl;

        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);

        out << "  ::brook::Kernel  __k(" << "__" << *FunctionName() << "_fp, brook::";

        if (decl->isReduce())
        {
            out << "KERNEL_REDUCE";
        }
        else
        {
            out << "KERNEL_MAP";
        }
        out << ");\n";
        out << "  ::brook::ArgumentInfo __argumentInfo;\n\n";

#if KEEP_VOUT
        if (vout)
        {
            PrintVoutPrefix(out);
        }
#endif // KEEP_VOUT
        for (i = 0; i < fType->nArgs; i++)
        {
            if (globals.printLineDirectives)
                location.printLineDirective(out, FuncRelLine);
#if KEEP_VOUT            
            if (vout)
                out << "  ";    //nice spacing
#endif // KEEP_VOUT
            if (recursiveIsStream(fType->args[i]->form)
                && (fType->args[i]->form->getQualifiers() & TQ_Out) != 0)
            {
#if KEEP_VOUT
                if (voutFunctions.find(FunctionName()->name) == voutFunctions.end()
                    || voutFunctions[FunctionName()->name].find(i) == voutFunctions[FunctionName()->name].end())
                {
#endif // KEEP_VOUT
                    out << "  __k.PushOutput(" << *fType->args[i]->name << ");\n";
#if KEEP_VOUT
                }
                else
                {
                    out << "  __k->PushOutput(*" << getDeclStream(fType->args[i], "_outputs");
                    out << ".back());\n";
                }
#endif // KEEP_VOUT
            }
            else if (fType->args[i]->isArray() && (fType->args[i]->form->getQualifiers() & TQ_Out) != 0)
            {
                out << "  __k.PushScatterStream(" << *fType->args[i]->name << ");\n";
            }
            else if ((fType->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
            {
                if(!streamOrVal[reducecount])
                {
                    out << "  __k.PushReduceStream(" << *fType->args[i]->name << ");\n";
                }
                else
                {
                    out << "  unsigned int __dim[] = {1};\n";
                    out << "  ::brook::Stream< ";
                    fType->args[i]->form->printBase(out, 0);

                    out << "> __stream_" << *fType->args[i]->name << "(1, __dim);\n";
                    out << "  __k.PushReduce(" << "__stream_" << *fType->args[i]->name << ");\n";
                }
            }
            else if ((fType->args[i]->form->getQualifiers() & TQ_Iter) != 0)
            {
                if(globals.userTargets & TARGET_CPU)
                {
                    out << "  __k.PushIter(" << *fType->args[i]->name << ");\n";
                }
            }
            else if (recursiveIsStream(fType->args[i]->form))
            {
                out << "  __k.PushStream(" << *fType->args[i]->name << ");\n";
            }
            else if (recursiveIsGather(fType->args[i]->form))
            {
                ArrayType* arrayType = AsArrayType(fType->args[i]->form);
                if(arrayType->resourceType == ConstBufferType)
                {
                    //! For Gather array used as constant buffer
                    out << "  brook::Constant<";
                    fType->args[i]->form->printBase(out, 0);
                    out << "> constant_" << i << "((";
                    fType->args[i]->form->printBase(out, 0);
                    out << "*)";
                    out << *fType->args[i]->name << ", " << arrayType->getTotalElementCount() << ");\n";

                    // Modifications for constant buffer implementation for CPU
                    out << "  unsigned int __dim" << i << "[] = { ";
                    int j;
                    for(j = 0; j < arrayType->getDimensions() - 1; j++)
                    {
                        out<< arrayType->getDimensionsSize(arrayType->getDimensions() - j) <<", ";
                    }
                    out<< arrayType->getDimensionsSize(arrayType->getDimensions() - j) <<"};\n";
                    out << "  __k.PushConstantBuffer(constant_" << i <<", " << arrayType->getDimensions() \
                        << ", " << "__dim" << i << ");\n";
                }
                else
                {
                    out << "  __k.PushGatherStream(" << *fType->args[i]->name << ");\n";
                }
            }
            else
            {
                Symbol name;
                name.name = fType->args[i]->name->name;
                
                out << "  brook::Constant<";
                fType->args[i]->form->printBase(out, 0);
                out << "> constant_" << i << "(" << *fType->args[i]->name << ");\n";

                out << "  __k.PushConstant(" << "constant_" << i << ");\n";
            }
        }
        if (globals.printLineDirectives)
            location.printLineDirective(out, FuncRelLine);
#if KEEP_VOUT        
        if (vout)
            out << "  ";        //nice spacing
#endif // KEEP_VOUT
        out << "  __argumentInfo.startExecDomain = _domainOffset;" << std::endl;
        out << "  __argumentInfo.domainDimension = _domainSize;" << std::endl;

        //! Add kernel attribute Group Size 
        if(kernelAttribute)
        {
            GccAttrib* groupSize = NULL;
            GccAttrib* tempAttrib = kernelAttribute;
            while(tempAttrib)
            {
                if(tempAttrib->type == Kernel_GroupSize)
                {                
                    groupSize = tempAttrib;
                    break;
                }
                tempAttrib = tempAttrib->next;
            }
            if(groupSize)
            {
                std::vector<int> values;
                INumList *tempNumList = groupSize->ns;
                while(tempNumList)
                {
                    values.push_back(tempNumList->value);
                    tempNumList = tempNumList->next;
                }
                out << "  unsigned int userGroupSize[" << values.size() << "] = { ";
                
                for(unsigned int i = 0; i < values.size() - 1; ++i)
                {
                    out << values[values.size() - i - 1];
                    out << ", ";
                }
                out << values[0];
                out << "};" << std::endl;

                out << "  __argumentInfo.groupSize = userGroupSize;" << std::endl;
            }
        }

        out << "\n\n  __k.run(&__argumentInfo);\n";
        out << "  DESTROYPARAM();\n";

        for (i = 0; i < fType->nArgs; i++)
        {
            if ((fType->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
            {
                if(streamOrVal[reducecount])
                {
                    out << "__stream_" << *fType->args[i]->name << ".write(&"
                        << *fType->args[i]->name << ");\n";
                }
            }
        }
#if KEEP_VOUT
        if (vout)
            PrintVoutPostfix(out);
#endif // KEEP_VOUT
        out << "\n}\n\n";
    }
    while (incrementBoolVec(streamOrVal));

    out << "__THREAD__ __" << *FunctionName() << " " << *FunctionName() << ";\n\n";
    kernelDeclHeader << "        EXTENDCLASS();\n"
                     << "};\n"
                     << "extern __THREAD__ __" << *FunctionName() << " " << *FunctionName() << ";\n\n";
}

bool BRTKernelDef::CheckSemantics() const
{
    LOG(LOG_FN_ENTRY, "BRTKernelDef::CheckSemantics()\n");
    FunctionType *fType;

    assert(decl->form->type == TT_Function);
    fType = (FunctionType *) decl->form;
    for (unsigned int i = 0; i < fType->nArgs; i++)
    {
        if (!fType->args[i]->isStream() && (fType->args[i]->form->getQualifiers() & TQ_Iter) != 0)
        {
            std::ostringstream msg;
            msg << location << "'";
            fType->args[i]->print(msg, true);
            msg << "' is tagged an iter, but is not a stream!\n";
#if BROOK_TO_IL_CODE_GEN_PATH
            semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
            return false;
        }

/*TIM: remove type checking for arglist (because of structs)
      BaseTypeSpec baseType;

      baseType = fType->args[i]->form->getBase()->typemask;
      if (baseType < BT_Float || baseType > BT_Float4) {
         std::cerr << location << "Illegal type in ";
         fType->args[i]->print(std::cerr, true);
         std::cerr << ". (Must be floatN).\n";
         return false;
      }
*/
    }

    return true;
}


bool BRTMapKernelDef::CheckSemantics() const
{
    LOG(LOG_FN_ENTRY, "BRTMapKernelDef::CheckSemantics()\n");
    FunctionType *fType;
    Decl *outArg = NULL;

    assert(decl->form->type == TT_Function);
    fType = (FunctionType *) decl->form;

    for (unsigned int i = 0; i < fType->nArgs; i++)
    {
        if (fType->args[i]->isReduce())
        {
            std::ostringstream msg;
            msg << location << "Reduce arguments are not allowed in " << *FunctionName() << ": ";
            fType->args[i]->print(msg, true);
            msg << ".\n";
#if BROOK_TO_IL_CODE_GEN_PATH
            semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
            return false;
        }

        if ((fType->args[i]->form->getQualifiers() & TQ_Out) != 0)
        {
            /*        if (outArg) {
               std::cerr << location << "Multiple outputs not supported: ";
               outArg->print(std::cerr, true);
               std::cerr << ", ";
               fType->args[i]->print(std::cerr, true);
               std::cerr << ".\n";
               return false;
               } */
            outArg = fType->args[i];
#if 0
            if (!recursiveIsStream(outArg->form))
            {
                std::ostringstream msg;
                msg << location << "Output is not a stream: ";
                outArg->print(msg, true);
                msg << ".\n";
                semanticCheckRecord(msg.str().c_str(), true);
                return false;
            }
#endif
            if ((outArg->form->getQualifiers() & TQ_Iter) != 0)
            {
                std::ostringstream msg;
                msg << location << "Output cannot be an iterator: ";
                outArg->print(msg, true);
                msg << ".\n";
#if BROOK_TO_IL_CODE_GEN_PATH
                semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
                return false;
            }
        }
    }

    if (outArg == NULL && returnsVoid())
    {
        std::cerr << location << "Warning: " << *FunctionName() << " has no output.\n";
    }

    return true;
}


bool BRTReduceKernelDef::CheckSemantics() const
{
    LOG(LOG_FN_ENTRY, "BRTReduceKernelDef::CheckSemantics()\n");
    FunctionType *fType;
    Decl *streamArg = NULL, *reduceArg = NULL;

    assert(decl->form->type == TT_Function);
    fType = (FunctionType *) decl->form;

    for (unsigned int i = 0; i < fType->nArgs; i++)
    {
        if (fType->args[i]->isReduce())
        {
            if (reduceArg != NULL)
            {
                std::ostringstream msg;
                msg << location << "Multiple reduce arguments in " << *FunctionName() << ": ";
                reduceArg->print(msg, true);
                msg << ", ";
                fType->args[i]->print(msg, true);
                msg << ".\n";
#if BROOK_TO_IL_CODE_GEN_PATH
                semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
                return false;
            }

            reduceArg = fType->args[i];
        }
        else if (fType->args[i]->isStream())
        {
            if (streamArg != NULL)
            {
                std::ostringstream msg;
                msg << location << "Multiple non-reduce streams in " << *FunctionName() << ": ";
                streamArg->print(msg, true);
                msg << ", ";
                fType->args[i]->print(msg, true);
                msg << ".\n";
#if BROOK_TO_IL_CODE_GEN_PATH
                semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
                return false;
            }

            streamArg = fType->args[i];
        }

        if ((fType->args[i]->form->getQualifiers() & TQ_Out) != 0)
        {
            std::ostringstream msg;
            msg << location << "Non-reduce output in reduction kernel " << *FunctionName() << ".\n";
#if BROOK_TO_IL_CODE_GEN_PATH
            semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
            return false;
        }
    }

    if (reduceArg == NULL)
    {
        std::ostringstream msg;
        msg << location << "Reduction kernel " << *FunctionName() << " has no reduce argument.\n";
#if BROOK_TO_IL_CODE_GEN_PATH
        semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
        return false;
    }

    if (streamArg == NULL)
    {
        std::ostringstream msg;
        msg << location << "Reduction kernel " << *FunctionName() << " has no stream argument.\n";
#if BROOK_TO_IL_CODE_GEN_PATH
        semanticCheckRecord(msg.str().c_str(), true);
#endif //BROOK_TO_IL_CODE_GEN_PATH
        return false;
    }

    return true;
}

