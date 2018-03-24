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
 * generate_il.cpp
 *
 *  Utilities for IL generation
 */

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <sstream>
#include <iomanip>
#include <fstream>
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <assert.h>
}

#include "logger.hpp"
#include "main.h"
#include "project.h"
#include "generate_il.h"
#include "express.h"

//todo: create a new source file for semantic check

static const char *SCRMsg[SCRTotal] = {
    "invalid array access",
    "indirect kernel call is not allowed",
    "unrecognized callee",
    "call expression total-actual != total-formal",
    "call expression formal-actual type mismatch",
    "kernel can't use string constant",
    "unsupported datatype (array, union) for variable",
    "problem with reduce kernel",
    "undefined variable",
    "unrecognized swizzle",
    "unsupported operator",
    "problem in"
};

bool semanticCheckOK;

void semanticCheckRecord(Expression * exp, SCResult failCode, const char *msg, bool isCodeGen)
{
    LOG(LOG_FN_ENTRY, "semanticCheckRecord(%p, %d, %s, %d)\n", exp, failCode, msg, isCodeGen);
    if (exp)
        std::cerr << exp->location;
    std::cerr << SCRMsg[failCode] << ": ";
    //exp->print(std::cerr);

    if (msg)
        std::cerr << msg;

    std::cerr << std::endl;
    if (exp)
        exp->getRootExp()->print(std::cerr);
    std::cerr << std::endl;

    semanticCheckOK = false;

    if (isCodeGen)
        exit(1);                //fixme
}

void semanticCheckRecord(Decl* dcl, SCResult failCode)
{
    LOG(LOG_FN_ENTRY, "semanticCheckRecord(%p, %d)\n", dcl, failCode);
    //if (dcl)
    //    std::cerr << dcl->location;
    std::cerr << SCRMsg[failCode] << ": ";
    if (dcl)
        dcl->print(std::cerr, 0);
    std::cerr << std::endl;

    semanticCheckOK = false;
}

void semanticCheckRecord(const char *msg, bool isCodeGen)
{
    LOG(LOG_FN_ENTRY, "semanticCheckRecord(%s, %d)\n", msg, isCodeGen);
    if (msg)
        std::cerr << msg;

    std::cerr << std::endl;

    semanticCheckOK = false;

    if (isCodeGen)
        exit(1);                //fixme
}

#ifdef IL_CONSTBUFFER
namespace bril
{
    using namespace std;

    CBuffNode *CBuffMngr::allocate(int sz)
    {
        LOG(LOG_FN_ENTRY, "CBuffMngr::allocate(%d)\n", sz);
        int idx = 0;
        int off = 0;
        if  (curBuffIdx >= totBuff || sz > buffSz)
        {
            sz = 0;
        }
        else if   ((sz + curBuffOff - 1) < buffSz)
        {
            idx = curBuffIdx;
            off = curBuffOff;
            curBuffOff += sz;

            if (curBuffOff == buffSz)
            {
                buffInfo[curBuffIdx] = curBuffOff;

                curBuffOff = 0;
                curBuffIdx++;
            }
        }
        else if (curBuffIdx + 1 < totBuff)
        {
            curBuffIdx++;
            idx = curBuffIdx;
            off = 0;
            curBuffOff = sz;

            if (curBuffOff == buffSz)
            {
                buffInfo[curBuffIdx] = curBuffOff;

                curBuffOff = 0;
                curBuffIdx++;
            }
        }
        else
        {
            sz = 0;
        }

        CBuffNode *node = sz == 0 ? NULL : new CBuffNode(idx, off, sz);

        return node;
    }

    void CBuffMngr::emitDeclaration(ostream & outst)
    {
        LOG(LOG_FN_ENTRY, "CBuffMngr::emitDeclaration(%p)\n", &outst);
        for (int i = 0; i < curBuffIdx; i++)
        {
            outst << IL_PREFIX_STRING << ILConstBuff << i << "[" << byteToCBOffset(buffInfo[i]) << "]" << "\n";
        }
        //emit the last one
        outst << IL_PREFIX_STRING << ILConstBuff << curBuffIdx << "[" << byteToCBOffset(curBuffOff) << "]" << "\n";
    }

    static map < std::string, CBuffNode * >ExternConstant;
    static CBuffMngr CBuffManager;

}                               //end of namespace bril
#endif

bool hasScatterout(Decl ** argDecl, int argN)
{
    LOG(LOG_FN_ENTRY, "convertScatter(%p)\n", argDecl);

    for (int i = 0; i < argN; i++)
    {
	    Type * paramType = argDecl[i]->form;
        if ((paramType->getQualifiers() & TQ_Out) != 0 && paramType->type == TT_Array)
            return true;
    }

    return false;
}


namespace bril
{


// manipulate "name => texture info"

    RTexNode::RTexFmtInfo RTexNode::allRTexFmt[] =
    {
        {
        RTexFloat, "float"}
    };

    RTexNode::RTexNode(int i, const std::string & n, int r):id(i), name(n), rank(r)
    {
        LOG(LOG_FN_ENTRY, "RTexNode::RTexNode(%d, %s, %d)\n", i, n.c_str(), r);
        xFmt = yFmt = zFmt = wFmt = RTexFloat;
    }

    void RTexNode::emitResourceDecl(std::ostream & outs)
    {
        LOG(LOG_FN_ENTRY, "RTexNode::emitResourceDecl(%p)\n", &outs);
        int dim = (rank == 3) ? rank : 2;

        outs << IL_PREFIX_STRING << "dcl_resource_id("
            << id << ")_type("
            << dim << "d,unnorm)_fmtx("
            << allRTexFmt[xFmt].name << ")_fmty("
            << allRTexFmt[yFmt].name << ")_fmtz("
            << allRTexFmt[zFmt].name << ")_fmtw(" << allRTexFmt[wFmt].name << ")\n";
    }

    static std::map < std::string, RTexNode * >allTexture;

    RTexNode *addTexture(int i, const std::string & n, int r)
    {
        LOG(LOG_FN_ENTRY, "addTexture(%d, %s, %d)\n", i, n.c_str(), r);
        RTexNode *node = new RTexNode(i, n, r);

        std::map < std::string, RTexNode * >::iterator it = allTexture.find(n);
        assert(it == allTexture.end());
        allTexture[n] = node;

        return node;
    }

    void addTextureAndEmit(std::ostream & outs, int i, const std::string & n, int r)
    {
        LOG(LOG_FN_ENTRY, "addTextureAndEmit(%p, %d, %s, %d)\n", &outs, i, n.c_str(), r);
        RTexNode *node = addTexture(i, n, r);

        node->emitResourceDecl(outs);
    }

    int getTextureId(const std::string & n)
    {
        LOG(LOG_FN_ENTRY, "getTextureId(%s)\n", n.c_str());
        if (currentKernelInstance == NULL)
        {
            std::map < std::string, RTexNode * >::iterator it = allTexture.find(n);
            if (it == allTexture.end())
            {
                std::cerr << "problem in handling " << n << std::endl;
                assert(0);
            }
            return (*it).second->getId();
        }
        //else

        return currentKernelInstance->getSampler(n);

    }

