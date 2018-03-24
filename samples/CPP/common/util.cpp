#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>

#include "util.h"

unsigned int padWidth  = 0;
unsigned int byteWidth = 0;
unsigned int datasize  = 0;
const short BITMAP_MAGIC_NUMBER = 19778;
const int RGB_BYTE_SIZE = 3;
const unsigned int Offset = 54;

namespace Util
{
    // Value functions
    template<>
    void 
    value(float2& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(float3& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(float4& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
        value(var.w, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(int2& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(int3& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(int4& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
        value(var.w, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(uint2& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(uint3& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(uint4& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
        value(var.z, x, y, z, width, height, depth, bound, type);
        value(var.w, x, y, z, width, height, depth, bound, type);
    }

    template<>
    void 
    value(double2& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        value(var.x, x, y, z, width, height, depth, bound, type);
        value(var.y, x, y, z, width, height, depth, bound, type);
    }    

    // Compare functions
    template <>
    bool
    compare(const float val0, const float val1)
    {
        const float epsilon = (float)1e-2;
        float diff = (val1 - val0);
        if (fabs(val1) > epsilon)
        {
            diff /= val0;            
        }

        return (fabs(diff) > epsilon);
    }

    template <>
    bool
    compare(const float2 val0, const float2 val1)
    {
        return (compare(val0.x, val1.x) && compare(val0.y,val1.y));
    }

    template <>
    bool
    compare(const float3 val0, const float3 val1)
    {
        return (compare(val0.x, val1.x) && 
                compare(val0.y,val1.y) &&
                compare(val0.z, val1.z));
    }

    template <>
    bool
    compare(const float4 val0, const float4 val1)
    {
        return (compare(val0.x, val1.x) && 
                compare(val0.y,val1.y) &&
                compare(val0.z, val1.z) &&
                compare(val0.w, val1.w));
    }

    template <>
    bool
    compare(const double val0, const double val1)
    {
        const double epsilon = (double)1e-2;
        double diff = (val1 - val0);
        if (fabs(val1) > epsilon)
        {
            diff /= val0;            
        }

        return (fabs(diff) > epsilon);
    }

    template <>
    bool
    compare(const double2 val0, const double2 val1)
    {
        return (compare(val0.x, val1.x) && compare(val0.y,val1.y));
    }


    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  
    //!
    //! \param  width                   Width of the buffer
    //! \param  height                  Height of the buffer
    //! \param  depth                   Depth of the buffer
    //! \param  operationsPerElement    Number of arithmetic operations per element
    //! \param  time                    Time taken to complete all operations
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    calculatePerformance(const unsigned int width, const unsigned int height, unsigned int depth,
                            const unsigned int operationsPerElement, 
                            double time)
    {
        double numElements = width * height * depth;
        double totalOperations = numElements * operationsPerElement;
        double operationsPerSecond = totalOperations / (double)time;
        double gigaOps =  operationsPerSecond * (double)1e-09;
        std::cout   << "Time    = " << time << " seconds" << std::endl
                    << "GIGAOPS = " << gigaOps << std::endl << std::endl;
    }

    // Print functions
    template<>
    void
    print(const float value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << value << "\t";
    }

    template<>
    void
    print(const float2 value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << "(" << value.x << "," << value.y << ")" << "\t";
    }

    template<>
    void
    print(const float3 value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << "(" << value.x << "," << value.y << ","  << value.z << ")" 
                    << "\t";
    }

    template<>
    void
    print(const float4 value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << "(" << value.x << "," << value.y << "," 
                    << value.z << "," << value.z << ")" << "\t";
    }

    template<>
    void
    print(const double value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << value << "\t";
    }

    template<>
    void
    print(const double2 value)
    {
        std::cout   << std::setiosflags(std::ios::fixed) << std::setprecision(6)
                    << "(" << value.x << "," << value.y << ")" << "\t";
    }

    template<>
    void
    print(const int2 value)
    {
        std::cout << "(" << value.x << "," << value.y << ")" << "\t";
    }

    template<>
    void
    print(const int3 value)
    {
        std::cout   << "(" << value.x << "," << value.y << "," << value.y << ")" 
                    << "\t";
    }

    template<>
    void
    print(const int4 value)
    {
        std::cout   << "(" << value.x << "," << value.y << "," 
                    << value.z << "," << value.z << ")" << "\t";
    }

    template<>
    void
    print(const uint2 value)
    {
        std::cout << "(" << value.x << "," << value.y << ")" << "\t";
    }

    template<>
    void
    print(const uint3 value)
    {
        std::cout   << "(" << value.x << "," << value.y << "," << value.y << ")" 
                    << "\t";
    }

    template<>
    void
    print(const uint4 value)
    {
        std::cout   << "(" << value.x << "," << value.y << "," 
                    << value.z << "," << value.z << ")" << "\t";
    }    

    void normalizeMatrix(float4* matrix, unsigned int height, unsigned int width, float min, float max)
    {
        float tmin = (float)INT_MAX;
        float tmax = (float)INT_MIN;
        unsigned int size = height * width;
        float4* ptr = matrix;
        do {
            tmax = (ptr->x > tmax) ? ptr->x : tmax;
            tmin = (ptr->x < tmin) ? ptr->x : tmin;
            tmax = (ptr->y > tmax) ? ptr->y : tmax;
            tmin = (ptr->y < tmin) ? ptr->y : tmin;
            tmax = (ptr->z > tmax) ? ptr->z : tmax;
            tmin = (ptr->z < tmin) ? ptr->z : tmin;
            tmax = (ptr->w > tmax) ? ptr->w : tmax;
            tmin = (ptr->w < tmin) ? ptr->w : tmin;
        }while(++ptr < (matrix + size));
        ptr = matrix;
        do {
            ptr->x = ((((ptr->x - tmin) * (max - min)) / (tmax - tmin)) + min);
            ptr->y = ((((ptr->y - tmin) * (max - min)) / (tmax - tmin)) + min);
            ptr->z = ((((ptr->z - tmin) * (max - min)) / (tmax - tmin)) + min);
            ptr->w = ((((ptr->w - tmin) * (max - min)) / (tmax - tmin)) + min);
        }while(++ptr < (matrix + size));
    }

    float4* convert8(unsigned char* data, unsigned int height, unsigned int width, BRRGBQUAD* colors)
    {
        float4* image;
        unsigned int offset, diff;
        diff = width * height * RGB_BYTE_SIZE;
        if (!(image = allocateBuffer<float4>(width, height)))
        {
            printf("Unable to allocate image matrix data.\n");
            return NULL;
        }

        if (height > 0)
        {
            unsigned int j = 0;
            offset = padWidth - byteWidth;
            for (unsigned int i = 0; i < datasize * RGB_BYTE_SIZE; i += 3)
            {
                if ((i + 1) % padWidth == 0)
                {
                    i += offset;
                }
                (*(image + i)).x = (float)colors[*(data + j)].rgbRed;
                (*(image + i)).y = (float)colors[*(data + j)].rgbGreen;
                (*(image + i)).z = (float)colors[*(data + j)].rgbBlue;
                j++;
            }
        }
        else
        {
            unsigned int j = datasize - 1;
            offset = padWidth - byteWidth;
            for (unsigned int i = 0; i < datasize * RGB_BYTE_SIZE; i += 3)
            {
                if ((i + 1) % padWidth == 0)
                {
                    i += offset;
                }
                (*(image + i)).x = (float)colors[*(data + j)].rgbRed;
                (*(image + i)).y = (float)colors[*(data + j)].rgbGreen;
                (*(image + i)).z = (float)colors[*(data + j)].rgbBlue;
                j--;
            }
        }
        normalizeMatrix(image, width, height, 0.0, 1.0);
        return image;
    }

    float4* convert24(unsigned char* data, unsigned int height, unsigned int width)
    {
        float4* image;
        unsigned int offset, diff;
        diff = width * height * RGB_BYTE_SIZE;
        if (!(image = allocateBuffer<float4>(width, height)))
        {
            printf("Unable to allocate image matrix data.\n");
            return NULL;
        }
        if (height > 0)
        {
            int j = 0;
            offset = padWidth - byteWidth;
            for (unsigned int i = 0; i < datasize; i += 3)
            {
                if (((i + 1) % padWidth) == 0)
                {
                    i += offset;
                }
                (*(image + j)).x = (*(data + i + 2));
                (*(image + j)).y = (*(data + i + 1));
                (*(image + j)).z = (*(data + i + 0));
                ++j;
            }
        }
        else
        {
            int j;
            offset = padWidth - byteWidth;
            j = datasize / 3;
            for (unsigned int i = 0; i < datasize; i += 3)
            {
                if (((i + 1) % padWidth) == 0)
                {
                    i += offset;
                }
                (*(image + j)).x = (*(data + i + 2));
                (*(image + j)).y = (*(data + i + 1));
                (*(image + j)).z = (*(data + i + 0));
                --j;
            }
        }
        normalizeMatrix(image, height, width, 0.0, 1.0);
        return image;
    }

    float4* convert16(unsigned char* data, unsigned int height, unsigned int width)
    {
        return NULL;
    }

    float4* convert32(unsigned char* data, unsigned int height, unsigned int width)
    {
        return NULL;
    }


    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Opens the image file and returns corresponding data and dimensions
    //!
    //! \param  filename    Name of image file to open
    //! \param  width       Width of the image
    //! \param  height      Height of the image
    //!
    ////////////////////////////////////////////////////////////////////////////////

    float4* 
    openImage(std::string filename, unsigned int& width, unsigned int& height)
    {
        float4* image   = NULL;
        FILE *fid       = NULL;
        unsigned char *tempData  = NULL;
        BRRGBQUAD* colors = NULL;
        unsigned int numColors = 0;
        BRBITMAPFILEHEADER bmfh = {0, 0, 0, 0, 0};
        BRBITMAPINFOHEADER bmih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        if (!(fid = fopen(filename.c_str(), "rb")))
        {
            printf("Error opening file: %s\n", filename.c_str());
            return NULL;
        }
        fread(&bmfh.bfType, 2, 1, fid);
        fread(&bmfh.bfSize, 4, 1, fid);
        fread(&bmfh.bfReserved1, 2, 1, fid);
        fread(&bmfh.bfReserved2, 2, 1, fid);
        fread(&bmfh.bfOffBits, 4, 1, fid);
        if (bmfh.bfType != BITMAP_MAGIC_NUMBER)
        {
            printf("File is not in DIB format\n");
            goto exit;
        }
        fread(&bmih, sizeof(bmih), 1, fid);
        height = bmih.biHeight;
        width = bmih.biWidth;
        numColors = 1 << bmih.biBitCount;
        datasize = width * height * (bmih.biBitCount >> 3);
        if (bmih.biBitCount == 8)
        {
            colors = (BRRGBQUAD*)malloc(sizeof*colors * numColors);
            if (!(colors))
            {
                printf("Error allocating space for color data\n");
                goto exit;
            }
            fread(colors, sizeof*colors, numColors, fid);
        }

        if (!(tempData = (unsigned char*)malloc(sizeof *tempData * datasize)))
        {
            printf("Error allocating temp data space\n");
            goto exit;
        }

        fread(tempData, sizeof *tempData, datasize, fid);

        byteWidth = padWidth = (width * bmih.biBitCount) >> 3;

        while((padWidth % 4) != 0) padWidth++;
        switch(bmih.biBitCount)
        {
            case 8:
                image = convert8(tempData, height, width, colors);
                break;
            case 16:
                printf("Does not support 16 bit images yet.\n");
                break;
            case 24:
                image = convert24(tempData, height, width);
                break;
            case 32:
                printf("Does not support 32 bit images yet.\n");
                break;
        }

        exit:
            if (tempData)
            {
                free(tempData);
            }
            if (colors)
            {
                free(colors);
            }
            if (fid)
            {
                fclose(fid);
            }
            return image;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Saves the image to the provided filename
    //! 
    //! \param  filename    Name of image file to save the data as
    //! \param  data        Color data
    //! \param  height      Height of the image
    //! \param  width       Width of the image    
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void 
    saveImage(std::string filename, float4* data, unsigned int width, unsigned int height)
    {
        BRBITMAPFILEHEADER bmfh = {0, 0, 0, 0, 0};
        BRBITMAPINFOHEADER bmih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        unsigned int  offset, diff;
        FILE *fid = NULL;
        bmih.biWidth = width;
        bmih.biHeight = height;
        bmih.biPlanes = 1;
        bmih.biBitCount = 24;
        bmih.biSize = 40;
        bmih.biXPelsPerMeter = 2835;
        bmih.biYPelsPerMeter = 2835;
        bmfh.bfType = BITMAP_MAGIC_NUMBER;
        bmfh.bfOffBits = Offset;
        bmfh.bfSize = (width * height * 3) + 40 + 14;
        if (!(fid = fopen(filename.c_str(), "wb")))
        {
            printf("Error creating file: %s\n", filename.c_str());
            return;
        }
        fwrite(&bmfh.bfType, 2, 1, fid);
        fwrite(&bmfh.bfSize, 4, 1, fid);
        fwrite(&bmfh.bfReserved1, 2, 1, fid);
        fwrite(&bmfh.bfReserved2, 2, 1, fid);
        fwrite(&bmfh.bfOffBits, 4, 1, fid);
        fwrite(&bmih, sizeof(bmih), 1, fid);
        diff = width * height * RGB_BYTE_SIZE;
        byteWidth = padWidth = (width * bmih.biBitCount) >> 3;
        datasize = (byteWidth * height);
        normalizeMatrix(data, height, width, 0.0, 255.0);
        while((padWidth % 4) != 0) padWidth++;
        if (height > 0)
        {
            unsigned int j = 0;
            offset = padWidth - byteWidth;
            for (unsigned int i = 0; i < datasize; i += 3)
            {
                if (((i + 1) % padWidth) == 0)
                {
                    i += offset;
                }
                fprintf(fid,"%c",(char)((int)(data[j].z)));
                fprintf(fid,"%c",(char)((int)(data[j].y)));
                fprintf(fid,"%c",(char)((int)(data[j].x)));
                ++j;
            }
        }
        else
        {
            unsigned int j = diff / 3;
            offset = padWidth - byteWidth;
            for (unsigned int i = 0; i < datasize; i += 3)
            {
                if (((i + 1) % padWidth) == 0)
                {
                    i += offset;
                }
                fprintf(fid,"%c",(char)((int)(data[j].z)));
                fprintf(fid,"%c",(char)((int)(data[j].y)));
                fprintf(fid,"%c",(char)((int)(data[j].x)));
                --j;
            }
        }

        fclose(fid);
    }
}
