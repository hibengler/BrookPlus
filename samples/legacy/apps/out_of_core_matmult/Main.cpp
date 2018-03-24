/* ============================================================

Copyright (c) 2007 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the following 
conditions:
 
Redistributions must retain the above copyright notice and all terms of this 
license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */

#include "out_of_core_matmult.h"

// For console run, remove this #define.
//#define CAL_MARK


unsigned int DEFAULT_WIDTH  = 64;
unsigned int DEFAULT_HEIGHT = 64;
int          DEFAULT_VERIFY = 0;
int          DEFAULT_QUIET = 0;
int          DEFAULT_TIMING = 0;
int	     DEFAULT_USEINTEGER = 0;
int 	     DEFAULT_USEFLOAT = 1;
int          DEFAULT_USEDOUBLE = 0;
int          DEFAULT_PERFFORALL = 0;

void ParseCommandLine(int argc, char** argv, Info* info)
{
    int x, i;
    info->Width = DEFAULT_WIDTH;
    info->Height = DEFAULT_HEIGHT;
    info->Verify = DEFAULT_VERIFY;
    info->Quiet = DEFAULT_QUIET;
    info->Timing = DEFAULT_TIMING;
    info->UseInteger = DEFAULT_USEINTEGER;
    info->UseFloat = DEFAULT_USEFLOAT;
    info->UseDouble = DEFAULT_USEDOUBLE; 
    info->PerfForAll = DEFAULT_PERFFORALL;
    

    for (x = 1, i = 1; x < argc; ++x, i++)
    {
        switch (argv[x][1])
        {
        case 'e':
            info->Verify = 1;
            break;

        case 'q':
            info->Quiet = 1;
            break;

        case 't':
            info->Timing = 1;
            break;

        case 'x':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Width);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s", argv[x-1]);
                Usage(0);
                exit(-1);
            }
            break;

        case 'y':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Height);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s", argv[x-1]);
                Usage(0);
                exit(-1);
            }
            break;

        case 'h':
            Usage(argv[0]);
	    exit(-1);
            break;

        case 'i':
            if(strcmp(argv[i],"-int") == 0)
            {
                info->UseInteger = 1;
            }
            else
            {
                Usage(argv[0]);
                exit(-1);
            }
            break;
        
        case 'd':
            if(strcmp(argv[i],"-double") == 0)
            {
                info->UseDouble = 1;
            }
            else
            {
                Usage(argv[0]);
                exit(-1);
            }
            break;
        
        case 'f':
            if(strcmp(argv[i],"-float") == 0)
            {
                info->UseFloat = 1;
            }
            else
            {
                Usage(argv[0]);
                exit(-1);
            }
            break;

        case 'a':
            if(strcmp(argv[i],"-all")== 0)
            {
                info->PerfForAll = 1;
            }
            else
            {
                Usage(argv[0]);
                exit(-1);
            }
            break;

        default:
            fprintf(stderr, "Error: Invalid command line parameter, %c\n", argv[x][1]);
            Usage(argv[0]);
            exit(-1);
        }
    }
}


void Usage(char *name)
{
    printf("\tUsage: %s [-h] [-e] [-q] [-t] [x] [y] [-int] [-float] [-double] [-all]\n", name);
    printf("   -h       Print this help menu.\n");
    printf("   -e       Verify correct output.\n");
    printf("   -q       Surpress all data output.\n");
    printf("   -t       Print out timing information.\n");
    printf("   -x <int> Sets the matrix width.\n");
    printf("   -y <int> Sets the matrix height.\n");
    printf("   -int  	Performs integer matrix matrix multiplication.\n");
    printf("   -float  	Performs float matrix matrix multiplication.\n");
    printf("   -double 	Performs double matrix matrix multiplication.\n");
    printf("   -all	Benchmark mode, tabulates performance for different dimensions.\n\n");
    printf("Example: %s -float -x 1024 -y 1024 -t\n", name);
    printf("Example: %s -float -x 4096 -y 4096 -t -e\n", name);
    printf("Example: %s -int -x 4096 -y 4096 -t -e\n", name);
    printf("Example: %s -double -x 4096 -y 4096 -t -e\n", name);
}