    void clearTexture()
    {
        LOG(LOG_FN_ENTRY, "clearTexture()\n");
        for (map < std::string, RTexNode * >::iterator it = allTexture.begin(); it != allTexture.end(); it++)
        {
            delete(*it).second;
        }
        allTexture.clear();
    }


// manipulate "name => coordinate info"
//                     not every coordinate corresponds to a texture

    static std::map < std::string, int >allTexcoord;
    static void clearTexcoord(void)
    {
        LOG(LOG_FN_ENTRY, "clearTexcoord()\n");
        allTexcoord.clear();
    }

    void addTexcoordAndEmit(ostream & outs, int id, string const &name)
    {
        LOG(LOG_FN_ENTRY, "addTexcoordAndEmit(%p, %d, %s)\n", &outs, id, name.c_str());
        std::map < std::string, int >::iterator it = allTexcoord.find(name);

        assert(it == allTexcoord.end());
        allTexcoord[name] = id;

        //shader << IL_PREFIX_STRING << "dclpi_x(*)_y(*)_z(*)_w(*)_center vT" << id << "\n"; 
        outs << IL_PREFIX_STRING << "dcl_input_generic_interp(linear) v" << id << ".xy__" << "\n";
    }

    int getTexcoordId(string const &name)
    {
        LOG(LOG_FN_ENTRY, "getTexcoordId(%s)\n", name.c_str());
        std::map < std::string, int >::iterator it = allTexcoord.find(name);

        assert(it != allTexcoord.end());
        return (*it).second;
    }



    static std::map < std::string, store * >allNameStore;

    store *getNameStore(string const &name)
    {
        LOG(LOG_FN_ENTRY, "getNameStore(%s)\n", name.c_str());
        std::map < std::string, store * >::iterator it = allNameStore.find(name);
        if (it == allNameStore.end())
        {
            store *newstore = new store();

            allNameStore[name] = newstore;
            return newstore;
        }

        return (*it).second;
    }

    static void clearNameStore(void)
    {
        LOG(LOG_FN_ENTRY, "clearNameStore()\n");
        for (map < std::string, store * >::iterator it = allNameStore.begin(); it != allNameStore.end(); it++)
        {
            delete(*it).second;
        }
        allNameStore.clear();
    }


    static int nextRegisterId;
    int allocRegisterId()
    {
        LOG(LOG_FN_ENTRY, "allocRegisterId()\n");
        return nextRegisterId++;
    }



    vector < string > constant_names;

    vector < f32 > scalar_constants;
    u32 n_scalar_constants_used;

    u32 pass = 0;
    bool useGmem = false;

    void begin_shader(ostream & shader, Decl **argDecl, int argN)
    {
        LOG(LOG_FN_ENTRY, "begin_shader(%p)\n", &shader);
        shader << IL_PREFIX_STRING << "il_ps_2_0\n";

        ++pass;
        if (hasScatterout(argDecl, argN))
            useGmem = true;
        else
            useGmem = false;

        clearTexture();
        clearTexcoord();
        clearNameStore();

        currentKernelInstance = NULL;

        nextRegisterId = 0;

        constant_names.clear();

        scalar_constants.clear();

#ifdef IL_CONSTBUFFER
        assert(ExternConstant.empty());
        CBuffManager.init();
#endif

        n_scalar_constants_used = 0;
    }

    static void printHex(ostream & outs, float f)
    {
        LOG(LOG_FN_ENTRY, "printHex(%p, %g)\n", &outs, f);
        unsigned int *pi = (unsigned int *) (&f);

        outs.setf(ios::hex, ios::basefield);
        outs << "0x" << (*pi);
        outs.setf(ios::dec, ios::basefield);
    }

    void end_shader(ostream & shader)
    {
        LOG(LOG_FN_ENTRY, "end_shader(%p)\n", &shader);
        // This depends on strip_il running "reshuffle constants" so they appear before they are used.

        for (u32 i = 0; i < scalar_constants.size(); i += 4)
        {
            shader << IL_PREFIX_STRING;
#ifdef IL_LITERALCONST
            shader << ILLiteralConst;
#else
            shader << ILLocalConst;
#endif

            shader << (i >> 2) + constant_names.size();

#ifdef IL_LITERALCONST
            shader << ", ";
            printHex(shader, scalar_constants[i]);
            shader << ", ";
            printHex(shader, (i + 1 < scalar_constants.size()? scalar_constants[i + 1] : 0));
            shader << ", ";
            printHex(shader, (i + 2 < scalar_constants.size()? scalar_constants[i + 2] : 0));
            shader << ", ";
            printHex(shader, (i + 3 < scalar_constants.size()? scalar_constants[i + 3] : 0));
#else
            shader << ", "<< scalar_constants[i];
            shader << ", " << (i + 1 < scalar_constants.size()? scalar_constants[i + 1] : 0);
            shader << ", " << (i + 2 < scalar_constants.size()? scalar_constants[i + 2] : 0);
            shader << ", " << (i + 3 < scalar_constants.size()? scalar_constants[i + 3] : 0);
#endif
            shader << "\n";
        }

#ifdef IL_CONSTBUFFER
        CBuffManager.emitDeclaration(shader);

        for (map < std::string, CBuffNode * >::iterator it = ExternConstant.begin(); it != ExternConstant.end(); it++)
        {
            //emit as comment
            //shader << IL_PREFIX_STRING << ";" << ILConstBuff << as_string((*it).second) << " " << (*it).first << "\n";
            delete(*it).second;
        }
        ExternConstant.clear();
#endif

        shader << IL_PREFIX_STRING << "end\n";

        clearTexture();
        clearTexcoord();
        clearNameStore();

        nextRegisterId = 0;

        constant_names.clear();

    }



    static char swizzleName[7] = "xyzw01";

    static int getDstSwizzlePos(char c)
    {
        LOG(LOG_FN_ENTRY, "getDstSwizzlePos(%c)\n", c);
        int pos = -1;

        switch (c)
        {
        case 'x':
            pos = 0;
            break;
        case 'y':
            pos = 1;
            break;
        case 'z':
            pos = 2;
            break;
        case 'w':
            pos = 3;
            break;
        default:
            assert(0);
        }
        return pos;
    }

    static bool isNormDstSwizzle(store * dst)
    {
        LOG(LOG_FN_ENTRY, "isNormDstSwizzle(%p)\n", dst);
        return dst->x_sel == ss_x && dst->y_sel == ss_y && dst->z_sel == ss_z && dst->w_sel == ss_w;
    }

    static void getLiteralSwizzle(store * st, char swizzle[], int sz)
    {
        LOG(LOG_FN_ENTRY, "getLiteralSwizzle(%p, %p, %d)\n", st, swizzle, sz);
        assert(sz >= 1 && sz <= 4);

        swizzle[0] = swizzleName[st->x_sel];
        swizzle[1] = swizzleName[st->y_sel];
        swizzle[2] = swizzleName[st->z_sel];
        swizzle[3] = swizzleName[st->w_sel];

        swizzle[sz] = '\0';
    }


