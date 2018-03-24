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
 * generate_il.h --
 *
 *      Generate IL from shader string
 */
#ifndef INC_GENERATE_IL_H
#define INC_GENERATE_IL_H
#define BROOK_TO_IL_CODE_GEN_PATH 0
#define IL_PREFIX_STRING "  // IL: "
// Comment strings don't seem to work, so put in an HLSL comment.
#define IL_COMMENT_STRING "  // IL# "

//use dcl_literal
#define IL_LITERALCONST

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <map>

#include "logger.hpp"
#include "types.h"


//todo: create a new source file for semantic check

#define ExpConstCast(a) const_cast<Expression*>(a)
#define FuncConstCast(a) const_cast<FunctionCall*>(a)
#define AssignConstCast(a) const_cast<AssignExpr*>(a)


typedef enum
{
    SCRArrayaccess = 0,
    SCRCallIndirect,
    SCRCallCallee,
    SCRCallParamnum,
    SCRCallParammatch,
    SCRConstantType,
    SCRDataType,
    SCRReduction,
    SCRVariableDefine,
    SCRVectorSwizzle,
    SCROperator,
    SCROther,
    SCRTotal
} SCResult;

typedef enum
{
    CGResultOK = 0,
    CGResultBad
} CGResult;

extern bool semanticCheckOK;
class Expression;
class Decl;
extern void semanticCheckRecord(Expression * exp, SCResult failCode, const char *msg = NULL, bool isCodeGen = false);
extern void semanticCheckRecord(Decl* dcl, SCResult failCode);
extern void semanticCheckRecord(const char* msg, bool isCodeGen=false);



class FunctionCall;
class FunctionType;

inline std::string as_string(s32 x)
{
    std::ostringstream tmp;
    tmp << x;
    return tmp.str();
}

inline std::string as_string(u32 x)
{
    std::ostringstream tmp;
    tmp << x;
    return tmp.str();
}
#ifdef BROOK_TO_IL_CODE_GEN_PATH
#define IL_CONSTBUFFER
#ifdef IL_CONSTBUFFER

namespace bril
{

//! constant buffer storage
    class CBuffNode
    {
      public:
        CBuffNode(int i, int o, int s);

        int Index()
        {
            return index;
        }
        int ByteOffset()
        {
            return byteOff;
        }
        int ByteSize()
        {
            return byteSz;
        }

      private:
        int index;              //byte
        int byteOff;
        int byteSz;
    };

    inline CBuffNode::CBuffNode(int i, int o, int s)
    {
        index = i;
        byteOff = o;
        byteSz = s;
    };

//! constant buffer manager
    class CBuffMngr
    {
#define byteToCBOffset(b) ((b + 15)/16)
      public:
        CBuffMngr(int tot = 15, int sz = 4096);
           ~CBuffMngr();

        void init();

        CBuffNode *allocate(int sz);
        void emitDeclaration(std::ostream & outst);

      private:
        int totBuff;            //total number of buffer
        int buffSz;             //buffer size in byte
        int curBuffIdx;         //current buffer index
        int curBuffOff;         //the offset of the next available byte in the current buffer
        int *buffInfo;          //the size of each buffer
    };

    inline CBuffMngr::CBuffMngr(int tot, int sz)
    {
        LOG(LOG_FN_ENTRY, " CBuffMngr::CBuffMngr(%d, %d)\n", tot, sz);
        totBuff = tot;
        buffSz = sz;
        buffInfo = new int[totBuff];

        init();
    }

    inline CBuffMngr::~CBuffMngr()
    {
        LOG(LOG_FN_ENTRY, " CBuffMngr::~CBuffMngr()\n");
        free(buffInfo);
    }

    inline void CBuffMngr::init()
    {
        LOG(LOG_FN_ENTRY, " CBuffMngr::init()\n");
        curBuffIdx = 0;
        curBuffOff = 0;

        for (int i = 0; i < totBuff; i++)
            buffInfo[i] = 0;

    }

}                               //end of namespace
#endif
#endif // BROOK_TO_IL_CODE_GEN_PATH
#ifdef IL_CONSTBUFFER
inline std::string as_string(bril::CBuffNode * node)
{
    std::ostringstream tmp;
    tmp << "cb" << node->Index() << "[" << byteToCBOffset(node->ByteOffset()) << "]";
    return tmp.str();
}
#endif
#ifdef BROOK_TO_IL_CODE_GEN_PATH

