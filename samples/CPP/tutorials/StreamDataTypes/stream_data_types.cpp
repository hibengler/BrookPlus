////////////////////////////////////////////////////////////////////////////////
//!
//! \file   stream_data_types.cpp
//!
//! \brief  Creates 2D streams of different types
//!
//! The stream types are as follows:-
//!     # float, float2, float3 and float4
//!     # double, double2
//!     # int, int2, int3, int4
//!     # uint, uint2, uint3, uint4
//!
////////////////////////////////////////////////////////////////////////////////

// Third-party headers
#include "brook/Stream.h"

using namespace brook;

int
main(int argc, char* argv[])
{
    // Specifying the sizes of different streams
    unsigned int streamSize1D[] = {64};
    unsigned int streamSize2D[] = {64, 64};
    unsigned int streamSize3D[] = {64, 64, 64};

    // Specifying the ranks of different streams
    unsigned int rank1 = 1;
    unsigned int rank2 = 2;
    unsigned int rank3 = 3;

    // Create a 1D stream of specified size i.e. 64 floating-point values
    fprintf(stdout, "Creating 1D streams of types float, double2, int3 and uint4\n");
    Stream<float>   X1(rank1, streamSize1D);
    Stream<double2> X2(rank1, streamSize1D);
    Stream<int3>    X3(rank1, streamSize1D);
    Stream<uint4>   X4(rank1, streamSize1D);

    // Create a 2D stream of specified size i.e. 64x64 floating-point values
    fprintf(stdout, "Creating 2D streams of types float, double2, int3\n");
    Stream<float>   Y1(rank2, streamSize2D);
    Stream<double2> Y2(rank2, streamSize2D);
    Stream<int3>    Y3(rank2, streamSize2D);

    // Create a 3D stream of specified size i.e. 64x64x64 floating-point values
    fprintf(stdout, "Creating 3D streams of types float, double2, int3, uint4\n");
    Stream<float>   Z1(rank3, streamSize3D);
    Stream<double2> Z2(rank3, streamSize3D);
    Stream<int3>    Z3(rank3, streamSize3D);
    Stream<uint4>   Z4(rank3, streamSize3D);

    return 0;
}