    static void implEmitFetch(ostream & out, store * s, u32 * sampler, const std::string & texposName, u32 n_cds)
    {
        LOG(LOG_FN_ENTRY, "implEmitFetch(%p, %p, %p, %s, %d)\n", &out, s, sampler, texposName.c_str(), n_cds);
        if (is_struct(s))
        {
            for (u32 i = 0; i < s->elts.size(); ++i)
                implEmitFetch(out, get_struct_elt(s, i), sampler, texposName, n_cds);
        }
        else
        {
            //out << IL_PREFIX_STRING << "texld_stage(" << *sampler << ")_shadowmode(never) " << get_gpr_name( s, rs_dst_sel ) << ", vT" << cds;
            out << IL_PREFIX_STRING << "sample_resource(" << *sampler << ")_sampler(" << *sampler << ") " <<
                get_gpr_name(s, rs_dst_sel) << ", " << texposName;

            if (n_cds <= 2)
                out << ".xy00\n";
            else
                out << ".xyz0\n";

            (*sampler)++;
        }
    }

    void emitFetch(ostream & out, store * s, u32 sampler, u32 cds, u32 n_cds)
    {
        LOG(LOG_FN_ENTRY, "emitFetch(%p, %p, %d, %d, %d)\n", &out, s, sampler, cds, n_cds);
        std::ostringstream texposName;
        texposName << "v" << cds;
        implEmitFetch(out, s, &sampler, texposName.str(), n_cds);
    }

    void emitFetch(ostream & out, store * s, u32 sampler, const std::string & texposName, u32 n_cds)
    {
	LOG(LOG_FN_ENTRY, "emitFetch(%p, %p, %d, %s, %d)\n", &out, s, sampler, texposName.c_str(), n_cds);
        implEmitFetch(out, s, &sampler, texposName, n_cds);
    }

    static void implEmitOutput(ostream & out, store * s, u32 * output)
    {
        LOG(LOG_FN_ENTRY, "implEmitOutput(%p, %p, %p)\n", &out, s, output);
        if (is_struct(s))
        {
            for (u32 i = 0; i < s->elts.size(); ++i)
                implEmitOutput(out, get_struct_elt(s, i), output);
        }
        else
        {
            if (useGmem == false)
            {
                out << IL_PREFIX_STRING << "dcl_output_generic o" << *output << "\n";
                out << IL_PREFIX_STRING << "mov o" << *output << ", " << get_gpr_name(s, rs_src_sel) << "\n";
            }

            (*output)++;
        }
    }

    void emitOutput(ostream & out, store * s, u32 output)
    {
        LOG(LOG_FN_ENTRY, "emitOutput(%p, %p, %d)\n", &out, s, output);
        implEmitOutput(out, s, &output);
    }

    void emitCopyToSwizzleDst(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitCopyToSwizzleDst(%p, %p, %p)\n", &out, dst, src);
        char swiz[6] = ".xyzw";

        swiz[dst->x_sel] = 'x';
        if (dst->size >= 2)
            swiz[dst->y_sel] = 'y';
        if (dst->size >= 3)
            swiz[dst->z_sel] = 'z';
        if (dst->size >= 4)
            swiz[dst->w_sel] = 'w';

        out << IL_PREFIX_STRING << "mov " << get_gpr_name(dst, rs_dst_sel) << ", " << get_gpr_name(src,
                                                                                                   rs_base) << swiz <<
            "\n";

    }

    string get_gpr_name(store * r, reg_style style)
    {
        LOG(LOG_FN_ENTRY, "get_gpr_name(%p, %d)\n", r, style);
        assert(r->pass == pass);

        if (r->type == st_gmem)
        {
            std::ostringstream namestr;
            namestr << "g[r" << r->idx << ".x" << "]";
            return namestr.str();
        }

        if (r->type == st_const)
        {
            u32 c = r->idx % 4;

            if (c == 0)
                return ILConstPrefix + as_string(r->idx / 4) + ".x" + (r->x_neg ? "_neg(xyzw)" : "");
            else if (c == 1)
                return ILConstPrefix + as_string(r->idx / 4) + ".y" + (r->y_neg ? "_neg(xyzw)" : "");
            else if (c == 2)
                return ILConstPrefix + as_string(r->idx / 4) + ".z" + (r->z_neg ? "_neg(xyzw)" : "");
            else
                return ILConstPrefix + as_string(r->idx / 4) + ".w" + (r->w_neg ? "_neg(xyzw)" : "");
        }

        string s = "r" + as_string(r->idx);

        if (style == rs_src_sel)
        {
            s += ".";

            if (r->size == 1)
            {
                for (int i = 0; i < 4; i++)
                    s += swizzleName[r->x_sel];

                if (r->x_neg)
                    s += "_neg(xyzw)";
            }
            else
            {
                s += swizzleName[r->x_sel];
                s += swizzleName[r->y_sel];
                if (r->size == 2)
                {
                    //overwrite the IL default component rule
                    s += swizzleName[r->x_sel];
                    s += swizzleName[r->y_sel];
                }
                else if (r->size == 3)
                {
                    s += swizzleName[r->z_sel];
                    //this is the same as the IL default component rule
                    //just make the default component explicit
                    s += swizzleName[r->x_sel];
                }
                else            //if( r->size >= 4 )
                {
                    s += swizzleName[r->z_sel];
                    s += swizzleName[r->w_sel];
                }

                if (any_negated(r))
                {
                    s += "_neg(";
                    if (r->x_neg)
                        s += 'x';
                    if (r->y_neg)
                        s += 'y';
                    if (r->z_neg)
                        s += 'z';
                    if (r->w_neg)
                        s += 'w';
                    s += ')';
                }
            }

            if (r->invert)
                s += "_invert";

            if (r->abs)
                s += "_abs";
        }
        else if (style == rs_dst_sel)
        {

            //fixme: add this later ... assert(isNormDstSwizzle(r) == true);

            s += '.';
            if (r->size >= 1)
                s += swizzleName[r->x_sel];

            if (r->size >= 2)
                s += swizzleName[r->y_sel];

            if (r->size >= 3)
                s += swizzleName[r->z_sel];

            if (r->size >= 4)
                s += swizzleName[r->w_sel];
        }

        return s;
    }

    void allocate_store(store * r, u32 size)
    {
        LOG(LOG_FN_ENTRY, "allocate_store(%p, %d)\n", r, size);
        // assert( size >= 1 && size <= 4 );
        if (r->pass != pass)
        {
            // Assume it's a gpr here - for now.
            r->type = st_gpr;
            r->idx = allocRegisterId();

            r->size = size;
            r->pass = pass;
        }
    }

