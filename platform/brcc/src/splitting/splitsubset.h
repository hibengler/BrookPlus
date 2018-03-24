/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splitsubset.h
#ifndef INC_SPLITSUBSET_H
#define INC_SPLITSUBSET_H

#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

#include <vector>

class SplitSubsetGenerator
{
  public:
    SplitSubsetGenerator(size_t inSubsetSize, size_t inSetSize):_subsetSize(inSubsetSize), _setSize(inSetSize)
    {
        _end = 0;
        const size_t one = 1;
        for    (size_t i = 0; i < _subsetSize; i++)
                   _end |= one << (i + (_setSize - _subsetSize));
               _current = 0xFFFFFFFF;
    }

    bool hasMore()
    {
        return (_current != _end);
    }

    size_t getNext()
    {
        if (_current == 0xFFFFFFFF)
        {
            for (size_t i = 0; i < _subsetSize; i++)
                _state.push_back(i);
        }
        else
        {
            // we need to advance things a tiny bit...
            for (size_t i = 0; i < _subsetSize; i++)
            {
                // if we can't advance this one, look higher
                if ((i < _subsetSize - 1) && _state[i] + 1 == _state[i + 1])
                    continue;

                // advance this one and reset everybody below
                _state[i]++;
                for (size_t j = 0; j < i; j++)
                    _state[j] = j;
                break;
            }
        }

        _current = 0;
        const size_t one = 1;

        for (size_t i = 0; i < _subsetSize; i++)
        {
            _current |= one << _state[i];
        }
        return _current;
    }

  private:
    size_t _subsetSize;
    size_t _setSize;

    size_t _current;
    size_t _end;

    std::vector < size_t > _state;
};

class SplitSubsetGenerator2
{
  public:
    SplitSubsetGenerator2(size_t inSubsetSize, size_t inSetSize):_subsetSize(inSubsetSize), _setSize(inSetSize)
    {
        _initialized = false;
        _done = false;
    }

    bool hasMore()
    {
        return !_done;
    }

    size_t getIndexedValue(size_t inIndex)
    {
        return _state[inIndex];
    }

    void getNext()
    {
        if (!_initialized)
        {
            _initialized = true;

            for (size_t i = 0; i < _subsetSize; i++)
                _state.push_back(i);

            _done = (_subsetSize == 0) || (_subsetSize == _setSize);
        }
        else
        {
            // we need to advance things a tiny bit...
            for (size_t i = 0; i < _subsetSize; i++)
            {
                // if we can't advance this one, look higher
                if ((i < _subsetSize - 1) && _state[i] + 1 == _state[i + 1])
                    continue;

                // advance this one and reset everybody below
                _state[i]++;
                for (size_t j = 0; j < i; j++)
                    _state[j] = j;
                break;
            }

            _done = (_state[0] == _setSize - _subsetSize);
        }
    }

  private:
    bool _initialized;
    bool _done;

    size_t _subsetSize;
    size_t _setSize;

    std::vector < size_t > _state;
};

#endif // INC_SPLITSUBSET_H
