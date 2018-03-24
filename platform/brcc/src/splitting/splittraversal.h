/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splittraversal.h
#ifndef INC_SPLITTRAVERSAL_H
#define INC_SPLITTRAVERSAL_H
#ifdef _WIN32
#pragma warning(disable:4786)
//the above warning disables visual studio's annoying habit of warning when using the standard set lib
#endif

#include <iostream>
#include <vector>
#include <set>

class SplitNode;

class SplitNodeTraversal
{
  public:
    void operator() (SplitNode * inNode)
    {
        traverse(inNode);
    }

    void operator() (const std::vector < SplitNode * >&inNodes)
    {
        for (std::vector < SplitNode * >::const_iterator i = inNodes.begin(); i != inNodes.end(); ++i)
            traverse(*i);
    }

    void operator() (const std::set < SplitNode * >&inNodes)
    {
        for (std::set < SplitNode * >::const_iterator i = inNodes.begin(); i != inNodes.end(); ++i)
            traverse(*i);
    }

    virtual void traverse(SplitNode * inNode) = 0;

    void traverseGraphChildren(SplitNode * inNode);

    virtual ~ SplitNodeTraversal()
    {
    }
};

class SplitArgumentCounter
{
  public:
    SplitArgumentCounter():samplerCount(0), constantCount(0), texcoordCount(0), outputCount(0)
    {
    }

    int samplerCount;
    int constantCount;
    int texcoordCount;
    int outputCount;
};

class SplitArgumentTraversal: public SplitNodeTraversal
{
  public:
    SplitArgumentTraversal(std::ostream & inStream, SplitNode * inOutputPosition):stream(inStream),
        outputPosition(inOutputPosition), hasOutput(false)
    {
    }

    virtual ~ SplitArgumentTraversal()
    {
    }
    void traverse(SplitNode * inNode);

  private:
    SplitArgumentCounter argumentCounter;
    std::ostream & stream;
    SplitNode *outputPosition;
    bool hasOutput;
};

class SplitStatementTraversal: public SplitNodeTraversal
{
  public:
    SplitStatementTraversal(std::ostream & inStream, SplitNode * inOutputPosition):stream(inStream),
        outputPosition(inOutputPosition)
    {
    }
    virtual ~ SplitStatementTraversal()
    {
    }
    void traverse(SplitNode * inNode);

  private:
    std::ostream & stream;
    SplitNode *outputPosition;
};

class SplitAnnotationTraversal: public SplitNodeTraversal
{
  public:
    SplitAnnotationTraversal(std::ostream & inStream, SplitNode * inOutputPosition):stream(inStream),
        outputPosition(inOutputPosition)
    {
    }
    virtual ~ SplitAnnotationTraversal()
    {
    }
    void traverse(SplitNode * inNode);

  private:
    std::ostream & stream;
    SplitNode *outputPosition;
};

#endif // INC_SPLITTRAVERSAL_H