    void allocate_store(store * arrayStore, store *indexStore)
    {
        LOG(LOG_FN_ENTRY, " allocate_store(%p, %d)\n", arrayStore, indexStore);
        if (arrayStore->pass != pass)
        {
            store tmpreg;
            allocate_store(&tmpreg, 4);

            arrayStore->type = st_gmem;
            arrayStore->idx = tmpreg.idx;

            arrayStore->size = indexStore->idx;
            arrayStore->pass = pass;
        }
    }

    store *alloc_struct_elt(store * r, u32 offset)
    {
        LOG(LOG_FN_ENTRY, "alloc_struct_elt(%p, %d)\n", r, offset);
        assert(!allocated(r) || r->type == st_struct);

        if (!allocated(r))
        {
            r->type = st_struct;
            r->pass = pass;
            r->elts.clear();
        }

        store dummy;

        while (r->elts.size() < offset + 1)
            r->elts.push_back(dummy);

        return &r->elts[offset];
    }

    store *get_struct_elt(store * r, u32 offset)
    {
        LOG(LOG_FN_ENTRY, "get_struct_elt(%p, %d)\n", r, offset);
        assert(r->pass == pass);
        return &r->elts[offset];
    }

// ----------------------------------------------------------------------------
// abstract emit routines

    static void emitInst(std::ostream & out, const char *insn, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitInst(%p, %s, %p, %p)\n", &out, insn, dst, src);
        if (is_standard_swizzle(dst))
            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(dst, rs_dst_sel) << ", " << get_gpr_name(src,
                                                                                                            rs_src_sel)
                << "\n";
        else
        {
            store tmp;

            allocate_store(&tmp, dst->size);

            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(&tmp, rs_dst_sel) << ", " << get_gpr_name(src,
                                                                                                             rs_src_sel)
                << "\n";
            emitCopyToSwizzleDst(out, dst, &tmp);
        }
    }

    static void emitInst(std::ostream & out, const char *insn, store * dst, store * src0, store * src1)
    {
        LOG(LOG_FN_ENTRY, "emitInst(%p, %s, %p, %p, %p)\n", &out, insn, dst, src0, src1);
        if (is_standard_swizzle(dst))
            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(dst, rs_dst_sel) << ", " << get_gpr_name(src0,
                                                                                                            rs_src_sel)
                << ", " << get_gpr_name(src1, rs_src_sel) << "\n";
        else
        {
            store tmp;

            allocate_store(&tmp, dst->size);

            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(&tmp, rs_dst_sel) << ", " << get_gpr_name(src0,
                                                                                                             rs_src_sel)
                << ", " << get_gpr_name(src1, rs_src_sel) << "\n";
            emitCopyToSwizzleDst(out, dst, &tmp);
        }
    }

    static void emitInst(std::ostream & out, const char *insn, store * dst, store * src0, store * src1, store * src2)
    {
        LOG(LOG_FN_ENTRY, "emitInst(%p, %s, %p, %p, %p)\n", &out, insn, dst, src0, src1, src2);
        if (is_standard_swizzle(dst))
            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(dst, rs_dst_sel) << ", "
                << get_gpr_name(src0, rs_src_sel) << ", "
                << get_gpr_name(src1, rs_src_sel) << ", " << get_gpr_name(src2, rs_src_sel) << "\n";
        else
        {
            store tmp;

            allocate_store(&tmp, dst->size);

            out << IL_PREFIX_STRING << insn << " " << get_gpr_name(&tmp, rs_dst_sel) << ", "
                << get_gpr_name(src0, rs_src_sel) << ", "
                << get_gpr_name(src1, rs_src_sel) << ", " << get_gpr_name(src2, rs_src_sel) << "\n";
            emitCopyToSwizzleDst(out, dst, &tmp);
        }
    }

    void emitOp(ostream & out, const char *opname, store * dst, store * src0, store * src1, store * src2)
    {
        LOG(LOG_FN_ENTRY, "emitOp(%p, %s, %p, %p, %p, %p)\n", &out, opname, dst, src0, src1, src2);
        emitInst(out, opname, dst, src0, src1, src2);
    }

    void emitOp(ostream & out, const char *opname, store * dst, store * src0, store * src1)
    {
        LOG(LOG_FN_ENTRY, "emitOp(%p, %s, %p, %p, %p)\n", &out, opname, dst, src0, src1);
        emitInst(out, opname, dst, src0, src1);
    }

    void emitOp(ostream & out, const char *opname, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitOp(%p, %s, %p, %p)\n", &out, opname, dst, src);
        emitInst(out, opname, dst, src);
    }


//fixme: use a table
    static const char *opcode2Opname(const Expression* exp, u32 op)
    {
        LOG(LOG_FN_ENTRY, "opcode2Opname(%p, %d)\n", exp, op);
        switch (op)
        {
        case BO_Plus:           // +
            if (isDouble(exp->getType()))
                return "dadd";
            else
                return "add";
            break;

        case BO_Minus:          // -
            return "sub";
            break;

        case BO_Mult:           // *
            if (isDouble(exp->getType()))
                return "dmul";
            else
                return "mul";
            break;

        case BO_Div:            // /
            if (isDouble(exp->getType()))
                return "ddiv";
            else
                return "div_zeroop(infinity)";
            break;

        case BO_Mod:            // %
            return "mod";
            break;

        case BO_And:            // &&
            return "mul";
            break;

        case BO_Or:             // ||
            return "add_sat";
            break;

        case BO_Shl:            // <<
        case BO_Shr:            // >>
        case BO_BitAnd: // &
        case BO_BitXor: // ^
        case BO_BitOr:          // |
            //      case BO_Index        // x[y]
        case BO_Assign: // An AssignExpr
        case BO_Rel:            // A RelExpr
        case BO_Comma:          // x,y
        case BO_Member: // x.y
        case BO_PtrMember:      // x->y
        default:
            //assert(0);
            semanticCheckRecord(ExpConstCast(exp), SCROperator, NULL, true);
        }

        return "bad";
    }

    void emitOp(ostream & out, const Expression* exp, u32 op, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitOp(%p, %p, %d, %p, %p)\n", &out, exp, op, dst, src);
        //fix me: use a table ...
        switch (op)
        {
        case UO_Plus:
        {
            emitInst(out, "mov", dst, src);
            break;
        }

        case UO_Minus:
        {
            store negated;

            negated = *src;
            negate(&negated);
            emitInst(out, "mov", dst, &negated);
            break;
        }
        case UO_Not:
        {
            store inverted;

            inverted = *src;
            inverted.invert = 1;
            emitInst(out, "mov", dst, &inverted);
            break;
        }

        case UO_BitNot:

        case UO_PreInc:
        case UO_PreDec:

        case UO_AddrOf:
        case UO_Deref:

        case UO_PostInc:
        case UO_PostDec:
        default:
            //fixme: the print_il is incorrectly defined as "const" method
            semanticCheckRecord(ExpConstCast(exp), SCROperator, NULL, true);
            //assert(0);
            break;

        }
    }

