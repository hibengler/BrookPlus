/****************************************************************************

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Advanced Micro Devices, Inc nor the names of its contributors
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

#include "MonteCarlo.h"
#include "CPUThread.h"
#include "brookgenfiles/AsianPrice.h"
#include "brook/Device.h"

// Declaring global data so that it can be accessed from different threads
unsigned int Steps = 10;
float InitialPrice = 50;
float StrikePrice = 55;
float Maturity = 1;
float Interest = 0.06f;
float FinalValue = 0.8f;

float* Sigma = NULL;
float* Price = NULL;
float* Vega = NULL;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

MonteCarlo::MonteCarlo(char* name) : SampleBase(name)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Does the command line parsing for the application
//! 
////////////////////////////////////////////////////////////////////////////////

void
MonteCarlo::ParseCommandLine(int argc, char** argv)
{
    info->Quiet = DEFAULT_QUIET;
    info->Timing = DEFAULT_TIMING;

    for (int x = 1; x < argc; ++x) 
    {
        unsigned int i = 0;
        switch (argv[x][1])
        {
            case 's':
                sscanf(argv[++x],"%u",&Steps);
                break;
            case 'p':
                sscanf(argv[++x],"%f",&InitialPrice);
                break;
            case 'r':
                sscanf(argv[++x],"%f",&StrikePrice);
                break;
            case 'i':
                sscanf(argv[++x],"%f",&Interest);
                break;
            case 'm':
                sscanf(argv[++x],"%f",&Maturity);
                break;
            case 'q':
                info->Quiet = 1;
                break;
            case 't':
                info->Timing = 1;
                break;
            case 'h':
                Usage(argv[0]);
                break;
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Default usage menu for all samples
//! 
////////////////////////////////////////////////////////////////////////////////

void
MonteCarlo::Usage(char *name)
{
    fprintf(stdout,
            "Usage : %s [OPTIONS]                                                                   \n\n"
            "   -s <integer>,                                                                       \n"
            "       Steps for which to run the monte-carlo simulation (Default = 10, Maximum = 256) \n\n"
            "   -p <float>,                                                                         \n"
            "       Initial Price (Default = 50)                                                    \n\n"
            "   -r <float>,                                                                         \n"
            "       Strike Price (Default = 55)                                                     \n\n"
            "   -i <float>,                                                                         \n"
            "       Interest (Default = 0.06)                                                       \n\n"
            "   -m <float>,                                                                         \n"
            "       Maturity (Default = 1)                                                          \n\n"
            "   -q  Surpress all data output                                                        \n"
            "   -t  Print out timing information                                               \n"
            "   -h  Displays this help                                                              \n\n",
            name);

    exit(0);

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
MonteCarlo::allocateBuffer()
{
    // Allocate buffers
    Sigma    = new float[Steps];
    Price    = new float[Steps];
    Vega     = new float[Steps];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
MonteCarlo::fillInputBuffer()
{
    // Initializing sigma
    float stepValue = FinalValue / Steps;
    Sigma[0] = 0.01f;
    for (unsigned int i = 1; i < Steps; i++)
    {
        Sigma[i] = Sigma[i - 1] + stepValue;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
MonteCarlo::printTimeInfo()
{
    double time = timer->GetElapsedTime();
    fprintf(stdout, " \n\t\tTotal time taken in seconds %3.6f\n\n\n ", time);
}

struct ThreadData
{
    brook::Device* device;
    unsigned int start;
    unsigned int end;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief calculates the option price and vega for array of sigma values
//! 
////////////////////////////////////////////////////////////////////////////////

void* _deviceRun(void* data)
{
    int  noOfSum = 12;
    int  noOfTraj = 1024;

    //Define the timestep to use in the Monte Carlo simulation
    float timeStep = Maturity / (noOfSum - 1);

    float4 strikePricef4 = float4(StrikePrice, StrikePrice, StrikePrice, StrikePrice);
    float4 initPricef4 = float4(InitialPrice, InitialPrice, InitialPrice, InitialPrice);
    float4 timeStepf4 = float4(timeStep, timeStep, timeStep, timeStep);

    unsigned int* randVec = new unsigned int[noOfTraj * (noOfTraj / 2)];

    ThreadData* threadData = (ThreadData*)data;
    brook::useDevices(threadData->device, 1, NULL);

    // Stream dimension
    int  ySize = noOfTraj / 4;
    int  xSize = noOfTraj / 2;

    unsigned int dim[] = {xSize,ySize};
    ::brook::Stream<uint4> randStream(2, dim);
    ::brook::Stream<float4> pathDeriv1(2, dim);
    ::brook::Stream<float4> pathDeriv2(2, dim);
    ::brook::Stream<float4> priceVec1(2, dim);
    ::brook::Stream<float4> priceVec2(2, dim);

    for (unsigned int k = threadData->start; k < threadData->end; k++)
    {
        //simulate price using joint geometric Brownian motion
        for (int j = 0; j < noOfTraj * (noOfTraj / 2); j++)
        {
            randVec[j] = (uint)(rand());
        }

        float C1 = (Interest - 0.5f * Sigma[k] * Sigma[k]) * timeStep;
        float C2 = Sigma[k] * sqrt(timeStep);
        float C3 = Interest + 0.5f * Sigma[k] * Sigma[k];

        float4 C1f4 = float4(C1, C1, C1, C1);
        float4 C2f4 = float4(C2, C2, C2, C2);
        float4 C3f4 = float4(C3, C3, C3, C3);
        float4 sigmaf4 = float4(Sigma[k], Sigma[k], Sigma[k], Sigma[k]);

        streamRead(randStream, randVec);

        calPriceVegaVec((float)noOfSum, strikePricef4, C1f4, C2f4, C3f4, initPricef4, sigmaf4, timeStepf4, randStream,
                        priceVec1, priceVec2, pathDeriv1, pathDeriv2);

        float4  finalPrice1;
        float4  finalPrice2;
        float4  meanPathDeriv1;
        float4  meanPathDeriv2;

        sum2dStreams(priceVec1, finalPrice1);
        sum2dStreams(priceVec2, finalPrice2);
        sum2dStreams(pathDeriv1, meanPathDeriv1);
        sum2dStreams(pathDeriv2, meanPathDeriv2);

        Price[k]   = finalPrice1.x + finalPrice2.x + 
                      finalPrice1.y + finalPrice2.y + 
                      finalPrice1.z + finalPrice2.z + 
                      finalPrice1.w + finalPrice2.w;

        Vega[k]    = meanPathDeriv1.x + meanPathDeriv2.x + 
                      meanPathDeriv1.y + meanPathDeriv2.y + 
                      meanPathDeriv1.z + meanPathDeriv2.z + 
                      meanPathDeriv1.w + meanPathDeriv2.w;

        Price[k]   /= noOfTraj * noOfTraj;
        Vega[k]    /= noOfTraj * noOfTraj;
        Price[k]   *= exp(-Interest * Maturity);
        Vega[k]    *= exp(-Interest * Maturity);
    }

    delete[] randVec;

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
MonteCarlo::run()
{
    if(!info->Quiet)
    {
        fprintf(stdout, "\n\t Monte Carlo Simulation for Asian Option Price Calculation \n");
        fprintf(stdout, "\n =======================================================================\n");
    }

    // Record GPU Total time 
    timer->Start();

    unsigned int count = 0;
    brook::Device* device = brook::getDevices("cal", &count);

    ThreadData* data = new ThreadData[count];
    CPUThread* threads = new CPUThread[count];

    for(unsigned int i = 0; i < count; ++i)
    {
        data[i].start = i * Steps/count;
        data[i].end = (i + 1)* Steps/count;

        data[i].device = device + i;
    }

    for(unsigned int i = 0; i < count; ++i)
    {
        threads[i].create(_deviceRun, (void*)(data + i));
    }

    for(unsigned int i = 0; i < count; ++i)
    {
        threads[i].join();
    }

    delete[] data;
    delete[] threads;

    timer->Stop();

    if(!info->Quiet)
    {
        fprintf(stdout, "\t Sigma \t\t Price \t\t Vega \n  ");
        fprintf(stdout, "\t---------------------------------------------\n");

        for (unsigned int i = 0; i < Steps; i++)
        {
        fprintf(stdout, "\t%3.6f \t %3.6f \t %3.6f\n ", Sigma[i], Price[i], Vega[i]);
        }
        fprintf(stdout, "\t---------------------------------------------\n");
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

MonteCarlo::~MonteCarlo()
{
    delete[] Sigma;
    delete[] Price;
    delete[] Vega;
}
