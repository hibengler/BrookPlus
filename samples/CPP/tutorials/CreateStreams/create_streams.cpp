////////////////////////////////////////////////////////////////////////////////
//!
//! \file   create_streams.cpp
//!
//! \brief  Creates 1D, 2D and 3D streams
//!
////////////////////////////////////////////////////////////////////////////////

// Third-party headers
#include "brook/Stream.h"

int
main(int argc, char* argv[])
{
    // Specifying the sizes of different streams
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int depth = 64;

    unsigned int streamSize1D[] = {width};
    unsigned int streamSize2D[] = {width, height};
    unsigned int streamSize3D[] = {width, height, depth};

    // Specifying the ranks of different streams
    unsigned int rank1 = 1;
    unsigned int rank2 = 2;
    unsigned int rank3 = 3;

    // Create a 1D stream of specified size i.e. 64 floating-point values
    fprintf(stdout, "Creating 1D stream\n");
    brook::Stream<float> X(rank1, streamSize1D);

    // Create a 2D stream of specified size i.e. 64x64 floating-point values
    fprintf(stdout, "Creating 2D stream\n");
    brook::Stream<float> Y(rank2, streamSize2D);

    // Create a 3D stream of specified size i.e. 64x64x64 floating-point values
    fprintf(stdout, "Creating 3D stream\n");
    brook::Stream<float> Z(rank3, streamSize3D);
    
    return 0;
}