    void emitOp(ostream & out, const Expression* exp, u32 op, store * dst, store * src0, store * src1)
    {
        if ((op == BO_Plus || op == BO_Mult || op == BO_Div) 
            && dst->size == 4 && isDouble(exp->getType()) )
        {
            //sorry, this is really a hack to work around the fact 
            //that the SC prevent writing two double add in one IL
            //swizzle in double2 will cause trouble here
            std::string opname;
            if (op == BO_Mult)
                opname = "dmul";
            else if (op == BO_Div)
                opname = "ddiv";
            else
                opname = "dadd"; 

            //this is a hack to handle dsub ...
            bool negsrc1 = src1->x_neg;

            out << IL_PREFIX_STRING << opname << " " 
                << get_gpr_name(dst, bril::rs_base) << ".xy, " 
                << get_gpr_name(src0, bril::rs_base) << "."
                << swizzleName[src0->x_sel]
                << swizzleName[src0->y_sel] << ", "
                << get_gpr_name(src1, bril::rs_base);
            if (negsrc1)
                out << "_neg(y)";
            out << "."
                << swizzleName[src1->x_sel]
                << swizzleName[src1->y_sel] << "\n";

            out << IL_PREFIX_STRING << opname << " " 
                << get_gpr_name(dst, bril::rs_base) << ".zw, " 
                << get_gpr_name(src0, bril::rs_base) << ".";
            if (src0->size == 4)
            {
                out << swizzleName[src0->z_sel]
                    << swizzleName[src0->w_sel];
            }
            else
            {
                out << swizzleName[src0->x_sel]
                    << swizzleName[src0->y_sel];
            }
            out << ", "
                << get_gpr_name(src1, bril::rs_base);
            if (negsrc1)
                out << "_neg(y)";   //this counters intuition, but it is what SC expects.
            out << ".";
            if (src1->size == 4)
            {
                out << swizzleName[src1->z_sel]
                    << swizzleName[src1->w_sel] << "\n";
            }
            else
            {
                out << swizzleName[src1->x_sel]
                    << swizzleName[src1->y_sel] << "\n";
            }
        }
        else
            emitInst(out, opcode2Opname(exp, op), dst, src0, src1);
    }

// ----------------------------------------------------------------------------
// individual emit routines

    void emitAcos(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitAcos(%p, %p, %p)\n", &out, dst, src);
        //fixme: remove assert ...
        //the IL acos operate on the 4th channel of the src 
        //the IL "default channel expansion rule" expand the value to the 4th channel
        assert(src->size == 1);

        emitOp(out, "acos", dst, src);
    }

    void emitAny(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitAny(%p, %p, %p)\n", &out, dst, src);
        //this is the implementation of any4 -- 4 compoment any

        u32 zero = 0x0;         //0
        f32 fzero = *(f32 *) & zero;

        store szero;

        get_scalar_constant_store(&szero, fzero);

        store temp1;

        allocate_store(&temp1, 4);

        out << IL_PREFIX_STRING << "ior "
            << get_gpr_name(&temp1, bril::rs_base) << ".xy__"
            << ", " << get_gpr_name(src, bril::rs_base) << ".xyxx"
            << ", " << get_gpr_name(src, bril::rs_base) << ".zwxx\n";

        out << IL_PREFIX_STRING << "ior "
            << get_gpr_name(&temp1, bril::rs_base) << ".x___"
            << ", " << get_gpr_name(&temp1, bril::rs_base) << ".x"
            << ", " << get_gpr_name(&temp1, bril::rs_base) << ".y\n";

        out << IL_PREFIX_STRING << "ine "
            << get_gpr_name(dst, bril::rs_base) << ".xyzw"
            << ", " << get_gpr_name(&temp1, bril::rs_base) << ".xxxx"
            << ", " << get_gpr_name(&szero, bril::rs_src_sel) << "\n";
    }

    void emitClamp(ostream & out, store * dst, store * val, store * minval, store * maxval)
    {
        LOG(LOG_FN_ENTRY, "emitClamp(%p, %p, %p, %p, %p)\n", &out, dst, val, minval, maxval);
        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitOp(out, "max", &tmp1, val, minval);
        emitOp(out, "min", dst, &tmp1, maxval);
    }

/*
 * Handle the "mismatch literal form" between Brook src code and IL instruction.
 *   Example
 *   Brook src code : a.wx = b.xy
 *   il code        : a.x__w = b.yxxx;
 */
    static void emitNormCopy(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitNormCopy(%p, %p, %p)\n", &out, dst, src);

        int sz = dst->size;

        char literalDst[5];

        getLiteralSwizzle(dst, literalDst, sz);

        char literalSrc[5];

        getLiteralSwizzle(src, literalSrc, sz);

        char ilDst[6] = ".____";
        char ilSrc[6] = ".xxxx";

        for (int i = 0; i < sz; i++)
        {
            char cDst = literalDst[i];
            int pos = getDstSwizzlePos(cDst);

            ilDst[pos + 1] = cDst;
            ilSrc[pos + 1] = literalSrc[i];
        }

        out << IL_PREFIX_STRING << "mov "
            << get_gpr_name(dst, bril::rs_base) << ilDst << ", " << get_gpr_name(src, bril::rs_base) << ilSrc << "\n";

    }

    void emitCopy(ostream & out, store * dst, store * src, bool reqDstNorm)
    {
        LOG(LOG_FN_ENTRY, "emitCopy(%p, %p, %p, %d)\n", &out, dst, src, reqDstNorm);
        if (is_struct(dst))
        {
            //fixme: remove assert ...
            assert(is_struct(src) && dst->elts.size() == src->elts.size());
            for (u32 i = 0; i < dst->elts.size(); ++i)
                emitCopy(out, &dst->elts[i], &src->elts[i], reqDstNorm);
        }
        else
        {
            if (reqDstNorm && isNormDstSwizzle(dst) == false)
                emitNormCopy(out, dst, src);
            else
                emitInst(out, "mov", dst, src);
        }
    }

    void emitDot(ostream & out, store * dst, store * src0, store * src1)
    {
        LOG(LOG_FN_ENTRY, "emitDot(%p, %p, %p, %p)\n", &out, dst, src0, src1);
        char opname[] = "dp0";
        int sz0 = src0->size;
        int sz1 = src1->size;

        assert(sz0 >= 1 && sz0 <= 4);
        assert(sz1 >= 1 && sz1 <= 4);

        opname[2] = (sz0 >= sz1 ? sz0 : sz1) + '0';

        emitOp(out, opname, dst, src0, src1);
    }

    void emitExp(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitExp(%p, %p, %p)\n", &out, dst, src);
        u32 c1 = 0x3FB8AA3B;    //1.442695
        f32 fc1 = *(f32 *) & c1;

        store sc1;

        get_scalar_constant_store(&sc1, fc1);

        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitOp(out, "mul", &tmp1, src, &sc1);
        emitOp(out, "exp_vec", dst, &tmp1);
    }