namespace bril
{

    using namespace std;

    const std::string ILLocalConst = "def c";
    const std::string ILConstBuff = "dcl_cb cb";
    const std::string ILLiteralConst = "dcl_literal l";

#ifdef IL_LITERALCONST
    const char ILConstPrefix = 'l';
#else
    const char ILConstPrefix = 'c';
#endif


// manipulate "name => texture info"

//! Resource texture node
    class RTexNode
    {
      public:
        //format for texture element
        enum RTexFmt
        {
            RTexFloat = 0,
            //more ...
        };

        typedef struct RTexFmtInfo_
        {
            RTexFmt fmt;
            std::string name;
        } RTexFmtInfo;

        static RTexFmtInfo allRTexFmt[];

      public:
        RTexNode(int i, const std::string &, int r);

        int getId()
        {
            return id;
        }
        int getRank()
        {
            return rank;
        }
        std::string & getName()
        {
            return name;
        }

        void emitResourceDecl(std::ostream &);

      private:
        int id;

        std::string name;
        int rank;
        RTexFmt xFmt;           // x element format
        RTexFmt yFmt;
        RTexFmt zFmt;
        RTexFmt wFmt;
    };

    RTexNode *addTexture(int i, const std::string &, int r);
    void addTextureAndEmit(std::ostream &, int i, const std::string &, int r);
    int getTextureId(const std::string &);
    void clearTexture(void);


// texcoord

    void addTexcoordAndEmit(ostream &, int, string const &);
    int getTexcoordId(string const &);



    // This is basically for setting swizzles on IL register strings
    enum reg_style
    {
        rs_dst_sel,
        rs_src_sel,
        rs_base,
    };

    enum store_type
    {
        st_gpr,
        st_const,
        st_struct,
        st_gmem,
    };

    enum selects
    {
        ss_x,
        ss_y,
        ss_z,
        ss_w,
        ss_0,
        ss_1,
        ss_unknown
    };

    // Pass is used to force reallocation of storage in symbol table entries,
    // which don't get freed between code generation passes.
    // I could have register freeing on store destruction - but I want multiple
    // stores to be able to refer to the same registers.

    extern u32 pass;
    extern bool useGmem;

    struct store
    {
        inline store()
        {
            pass = 0;
            x_sel = ss_x;
            y_sel = ss_y;
            z_sel = ss_z;
            w_sel = ss_w;
            x_neg = 0;
            y_neg = 0;
            z_neg = 0;
            w_neg = 0;
            abs = 0;
            invert = 0;
        }

        u32 pass;
        store_type type;

        u32 idx;
        u32 size;

        u32 x_sel:3;
        u32 y_sel:3;
        u32 z_sel:3;
        u32 w_sel:3;

        u32 x_neg:1;
        u32 y_neg:1;
        u32 z_neg:1;
        u32 w_neg:1;

        u32 abs:1;
        u32 invert:1;

        vector < store > elts;
    };

    inline bool allocated(store * s)
    {
        return s->pass == pass;
    }

    inline bool is_struct(store * rs)
    {
        return rs->type == st_struct;
    }

    inline bool is_standard_swizzle(store * rs)
    {
        if (is_struct(rs))
            return false;

        return rs->x_sel == ss_x && rs->y_sel == ss_y && rs->z_sel == ss_z && rs->w_sel == ss_w;
    }

    CGResult set_swizzle_double(store * rs, string const &swiz);
    CGResult set_swizzle_float(store * rs, string const &swiz);

    inline void negate(store * rs)
    {
        rs->x_neg = 1 - rs->x_neg;
        rs->y_neg = 1 - rs->y_neg;
        rs->z_neg = 1 - rs->z_neg;
        rs->w_neg = 1 - rs->w_neg;
    }

    inline bool any_negated(store * rs)
    {
        return rs->x_neg || rs->y_neg || rs->z_neg || rs->w_neg;
    }

    string get_gpr_name(store * r, reg_style style);