////////////////////////////////////////////////////////////////////////////////////////
// \!brief          Main function, the entry point of the program.
//
// \!param argc     total number of command line arguments.
// \!param argv     a 2D array of string holding all command line inputs.
////////////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char **argv )
{
    // Parse Command Line Options 
    struct infoRec info;
    ParseCommandLine(argc, argv, &info);
    
    // If the type is float.
    if(info.UseFloat)
    {
        // If multiple run of the application required.
        if(info.PerfForAll)
        {
            unsigned int i = 1024;
            unsigned int j = 1024;
            unsigned int k = 1024;
            double gpuPerformance;
            double constantFactor = pow((double)10,(double)9);

            // If CAL_MARK support needed.
#ifdef CAL_MARK
            fprintf(stdout, "OUTPUT_GRAPH:GRAPH_DATA-OutOfCoreMM Performance chart, Dimension, \
                    GFlops, Gflops vs Dimension\n");
            for(i = 256; i <= 8192; i = 2 * i)
            {
                // j, k and i values are same as we are plotting graph Dimensions vs. Gflops.
                j = k = i;
                matrixMult <float>floatMult;
                floatMult.setDimension(i, j, k);
                floatMult.allocateMem();
                if(!floatMult.checkMemAlloc())
                {
                    cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                    return 0;
                }
                floatMult.fillMatrices();
                Setup(0);
                Start(0);
                floatMult.floatBrookFunction();
                Stop(0);
                gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;
                fprintf(stdout, "OUTPUT_GRAPH:%d\t\t%lf\n", i, gpuPerformance);
                fflush(stdout);
            }
#else
            // This is the implementation for console run.
            cout<<"\n\n========================================================================================";
            cout<<"\nM              K               N               GFLOPS                                    ";
            cout<<"\n=========================================================================================";
            for(i = 1024; i <= 8192; i = 2 * i)
            {
                for(j = 1024; j <= 8192; j = 2 *j)
                {
                    for(k = 1024; k <= 8192; k = 2 * k)
                    {
                        // Create teh float object of matrixMult class.
                        matrixMult <float>floatMult;

                        // Set dimensions for the matrices.
                        floatMult.setDimension(i, j, k);

                        // Allocate memory for the matrices with specified dimension.
                        floatMult.allocateMem();

                        // Check if memory allocation failed!
                        if(!floatMult.checkMemAlloc())
                        {
                            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                            return 0;
                        }

                        // If allocation succeed, fill the matrices with random values.
                        floatMult.fillMatrices();

                        // Setup the timer.
                        Setup(0);

                        // Start the timer.
                        Start(0);

                        // Call the c++ wraper function which internally invokes gpu kernel 
                        // handler ( a  C implementation).
                        floatMult.floatBrookFunction();
                        
                        // Stop the timer.
                        Stop(0);

                        // Calculate the GPU performance.
                        gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;
                        
                        // Print the performance.
                        cout<<"\n";
                        cout<<i<<"\t\t"<<j<<"\t\t"<<k<<"\t\t"<<gpuPerformance;
                    }
                    // Reapeat the steps for other matrix dimensions.
                }
                // Repeat the steps for other matrix dimensions.
            }
            // Repeat the steps for other matrix dimensions.
#endif
            return 0;
        }

        // Create the float object of the matrixMult class.
        matrixMult <float>floatMult;
        
        if(info.Width != 0 && info.Height != 0)
        {
            // Set the dimensions for the matrices.
            floatMult.setDimension(info.Height, info.Width, info.Height);
        }
        // Allocate memory for the specified dimensions.
        floatMult.allocateMem();

        // Check if memory allocation is failed.
        if(!floatMult.checkMemAlloc())
        {
            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
            return 0;
        }
        
        // Fill the matrices with random numbers (float random numbers).
        floatMult.fillMatrices();
        
        // Setup timers.
        Setup(0);

        // Start timer.
        Start(0);

        // Invoke the C++ wraper of the brook kernel handler.
        floatMult.floatBrookFunction();
        
        // Stop the timer.
        Stop(0);

        // Calculate the GPU performance.
        floatMult.calculateGPUPerformance(GetElapsedTime(0));
        
        if(info.Timing)
        {
            double time_taken = GetElapsedTime(0);
            cout<<"\nTime Taken: "<< time_taken <<endl;
        }

        if(info.Verify)
        {
            // Setup the timer for CPU multiplication.
            Setup(0);

            // Start the timer.
            Start(0);

            // Invoke the C++ wraper function for CPU matrix multiplication.
            floatMult.floatCpuMatMultCall();

            // Stop the timer.
            Stop(0);

            // Calculate the CPUPerformance.
            floatMult.calculateCPUPerformance(GetElapsedTime(0));

            // Match the CPU and GPU result for correctness.
            floatMult.checkResult(argv[0]);
        }
    }

    // If the data type is mentioned as integer.
    else if(info.UseInteger)
    {
        // If the application is requested for multiple run with different dimensions as inputs.
        if(info.PerfForAll)
        {
            unsigned int i = 1024;
            unsigned int j = 1024;
            unsigned int k = 1024;
            double gpuPerformance;
            double constantFactor = pow((double)10,(double)9);

            // Implementing the CAL_MARK support for integer data type.
#ifdef CAL_MARK
            // Plot the graph with the performance and dimensions.
            fprintf(stdout, "OUTPUT_GRAPH:GRAPH_DATA-OutOfCoreMM Performance chart for Integer, Dimension, \
                            GFlops, Gflops vs Dimension\n");
            for(i = 256; i <= 8192; i = 2 * i)
            {
                // j, k and i values are same as we are plotting graph Dimensions vs. Gflops.
                j = k = i;

                // Create the object for the class matrixMult.
                matrixMult <int>intMult;

                // Set the dimension.
                intMult.setDimension(i, j, k);

                // Allocate memory.
                intMult.allocateMem();

                // Check if the memory allocation is successful.
                if(!intMult.checkMemAlloc())
                {
                    cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                    return 0;
                }

                // Fill the matrices with random numbers of integer type.
                intMult.fillMatrices();

                // Setup the timer.
                Setup(0);

                // Start the timer.
                Start(0);

                // Invoke the Brook+ kernel handler.
                intMult.intBrookFunction();

                // Stop the timer.
                Stop(0);

                // Calculate the GPU performance.
                gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;

                fprintf(stdout, "OUTPUT_GRAPH:%d\t\t%f\n", i, gpuPerformance);
                
                fflush(stdout);
            }
#else
            // This is the implementation for console run.
            cout<<"\n\n========================================================================================";
            cout<<"\nM              K               N               GFLOPS                                    ";
            cout<<"\n=========================================================================================";
            for(i = 1024; i <= 8192; i = 2 * i)
            {
                for(j = 1024; j <= 8192; j = 2 *j)
                {
                    for(k = 1024; k <= 8192; k = 2 * k)
                    {
                        // Create object of matrixMUlt.
                        matrixMult <int>intMult;

                        // Set the dimensions.
                        intMult.setDimension(i, j, k);

                        // Allocate memory for the matrices.
                        intMult.allocateMem();

                        // Check if the requested memory is allocated successfully.
                        if(!intMult.checkMemAlloc())
                        {
                            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                            return 0;
                        }

                        // Fill the matrices with the integer random numbers.
                        intMult.fillMatrices();

                        // Setup the timer.
                        Setup(0);

                        // Start the timer.
                        Start(0);

                        // Call the Brook+ kernel handler.
                        intMult.intBrookFunction();

                        // Stop the timer.
                        Stop(0);

                        // Calculate the GPU performance.
                        gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;
                        
                        // Print the result.
                        cout<<"\n";
                        cout<<i<<"\t\t"<<j<<"\t\t"<<k<<"\t\t"<<gpuPerformance;
                    }
                }
            }
#endif
            return 0;
        }

        // Create the int object of matrixMult.
        matrixMult <int>intMult;
        if(info.Width != 0 && info.Height != 0)
        {
            intMult.setDimension(info.Height, info.Width, info.Height);
        }
        // Allocate memory for given matrices.
        intMult.allocateMem();
        
        // Check if the memory allocation is succeeded.
        if(!intMult.checkMemAlloc())
        {
            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
            return 0;
        }
        
        // Fill the matrices with the integer random numbers.
        intMult.fillMatrices();

        // Setup the timer.
        Setup(0);

        // Start the timer.
        Start(0);

        // Call the brook+ kernel handler.
        intMult.intBrookFunction();

        // Stop the timer.
        Stop(0);

        // Calculate the GPU performance.
        intMult.calculateGPUPerformance(GetElapsedTime(0));

        if(info.Timing)
        {
            double time_taken = GetElapsedTime(0);
            cout<<"\nTime Taken: "<< time_taken <<endl;
        }

        if(info.Verify)
        {
            // Setup the timer.
            Setup(0);

            // Start the timer.
            Start(0);

            // Call the matrix multiplication method for CPU.
            intMult.intCpuMatMultCall();

            // Stop the timer.
            Stop(0);

            // Calculate the CPU performance.
            intMult.calculateCPUPerformance(GetElapsedTime(0));
            // match the twor result for correctness.
            intMult.checkResult(argv[0]);

        }
    }

    // If the matrix multiplication for double requested.
    else if(info.UseDouble)
    {
        // If the application is requested for multiple run with different dimensions as inputs.
        if(info.PerfForAll)
        {
            unsigned int i = 1024;
            unsigned int j = 1024;
            unsigned int k = 1024;
            double gpuPerformance;
            double constantFactor = pow((double)10,(double)9);

            // Code added for CAL_MARK support.
#ifdef CAL_MARK
            // Plot the graph for gpu performance Vs dimensions.
            fprintf(stdout, "OUTPUT_GRAPH:GRAPH_DATA-OutOfCoreMM Performance chart, Dimension, \
                    GFlops, Gflops vs Dimension\n");

            for(i = 128; i <= 4096; i = 2 * i)
            {
                // j, k and i values are same as we are plotting graph Dimensions vs. Gflops.
                j = k = i;

                // Create the double object of the matrixMult class.
                matrixMult <double>doubleMult;

                // Set dimensions.
                doubleMult.setDimension(i, j, k);

                // Allocate memory for the specified dimensions.
                doubleMult.allocateMem();

                // Check if the memory allocation is succeeded.
                if(!doubleMult.checkMemAlloc())
                {
                    cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                    return 0;
                }

                // Fill the matrices with double data type.
                doubleMult.fillMatrices();

                // Setup timer.
                Setup(0);

                // Start the timer.
                Start(0);

                // Invoke the Brook+ kernel handler.
                doubleMult.doubleBrookFunction();

                // Stop the timer.
                Stop(0);

                // Calculate the GPU performance.
                gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;
               
                fprintf(stdout, "OUTPUT_GRAPH:%d\t\t%f\n", i, gpuPerformance);

                fflush(stdout);
            }
#else
            // This is the implementation for console run.
            cout<<"\n\n========================================================================================";
            cout<<"\nM              K               N               GFLOPS                                    ";
            cout<<"\n=========================================================================================";
            for(i = 512; i <= 4096; i = 2 * i)
            {
                for(j = 512; j <= 4096; j = 2 *j)
                {
                    for(k = 512; k <= 4096; k = 2 * k)
                    {
                        // Create a double object of the class matrixMult.
                        matrixMult <double>doubleMult;

                        // Set the dimensions for input matrices.
                        doubleMult.setDimension(i, j, k);

                        // Allocate memory for the input matrices and the result matrix.
                        doubleMult.allocateMem();

                        // Check if the memory allocation is successfull.
                        if(!doubleMult.checkMemAlloc())
                        {
                            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
                            return 0;
                        }

                        // Fill the matrices with double random numbers.
                        doubleMult.fillMatrices();

                        // Setup timer.
                        Setup(0);

                        // Start timer.
                        Start(0);

                        // Invoke the Brook+ kernel handler.
                        doubleMult.doubleBrookFunction();

                        // Stop the timer.
                        Stop(0);

                        // Calculate the GPU performance.
                        gpuPerformance = ((2 *(double)i * j * k)/ GetElapsedTime(0))/ constantFactor;

                        // Print the result.
                        cout<<"\n";
                        cout<<i<<"\t\t"<<j<<"\t\t"<<k<<"\t\t"<<gpuPerformance;
                    }
                }
            }
#endif
            return 0;
        }
        // If the exe runs for a specific input dimension set.
        // Create the float object of the matrixMult class.
        matrixMult <double>doubleMult;
        
        if(info.Width != 0 && info.Height != 0)
        {
            // Set the dimensions for the matrices.
            doubleMult.setDimension(info.Height, info.Width, info.Height);
        }
        // Allocate memory for the specified dimensions.
        doubleMult.allocateMem();

        // Check if memory allocation is failed.
        if(!doubleMult.checkMemAlloc())
        {
            cout<<"\n\nError in memory allocation of matrices...\n\nExiting......\n\n";
            return 0;
        }
        
        // Fill the matrices with random numbers (float random numbers).
        doubleMult.fillMatrices();
        
        // Setup timers.
        Setup(0);

        // Start timer.
        Start(0);

        // Invoke the C++ wraper of the brook kernel handler.
        doubleMult.doubleBrookFunction();
        
        // Stop the timer.
        Stop(0);

        // Calculate the GPU performance.
        doubleMult.calculateGPUPerformance(GetElapsedTime(0));
        
        if(info.Timing)
        {
            double time_taken = GetElapsedTime(0);
            cout<<"\nTime Taken: "<< time_taken <<endl;
        }

        if(info.Verify)
        {
            // Setup the timer for CPU multiplication.
            Setup(0);

            // Start the timer.
            Start(0);

            // Invoke the C++ wraper function for CPU matrix multiplication.
            doubleMult.doubleCpuMatMultCall();

            // Stop the timer.
            Stop(0);

            // Calculate the CPUPerformance.
            doubleMult.calculateCPUPerformance(GetElapsedTime(0));

            // Match the CPU and GPU result for correctness.
            doubleMult.checkResult(argv[0]);
        }
    }

	if (!info.Verify)
    {
        printf("\nPress enter to exit...\n");
        getchar();
    }
}