    void emitFmod(ostream & outs, store * dst, store * src0, store * src1)
    {
        LOG(LOG_FN_ENTRY, "emitFmod(%p, %p, %p, %p)\n", &outs, dst, src0, src1);
        if (false)//if(globals.fastfmod)
        {
            store tmp1;
            allocate_store(&tmp1, dst->size);
            emitOp(outs, "div_zeroop(infinity)", &tmp1, src0, src1);

            store tmp2;
            allocate_store(&tmp2, dst->size);

            //todo: emitOp(outs, "ge", &tmp2, &tmp1, &tmp1? neg
            outs << IL_PREFIX_STRING << "ge "
                << get_gpr_name(&tmp2, rs_dst_sel) << ", "
                << get_gpr_name(&tmp1, rs_src_sel) << ", " << get_gpr_name(&tmp1, rs_base) << "_neg(xyzw)\n";

            //emitOp(outs, "frc", &tmp1, &tmp1? abs
            outs << IL_PREFIX_STRING << "frc "
                << get_gpr_name(&tmp1, rs_dst_sel) << ", " << get_gpr_name(&tmp1, rs_base) << "_abs\n";

            //emitOp(outs, "cmov_logical", &tmp1, &tmp2, tmp1, tmp1? neg
            outs << IL_PREFIX_STRING << "cmov_logical "
                << get_gpr_name(&tmp1, rs_dst_sel) << ", "
                << get_gpr_name(&tmp2, rs_src_sel) << ", "
                << get_gpr_name(&tmp1, rs_src_sel) << ", " << get_gpr_name(&tmp1, rs_base) << "_neg(xyzw)\n";

            emitOp(outs, "mul", dst, &tmp1, src1);
        }
        else
            emitOp(outs, "mod", dst, src0, src1);

    }

/*
 isfinite(x): (x & 0x7f800000) != 0x7f800000
*/
    void emitIsfinite(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitIsfinite(%p, %p, %p)\n", &out, dst, src);
        u32 c1 = 0x7F800000;
        f32 fc1 = *(f32 *) & c1;

        u32 c2 = 0x3F800000;
        f32 fc2 = *(f32 *) & c2;

        store sc1, sc2;

        get_scalar_constant_store(&sc1, fc1);
        get_scalar_constant_store(&sc2, fc2);

        store tmp1, tmp2;

        allocate_store(&tmp1, dst->size);
        allocate_store(&tmp2, dst->size);

        emitOp(out, "and", &tmp1, src, &sc1);
        emitOp(out, "ine", &tmp2, &tmp1, &sc1);
        emitOp(out, "and", dst, &tmp2, &sc2);
    }

/*
 *  isinf(x): (x & 0x7fffffff) == 0x7f800000 
*/
    void emitIsinf(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitIsinf(%p, %p, %p)\n", &out, dst, src);
        u32 c1 = 0x7Fffffff;
        f32 fc1 = *(f32 *) & c1;

        u32 c2 = 0x7F800000;
        f32 fc2 = *(f32 *) & c2;

        u32 c3 = 0x3F800000;
        f32 fc3 = *(f32 *) & c3;

        store sc1, sc2, sc3;

        get_scalar_constant_store(&sc1, fc1);
        get_scalar_constant_store(&sc2, fc2);
        get_scalar_constant_store(&sc3, fc3);

        store tmp1, tmp2;

        allocate_store(&tmp1, dst->size);
        allocate_store(&tmp2, dst->size);

        emitOp(out, "and", &tmp1, src, &sc1);
        emitOp(out, "ieq", &tmp2, &tmp1, &sc2);
        emitOp(out, "and", dst, &tmp2, &sc3);
    }

/*
 *  isnan(x): (x & 0x7fffffff != 0x7f800000) && (x & 0x7f800000 == 0x7f800000)
 *  or: isinf(x) == false && (x & 0x7f800000 == 0x7f800000)
*/
    void emitIsnan(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitIsnan(%p, %p, %p)\n", &out, dst, src);
        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitIsinf(out, &tmp1, src);

        store tmp2;

        allocate_store(&tmp2, dst->size);

        u32 c1 = 0x00000000;
        f32 fc1 = *(f32 *) & c1;

        store sc1;

        get_scalar_constant_store(&sc1, fc1);

        emitOp(out, "ieq", &tmp2, &tmp1, &sc1);

        u32 c2 = 0x7F800000;
        f32 fc2 = *(f32 *) & c2;

        u32 c3 = 0x3F800000;
        f32 fc3 = *(f32 *) & c3;

        store sc2, sc3;

        get_scalar_constant_store(&sc2, fc2);
        get_scalar_constant_store(&sc3, fc3);

        store tmp3, tmp4, tmp5;

        allocate_store(&tmp3, dst->size);
        allocate_store(&tmp4, dst->size);
        allocate_store(&tmp5, dst->size);

        emitOp(out, "and", &tmp3, src, &sc2);
        emitOp(out, "ieq", &tmp4, &tmp3, &sc2);

        emitOp(out, "and", &tmp5, &tmp2, &tmp4);
        emitOp(out, "and", dst, &tmp5, &sc3);
    }

    void emitLerp(ostream & out, store * dst, store * src0, store * src1, store * src2)
    {
        LOG(LOG_FN_ENTRY, "emitLerp(%p, %p, %p, %p, %p)\n", &out, dst, src0, src1, src2);
        u32 c1 = 0x3F317218;    //0.6931471825
        f32 fc1 = *(f32 *) & c1;

        store sc1;

        get_scalar_constant_store(&sc1, fc1);

        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitOp(out, "sub", &tmp1, src1, src0);
        emitOp(out, "mad", dst, src2, &tmp1, src0);
    }

    void emitLog(ostream & out, store * dst, store * src)
    {
        LOG(LOG_FN_ENTRY, "emitLog(%p, %p, %p)\n", &out, dst, src);
        u32 c1 = 0x3F317218;    //0.6931471825
        f32 fc1 = *(f32 *) & c1;

        store sc1;

        get_scalar_constant_store(&sc1, fc1);

        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitOp(out, "log_vec", &tmp1, src);
        emitOp(out, "mul", dst, &tmp1, &sc1);
    }

    void emitPow(ostream & out, store * dst, store * src0, store * src1)
    {
        LOG(LOG_FN_ENTRY, "emitPow(%p, %p, %p, %p)\n", &out, dst, src0, src1);
        store tmp1;

        allocate_store(&tmp1, dst->size);

        emitOp(out, "log_zeroop(fltmax)", &tmp1, src0);
        emitOp(out, "mul", &tmp1, &tmp1, src1);
        emitOp(out, "exp", dst, &tmp1);
    }

    void emitRound(ostream & out, store * dst, store * prm)
    {
        LOG(LOG_FN_ENTRY, "emitRound(%p, %p, %p)\n", &out, dst, prm);
        // Determine if fmod is really floating point mod operation, or if it's more c-cast-to-int-like...
        store tmp;

        allocate_store(&tmp, dst->size);

        store half;

        get_scalar_constant_store(&half, 0.5);

        emitOp(out, "add", &tmp, prm, &half);
        emitOp(out, "flr", dst, &tmp);
    }