    void begin_shader(ostream &, Decl **, int);
    void end_shader(ostream &);

    void Init(void);

    // Named resources are for Brook's main shader routine.
    // Rather than re-writing that code generation completely, the simplest approach
    // is to allocate and index by HLSL names.

    void name_extern_constant(u32 idx, string const &name);
    std::string get_extern_constant(string const &name);

    // For structures; named gpr should go away.
    store *getNameStore(string const &);

    u32 get_named_gpr(string const &);

    void get_scalar_constant_store(store * r, f32 x);
    void allocate_store(store * regs, u32 size);
    void allocate_store(store *, store *);

    bool emitBuiltin(ostream & out, const FunctionCall *);

    void emitOp(ostream & out, const Expression*, u32 op, store * dst, store * src);
    void emitOp(ostream & out, const Expression*, u32 op, store * dst, store * src0, store * src1);

    void emitOp(ostream & out, const char *opname, store * dst, store * src0, store * src1, store * src2);
    void emitOp(ostream & out, const char *opname, store * dst, store * src0, store * src1);
    void emitOp(ostream & out, const char *opname, store * dst, store * src);

    void emitAcos(ostream & out, store * dst, store * src);
    void emitAny(ostream & out,  store * dst, store * src);
    void emitClamp(ostream & out, store * dst, store * src0, store * src1, store * src2);
    void emitCopy(ostream & out, store * dst, store * src, bool reqNormDst = false);
    void emitDot(ostream & out, store * dst, store * src0, store * src1);
    void emitExp(ostream & out, store * dst, store * src);
    void emitFetch(ostream & out, store * s, u32 sampler, u32 cds, u32 n_cds);
    void emitFetch(ostream & out, store * s, u32 sampler, const std::string & texposName, u32 n_cds);
    void emitFmod(ostream & out, store * dst, store * lft, store * rgt);
    void emitIsfinite(ostream & out, store * dst, store * src);
    void emitIsinf(ostream & out, store * dst, store * src);
    void emitIsnan(ostream & out, store * dst, store * src);
    void emitLerp(ostream & out, store * dst, store * src0, store * src1, store * src2);
    void emitLog(ostream & out, store * dst, store * src);
    void emitOutput(ostream & out, store * s, u32 output);
    void emitPow(ostream & out, store * dst, store * s0, store * s1);
    void emitRound(ostream & out, store * dst, store * prm);
    void emitd2f(ostream & out, store * dst, store * prm);
    void emitf2d(ostream & out, store * dst, store * prm);

    // For the "real" code generation; size is number of scalars -
    // packing is assumed to be very loose (DX10, basically).

    store *get_struct_elt(store * regs, u32 offset);
    store *alloc_struct_elt(store * regs, u32 offset);

    // String naming register and component that holds a scalar.
    // Will allocate if the value is not found.
    string get_scalar_constant_name(f32 x);

}



typedef std::map < std::string, int >::const_iterator SamplerParamInfoIter;

//typedef std::map<std::string, int> SamplerParamInfo;
class SamplerParamInfo:public std::map < std::string, int >
{
  public:
    void insert(std::string & name, int s);
    void print(std::ostream & shader) const;
};

class KernelInstance
{
  public:
    KernelInstance():gen(false)
    {
        LOG(LOG_FN_ENTRY, " KernelInstance::KernelInstance()\n");
    }

    KernelInstance(SamplerParamInfo const &p, int i):samplerParam(p)
    {
        LOG(LOG_FN_ENTRY, " KernelInstance::KernelInstance(%p, %d)\n", &p, i);
        gen = false;
        id = i;
    }

    bool match(const SamplerParamInfo & src);
    int getSampler(const std::string & n);

    void printSampler(std::ostream & outs) const
    {
        samplerParam.print(outs);
    }

    int getId()
    {
        return id;
    }
    bool generated()
    {
        return gen;
    }
    void setGenerated()
    {
        gen = true;
    }

  private:
    bool gen;
    int id;

    // (formal, actual-sampler) mapping
    SamplerParamInfo samplerParam;
};

extern KernelInstance *currentKernelInstance;


#endif //BROOK_TO_IL_CODE_GEN_PATH


#endif // INC_GENERATE_IL_H
