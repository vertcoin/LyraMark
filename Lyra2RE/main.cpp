#include <iostream>
#include <chrono>
#include <fstream>
#include "Lyra2RE.h"

#define MAXTIME 20
#define MAXROWS 64
#define MAXCOLS 64
#define MAXTESTS 1000

int main()
{
    const char input[80] = "diejfuejfmfj45889966332211rrwwgdnfjksndfjknsdfjknsdkjfnskdffffffjnsffffffffffff";
    char output[32];
    const uint64_t totaltests = (MAXROWS - 4) * (MAXCOLS - 4) * MAXTIME;
    uint64_t currenttest = 0;

    std::ofstream outputlog;
    outputlog.open("benchmark.csv", std::ios::out);
    if(outputlog.is_open())
    {
        for(uint64_t timeCost = 1; timeCost <= MAXTIME; timeCost++)
        {
            for(uint64_t nRows = 4; nRows <= MAXROWS; nRows++)
            {
                for(uint64_t nCols = 4; nCols <= MAXCOLS; nCols++)
                {
                    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                    for(int i = 0; i < MAXTESTS; i++)
                    {
                        lyra2re_hash(input, output, timeCost, nRows, nCols);
                    }

                    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<float, std::milli> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1);

                    currenttest++;

                    std::cout << "timeCost: " << timeCost << ", nRows: " << nRows << ", nCols: " << nCols << " -- Hashrate: " << MAXTESTS / time_span.count() << "KH/s -- Progress: " << ((float)currenttest / (float)totaltests) * 100 << "%\n";
                    outputlog << timeCost << "," << nRows << "," << nCols << "," << MAXTESTS / time_span.count() << "\n";
                    outputlog.flush();
                }
            }
        }
        outputlog.close();
    }
    else
    {
        std::cout << "ERROR: Could not open log file.";
    }

    return 0;
}