    void emitd2f(ostream & out, store * dst, store * src)
    {
        emitOp(out, "d2f", dst, src);
    }

    void emitf2d(ostream & out, store * dst, store * src)
    {
        emitOp(out, "f2d", dst, src);
    }
// ----------------------------------------------------------------------------
// use table to drive the codegen for built-in routines

    typedef void (*CodegenFunc_1) (ostream & out, store * dst, store * src);
    typedef void (*CodegenFunc_2) (ostream & out, store * dst, store * src0, store * src1);
    typedef void (*CodegenFunc_3) (ostream & out, store * dst, store * src0, store * src1, store * src2);

#define VOIDCAST(t) ((void*)t)

    typedef struct opInfo
    {
        const char *name;
        const char *ilName;
        void *cgFunc;
        int srcTot;
    } OpInfo;

    typedef std::map < std::string, OpInfo * >OpinfoMap;
    typedef std::map < std::string, OpInfo * >::iterator OpinfoIter;

    static OpinfoMap allBuiltin;

    static OpInfo builtinTable[] = {
        {"abs", "abs", NULL, 1},
        {"acos", NULL, VOIDCAST(emitAcos), 1},
        {"clamp", NULL, VOIDCAST(emitClamp), 3},
        {"cos", "cos_vec", NULL, 1},
        {"cross", "crs", NULL, 2},
        {"dot", NULL, VOIDCAST(emitDot), 2},
        {"exp", NULL, VOIDCAST(emitExp), 1},
        {"floor", "flr", NULL, 1},
        {"fmod", NULL, VOIDCAST(emitFmod), 2},
        {"frac", "frc", NULL, 1},
        {"isfinite", NULL, VOIDCAST(emitIsfinite), 1},
        {"isinf", NULL, VOIDCAST(emitIsinf), 1},
        {"isnan", NULL, VOIDCAST(emitIsnan), 1},
        {"lerp", NULL, VOIDCAST(emitLerp), 3},
        {"log", NULL, VOIDCAST(emitLog), 1},
        {"max", "max", NULL, 2},
        {"min", "min", NULL, 2},
        {"normalize", "nrm_zeroop(fltmax)", NULL, 1},
        {"pow", NULL, VOIDCAST(emitPow), 2},
        {"rsqrt", "rsq_vec", NULL, 1},
        {"round", NULL, VOIDCAST(emitRound), 1},
        {"sign", "sgn", NULL, 1},
        {"sin", "sin_vec", NULL, 1},
        {"sqrt", "sqrt_vec", NULL, 1},
        {NULL, NULL, NULL, 0}
    };

/**
 *  Initialize the il code gen data structures.
 */
    void Init()
    {
        LOG(LOG_FN_ENTRY, "Init()\n");
        /* 
         * built the (name, opinfo) map for the table 
         * that contains all the builtin routines
         */
        int i = 0;

        while (builtinTable[i].name != NULL)
        {
            //integrity check
            //this is to assert internal problem, ok to assert here
            assert(builtinTable[i].ilName == NULL || builtinTable[i].cgFunc == NULL);
            assert(builtinTable[i].srcTot >= 0);

            std::string name(builtinTable[i].name);
            OpinfoIter it = allBuiltin.find(name);

            if (it != allBuiltin.end())
            {
                //this is to assert internal problem, ok to assert here
                std::cerr << name << " redefined builtin\n";
                assert(0);
            }

            allBuiltin[name] = &builtinTable[i];
            i++;
        }
    }

/** 
 * return false if the corresponding routine is not found in the builtin table
 */
    bool emitBuiltin(ostream & out, const FunctionCall * funcall)
    {
        LOG(LOG_FN_ENTRY, "emitBuiltin(%p, %p)\n", &out, funcall);
        Variable *v = static_cast < Variable * >(funcall->function);

        std::string name(v->name->name);
        OpinfoIter it = allBuiltin.find(name);

        if (it == allBuiltin.end())
            return false;

        OpInfo *opinfo = (*it).second;

        assert(opinfo->srcTot == funcall->nArgs());

        if (opinfo->ilName != NULL)
        {
            if (opinfo->srcTot == 1)
            {
                emitOp(out, opinfo->ilName, funcall->get_il_store(), funcall->args[0]->get_il_store());
            }
            else if (opinfo->srcTot == 2)
            {
                emitOp(out, opinfo->ilName, funcall->get_il_store(),
                       funcall->args[0]->get_il_store(), funcall->args[1]->get_il_store());
            }
            else
                semanticCheckRecord(FuncConstCast(funcall), SCROther, NULL, true);      //not implemented yet
        }
        else
        {
            if (opinfo->srcTot == 1)
            {
                ((CodegenFunc_1) opinfo->cgFunc) (out, funcall->get_il_store(), funcall->args[0]->get_il_store());
            }
            else if (opinfo->srcTot == 2)
            {
                ((CodegenFunc_2) opinfo->cgFunc) (out,
                                                  funcall->get_il_store(),
                                                  funcall->args[0]->get_il_store(), funcall->args[1]->get_il_store());
            }
            else if (opinfo->srcTot == 3)
            {
                ((CodegenFunc_3) opinfo->cgFunc) (out,
                                                  funcall->get_il_store(),
                                                  funcall->args[0]->get_il_store(),
                                                  funcall->args[1]->get_il_store(), funcall->args[2]->get_il_store());
            }
            else
                semanticCheckRecord(FuncConstCast(funcall), SCROther, NULL, true);      //not implemented yet
        }

        return true;
    }

// ---------------------------------------------------------------------------------------------
// Constants

    static u32 find_scalar_constant(f32 x)
    {
        LOG(LOG_FN_ENTRY, "find_scalar_constant(%g)\n", x);
        //this is to assert internal problem, ok to assert here
        assert(n_scalar_constants_used >= constant_names.size() * 4);
        for (u32 i = 0; i < scalar_constants.size(); ++i)
            if (FPEQ(scalar_constants[i], x))
                return i + (u32) constant_names.size() * 4;

        u32 r = (u32) (scalar_constants.size() + constant_names.size() * 4);

        scalar_constants.push_back(x);
        ++n_scalar_constants_used;
        return r;
    }

    string get_scalar_constant_name(f32 x)
    {
        LOG(LOG_FN_ENTRY, "get_scalar_constant_name(%g)\n", x);
        if (FPEQ(x, 0.f))
            return "r0.000";

        static char const components[4][2] = { "x", "y", "z", "w" };

        u32 idx = find_scalar_constant(x);

        return ILConstPrefix + as_string(idx >> 2) + "." + components[idx & 3];
    }

