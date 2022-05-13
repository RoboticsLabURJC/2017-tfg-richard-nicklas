#include <unistd.h>
#include <getopt.h>

#include <verilated.h>          // Defines common routines
#include <verilated_vcd_c.h>
#include <iostream>             // Need std::cout
#include <iomanip>
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "Vtop.h"

using namespace std;

static Vtop *ptop;                      // Instantiation of module

static vluint64_t main_time = 0;       // Current simulation time
static VerilatedVcdC* tfp = 0;

#define PERIOD  (5)

/**
* Called by $time in Verilog
****************************************************************************/
double sc_time_stamp ()
{
    return main_time;       // converts to double, to match
                            // what SystemC does
}

/**
****************************************************************************/
static void tick(int count)
{
    for (;count > 0; --count)
    {
        //if (tfp)
            //tfp->dump(main_time); // dump traces (inputs stable before outputs change)
        ptop->eval();            // Evaluate model
        main_time++;            // Time passes...
        if (tfp)
            tfp->dump(main_time);   // inputs and outputs all updated at same time
    }
}

/**
****************************************************************************/
static void run(uint64_t limit)
{
    uint64_t count = 0;

    while(count < limit)
    {
        ptop->clk = 1;
        tick(PERIOD);
        ptop->clk = 0;
        tick(PERIOD);

        ++count;
    }
}

/**
****************************************************************************/
static void reset()
{
	// this module has nothing to reset
}

/**
****************************************************************************/
int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);
    ptop = new Vtop;             // Create instance

    int verbose = 0;

    int opt;
    while ((opt = getopt(argc, argv, "tv:")) != -1)
    {
        switch (opt)
        {
        case 'v':
            verbose=atoi(optarg);
            break;
        case 't':
            // init trace dump
            Verilated::traceEverOn(true);
            tfp = new VerilatedVcdC;
            ptop->trace(tfp, 99);
            tfp->open("wave.vcd");
            break;
        }
    }

    // start things going
    reset();
    run(1000);

    if (tfp)
        tfp->close();

    ptop->final();               // Done simulating

    if (tfp)
        delete tfp;

    delete ptop;

	return 0;
}
