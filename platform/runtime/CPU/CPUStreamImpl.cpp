/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Stanford University nor the names of any contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/
#include "CPUStreamImpl.h"
#include "Utility.h"

namespace brt
{
    using namespace brook;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    //! \param rank Rank of the Buffer
    //! \param dimensions dimension of the Buffer
    //! \param dataFormat Stream data fromat
    //! \param count The total number of members if it is a structure
    //! \param device by default device is 0 for CPU backend
    //!
    ////////////////////////////////////////////////////////////////////////////////

    CPUStreamImpl::CPUStreamImpl(unsigned short rank, unsigned int* dimensions, 
                                 BRformat* dataFormats, unsigned int count, BaseDevice* device, 
                                 Device* execDevices, unsigned int deviceCount)
                        :StreamImpl(rank, dimensions, dataFormats, count, device, 
                                    execDevices, deviceCount)
    {
        StreamType* inFieldTypes = new StreamType[count];
        unsigned int* cpuStreamDimensions = new unsigned int[rank];

        for(unsigned int i = 0; i < rank; i++)
        {
            cpuStreamDimensions[i] = dimensions[rank - i - 1];
        }

        for(unsigned int i = 0; i < count; i++)
        {
            switch(dataFormats[i])
            {
                case BR_FORMAT_FLOAT_1:
                    inFieldTypes[i] = __BRTFLOAT; 
                    break;

                case BR_FORMAT_FLOAT_2:
                    inFieldTypes[i] = __BRTFLOAT2;
                    break;

                case BR_FORMAT_FLOAT_3:
                    inFieldTypes[i] = __BRTFLOAT3;
                    break;

                case BR_FORMAT_FLOAT_4:
                    inFieldTypes[i] = __BRTFLOAT4;
                    break;

                case BR_FORMAT_CHAR_1:
                    inFieldTypes[i] = __BRTCHAR;
                    break;

                case BR_FORMAT_CHAR_2:
                    inFieldTypes[i] = __BRTCHAR2;
                    break;

                case BR_FORMAT_CHAR_3:
                    inFieldTypes[i] = __BRTCHAR3;
                    break;

                case BR_FORMAT_CHAR_4:
                    inFieldTypes[i] = __BRTCHAR4;
                    break;

                case BR_FORMAT_UCHAR_1:
                    inFieldTypes[i] = __BRTUCHAR;
                    break;

                case BR_FORMAT_UCHAR_2:
                    inFieldTypes[i] = __BRTUCHAR2;
                    break;

                case BR_FORMAT_UCHAR_3:
                    inFieldTypes[i] = __BRTUCHAR3;
                    break;

                case BR_FORMAT_UCHAR_4:
                    inFieldTypes[i] = __BRTUCHAR4;
                    break;
                
                case BR_FORMAT_SHORT_1:
                    inFieldTypes[i] = __BRTSHORT;
                    break;

                case BR_FORMAT_SHORT_2:
                    inFieldTypes[i] = __BRTSHORT2;
                    break;

                case BR_FORMAT_SHORT_3:
                    inFieldTypes[i] = __BRTSHORT3;
                    break;

                case BR_FORMAT_SHORT_4:
                    inFieldTypes[i] = __BRTSHORT4;
                    break;

                case BR_FORMAT_USHORT_1:
                    inFieldTypes[i] = __BRTUSHORT;
                    break;

                case BR_FORMAT_USHORT_2:
                    inFieldTypes[i] = __BRTUSHORT2;
                    break;

                case BR_FORMAT_USHORT_3:
                    inFieldTypes[i] = __BRTUSHORT3;
                    break;

                case BR_FORMAT_USHORT_4:
                    inFieldTypes[i] = __BRTUSHORT4;
                    break;

                case BR_FORMAT_UINT_1:
                    inFieldTypes[i] = __BRTUINT;
                    break;

                case BR_FORMAT_UINT_2:
                    inFieldTypes[i] = __BRTUINT2;
                    break;

                case BR_FORMAT_UINT_3:
                    inFieldTypes[i] = __BRTUINT3;
                    break;

                case BR_FORMAT_UINT_4:
                    inFieldTypes[i] = __BRTUINT4;
                    break;

                case BR_FORMAT_INT_1:
                    inFieldTypes[i] = __BRTINT;
                    break;

                case BR_FORMAT_INT_2:
                    inFieldTypes[i] = __BRTINT2;
                    break;

                case BR_FORMAT_INT_3:
                    inFieldTypes[i] = __BRTINT3;
                    break;

                case BR_FORMAT_INT_4:
                    inFieldTypes[i] = __BRTINT4;
                    break;

                case BR_FORMAT_DOUBLE_1:
                    inFieldTypes[i] = __BRTDOUBLE;
                    break;

                case BR_FORMAT_DOUBLE_2:
                    inFieldTypes[i] = __BRTDOUBLE2;
                    break;
            }
        }

        _cpuStream = new CPUStream(count, inFieldTypes, rank, cpuStreamDimensions);
        
        delete [] cpuStreamDimensions;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Implementation of Stream.Read()
    //!
    //! \param ptr Application pointer to copy
    //!
    ////////////////////////////////////////////////////////////////////////////////
    
    void 
    CPUStreamImpl::read(const void *ptr, const char* flags)
    {
        _cpuStream->Read(ptr);
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Implementation of Stream.write()
    //!
    //! \param ptr Application pointer to copy
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    CPUStreamImpl::write(void* ptr, const char* flags)
    {
        _cpuStream->Write(ptr); 
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief copy data from source stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    CPUStreamImpl::copyDataFrom(StreamImpl* src)
    {
        unsigned int size = 1;
        for(unsigned int i = 0; i < _bufferCount; ++i)
        {
            size *= Utility::getElementBytes(_format[i]);
        }

        for(unsigned int i = 0; i < _rank; ++i)
        {
            size *= _dimensions[i];
        }

        // Currently, this is the best way to do it given the read/write 
        // implementation of CPU backend
        char* data = new char[size];
        src->write(data);
        read(data);
        delete[] data;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief copy data to destination stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    CPUStreamImpl::copyDataTo(StreamImpl* dst)
    {
        unsigned int size = 1;
        for(unsigned int i = 0; i < _bufferCount; ++i)
        {
            size *= Utility::getElementBytes(_format[i]);
        }

        for(unsigned int i = 0; i < _rank; ++i)
        {
            size *= _dimensions[i];
        }

        // Currently, this is the best way to do it given the read/write 
        // implementation of CPU backend
        char* data = new char[size];
        write(data);
        dst->read(data);
        delete[] data;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Implementation of Stream.domain()
    //!
    //! \param start Pointer pointing to start of the dimensions
    //! \param end Pointer pointing to end of the dimensions
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamImpl*
    CPUStreamImpl::createDomainStream(unsigned int* start, unsigned int* end)
    {
        switch(this->getRank())
        {
            case 1:
            {
                int min = (int)start[0];
                int max = (int)end[0];
                unsigned int dimensions[] = {max - min};
                CPUStreamImpl* cpuStreamImpl = new CPUStreamImpl(_rank, dimensions, _format,
                                                                 _bufferCount, _device, _execDevices, 
                                                                 _execDeviceCount);
                cpuStreamImpl->_parent = this;
                cpuStreamImpl->_cpuStream = this->_cpuStream->Domain(min, max);
                
                return cpuStreamImpl;
            }
            case 2:
            {
                int2 min = int2(start[0], start[1]);
                int2 max = int2(end[0], end[1]);
                unsigned int dimensions[2];
                
                dimensions[0] = end[0] - start[0];
                dimensions[1] = end[1] - start[1];
                CPUStreamImpl* cpuStreamImpl = new CPUStreamImpl(_rank, dimensions, _format,
                                                                 _bufferCount, _device, _execDevices, 
                                                                 _execDeviceCount);
                cpuStreamImpl->_parent = this;
                cpuStreamImpl->_cpuStream = this->_cpuStream->Domain(min, max);
                
                return cpuStreamImpl;
            }
            case 3:
            {
                int3 min = int3(start[0], start[1], start[2]);
                int3 max = int3(end[0], end[1], end[2]);
                unsigned int dimensions[3];
                
                dimensions[0] = end[0] - start[0];
                dimensions[1] = end[1] - start[1];
                dimensions[2] = end[2] - start[2];
                CPUStreamImpl* cpuStreamImpl = new CPUStreamImpl(_rank, dimensions, _format,
                                                                 _bufferCount, _device, _execDevices, 
                                                                 _execDeviceCount);
                cpuStreamImpl->_parent = this;
                cpuStreamImpl->_cpuStream = this->_cpuStream->Domain(min, max);
                
                return cpuStreamImpl;
            }
            case 4:
            {
                int4 min = int4(start[0], start[1], start[2], start[3]);
                int4 max = int4(end[0], end[1], end[2], end[3]);
                unsigned int dimensions[4];
                
                dimensions[0] = end[0] - start[0];
                dimensions[1] = end[1] - start[1];
                dimensions[2] = end[2] - start[2];
                dimensions[3] = end[3] - start[3];
                CPUStreamImpl* cpuStreamImpl = new CPUStreamImpl(_rank, dimensions, _format,
                                                                 _bufferCount, _device, _execDevices, 
                                                                 _execDeviceCount);
                cpuStreamImpl->_parent = this;
                cpuStreamImpl->_cpuStream = this->_cpuStream->Domain(min, max);
                
                return cpuStreamImpl;
            }
        }

        return NULL;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Implementation of Stream.execDomain()
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamImpl*
    CPUStreamImpl:: execDomain(int numThreads)
    {
        CPUStreamImpl* cpuStreamImpl = new CPUStreamImpl(_rank, _dimensions, _format,
                                                         _bufferCount, _device, _execDevices, 
                                                         _execDeviceCount);
        cpuStreamImpl->_cpuStream = this->_cpuStream->ExecDomain(numThreads);
               
        return cpuStreamImpl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    CPUStreamImpl::~CPUStreamImpl()
    {
        delete _cpuStream;
    }
}