    void get_scalar_constant_store(store * store, f32 x)
    {
        LOG(LOG_FN_ENTRY, "get_scalar_constant_store(%p, %g)\n", store, x);
        if (FPEQ(x, 0.f) || FPEQ(x, 1.f))
        {
            store->pass = pass;
            store->type = st_gpr;
            store->idx = 0;
            store->size = 1;
            if (FPEQ(x, 0.f))
                store->x_sel = ss_0;
            else
                store->x_sel = ss_1;
        }
        else if (store->pass != pass)
        {
            u32 idx = find_scalar_constant(x);

            store->pass = pass;
            store->type = st_const;
            store->idx = idx;
            store->size = 1;
        }
    }

#ifdef IL_CONSTBUFFER
//todo: retire parameter "idx"
//todo: handle variable with size > 16 bytes correctly
//
    void name_extern_constant(u32 idx, string const &name)
    {
        LOG(LOG_FN_ENTRY, "name_extern_constant(%d, %s)\n", idx, name.c_str());
        std::map < std::string, CBuffNode * >::iterator it = ExternConstant.find(name);

        //this is to assert internal problem, ok to assert here
        assert(it == ExternConstant.end());

        CBuffNode *node = CBuffManager.allocate(16);    //4 float = 16 bytes

        //this is to assert internal problem, ok to assert here
        assert(node != NULL);

        ExternConstant[name] = node;
    }
#else
    void name_extern_constant(u32 idx, string const &name)
    {
        LOG(LOG_FN_ENTRY, "name_extern_constant(%d, %s)\n", idx, name.c_str());
        assert(n_scalar_constants_used == constant_names.size() * 4);
        while (int (constant_names.size()) < idx + 1)
            constant_names.push_back("");
        constant_names[idx] = name;
        n_scalar_constants_used = max(n_scalar_constants_used, 4 * (idx + 1));
    }
#endif

#ifdef IL_CONSTBUFFER
    std::string get_extern_constant(string const &name)
    {
        LOG(LOG_FN_ENTRY, "get_extern_constant(%s)\n", name.c_str());
        map < string, CBuffNode * >::iterator it = ExternConstant.find(name);
        if (it == ExternConstant.end())
        {
            std::cerr << "Problem with handling " << name << std::endl;
            assert(0);
        }

        return as_string((*it).second);
    }
#else
    std::string get_extern_constant(string const &name)
    {
        LOG(LOG_FN_ENTRY, "get_extern_constant(%s)\n", name.c_str());
        for (unsigned int i = 0; i < constant_names.size(); ++i)
        {
            if (constant_names[i] == name)
            {
                return "c" + as_string(i);
            }
        }

        assert(0);
        return as_string(0);

    }
#if 0
    u32 get_extern_constant(string const &name)
    {
        LOG(LOG_FN_ENTRY, "get_extern_constant(%s)\n", name.c_str());
        for (unsigned int i = 0; i < constant_names.size(); ++i)
            if (constant_names[i] == name)
                return i;

        assert(0);
        return 0;
    }
#endif
#endif

    static selects encode_swizzle(char c)
    {
        LOG(LOG_FN_ENTRY, "encode_swizzle(%c)\n", c);
        switch (c)
        {
        case 'x':
            return ss_x;
        case 'y':
            return ss_y;
        case 'z':
            return ss_z;
        case 'w':
            return ss_w;
        case '0':
            return ss_0;
        case '1':
            return ss_1;
        default:
            return ss_unknown;
        }

        return ss_x;            //to please the compiler
    }

    CGResult set_swizzle_float(store * rs, string const &swiz)
    {
        LOG(LOG_FN_ENTRY, "set_swizzle_float(%p, %s)\n", rs, swiz.c_str());
        assert(swiz.length() >= 1);
        rs->x_sel = encode_swizzle(swiz[0]);
        if (rs->x_sel == ss_unknown)
            return CGResultBad;

        if (swiz.length() >= 2)
        {
            rs->y_sel = encode_swizzle(swiz[1]);
            if (rs->y_sel == ss_unknown)
                return CGResultBad;
        }
        if (swiz.length() >= 3)
        {
            rs->z_sel = encode_swizzle(swiz[2]);
            if (rs->z_sel == ss_unknown)
                return CGResultBad;
        }
        if (swiz.length() >= 4)
        {
            rs->w_sel = encode_swizzle(swiz[3]);
            if (rs->w_sel == ss_unknown)
                return CGResultBad;
        }

        return CGResultOK;

    }

    CGResult set_swizzle_double(store * rs, string const &swiz)
    {
        LOG(LOG_FN_ENTRY, "set_swizzle_double(%p, %s)\n", rs, swiz.c_str());
        assert(swiz.length() >= 1);
        rs->x_sel = encode_swizzle(swiz[0]);
        if (rs->x_sel == ss_unknown)
            return CGResultBad;
        else if (rs->x_sel == ss_x)
            rs->y_sel = ss_y;
        else if (rs->x_sel == ss_y)
        {
            rs->x_sel = ss_z;
            rs->y_sel = ss_w;
        }

        if (swiz.length() >= 2)
        {
            rs->z_sel = encode_swizzle(swiz[1]);
            if (rs->z_sel == ss_unknown)
                return CGResultBad;
            else if (rs->z_sel == ss_x)
                rs->w_sel = ss_y;
            else if (rs->z_sel == ss_y)
            {
                rs->z_sel = ss_z;
                rs->w_sel = ss_w;
            }
        }

        return CGResultOK;

    }

}                               // close namespace bil


// manipulate clone copy of IL kernel subroutine
// May need to generate multiple instances of functions that take samplers as parameters

void SamplerParamInfo::insert(std::string & name, int s)
{
    LOG(LOG_FN_ENTRY, "SamplerParamInfo::insert(%s, %d)\n", name.c_str(), s);
    SamplerParamInfoIter it = find(name);

    assert(it == end());
    (*this)[name] = s;
}

void SamplerParamInfo::print(std::ostream & shader) const
{
    LOG(LOG_FN_ENTRY, "SamplerParamInfo::print(%p)\n", &shader);
    SamplerParamInfoIter it;

    for (it = begin(); it != end(); it++)
    {
        shader << (*it).first << " = s" << (*it).second << ", ";
    }
}

bool KernelInstance::match(const SamplerParamInfo & src)
{
    LOG(LOG_FN_ENTRY, "KernelInstance::match(%p)\n", &src);
    if (src.size() == samplerParam.size())
    {
        SamplerParamInfoIter it;

        for (it = samplerParam.begin(); it != samplerParam.end(); it++)
        {
            SamplerParamInfoIter srcit = src.find((*it).first);

            if (srcit == src.end() || (*srcit).second != (*it).second)
                return false;
        }
        return true;
    }

    return false;
}

int KernelInstance::getSampler(const std::string & n)
{
    LOG(LOG_FN_ENTRY, "KernelInstance::getSampler(%p)\n", &n);
    SamplerParamInfoIter it = samplerParam.find(n);

    //this is to assert internal problem, ok to assert here
    assert(it != samplerParam.end());
    return (*it).second;
}

KernelInstance *currentKernelInstance;


