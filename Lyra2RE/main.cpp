#include <unistd.h>
#include <ios>
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <omp.h>
#include "Lyra2RE.h"

#define MINTIME 1 // default = 1
#define MAXTIME 10 // 20
#define MINROWS 4 // 4
#define MAXROWS 64 // 64
#define MINCOLS 4 // 4
#define MAXCOLS 64 // 64
#define MAXTESTS 100000 // 1000
#define OMP_THREADS 4

/*
 * This function reads the current memory usage values from /proc/self/stat.
 *
 * @virtualMemUsage output parameter for saving virtual memory size
 * @residentSize output parameter for saving resident set size
 */

void processMemUsage(double& virtualMemUsage, double& residentSet)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   virtualMemUsage = 0.0;
   residentSet = 0.0;

   // Read the stat file.
   ifstream statStream("/proc/self/stat", ios_base::in);

   // Variables that are not needed
   string pid; // process ID
   string comm; // the filename of the executable
   string state; // process state
   string ppid; // parent process ID
   string pgrp; // process group
   string session; // the session ID of the process
   string tty_nr; // controlling terminal of the process
   string tpgid; // TTY process group ID
   string flags;
   string minflt; // number of minor page faults
   string cminflt; // cumulative number of minor page faults
   string majflt; // number of major page faults
   string cmajflt; // cumulative number of major page faults
   string utime; // user time
   string stime; // system time
   string cutime; // cumulative user time
   string cstime; // cumulative system time
   string priority;
   string nice; // process nice value
   string O;
   string itrealvalue; // time in jiffies - obsolete
   string starttime; // process start time

   // Important variables
   unsigned long vsize; // Virtual memory size
   long rss; // Resident set size

   statStream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
              >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
              >> utime >> stime >> cutime >> cstime >> priority >> nice
              >> O >> itrealvalue >> starttime >> vsize >> rss;

   statStream.close();

   long pageSize = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   virtualMemUsage = (double)vsize / 1024.0;
   residentSet = (double)(rss * pageSize);
}

int main()
{
    const char input[80] = "diejfuejfmfj45889966332211rrwwgdnfjksndfjknsdfjknsdkjfnskdffffffjnsffffffffffff";
    char output[32];
    const uint64_t totaltests = (MAXROWS - MINCOLS + 1) * (MAXCOLS - MINCOLS + 1) * MAXTIME;
    uint64_t currenttest = 0;
    double virtualMemUsage;
    double residentSetSize;

    std::ofstream outputlog;
    outputlog.open("benchmark.csv", std::ios::out);
    if(outputlog.is_open())
    {
        for(uint64_t timeCost = MINTIME; timeCost <= MAXTIME; timeCost++)
        {
            for(uint64_t nRows = MINROWS; nRows <= MAXROWS; nRows++)
            {
                for(uint64_t nCols = MINCOLS; nCols <= MAXCOLS; nCols++)
                {
                    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                    #pragma omp parallel for num_threads(OMP_THREADS)
                    for(int i = 0; i < MAXTESTS; i++)
                    {
                        lyra2re_hash(input, output, timeCost, nRows, nCols);
                    }

                    processMemUsage(virtualMemUsage, residentSetSize);

                    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<float, std::milli> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1);

                    currenttest++;

                    std::cout << "timeCost: " <<  timeCost
                              << ", nRows: " << nRows
                              << ", nCols: " << nCols
                              << ", hashrate: " << MAXTESTS / time_span.count() << "KH/s, "
                              << "virtualMemUsage: " << virtualMemUsage
                              << "kB, residentSetSize: " << residentSetSize
                              << "kB -- Progress: " << ((float)currenttest / (float)totaltests) * 100 << "%\n";

                    outputlog << timeCost << "," << nRows << "," << nCols << "," << MAXTESTS / time_span.count() << "," << virtualMemUsage << "," << residentSetSize << "\n";
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
