#include <verilated.h>          // Defines common routines
#include <verilated_vcd_c.h>	// Need this to generate waveforms

#include "Vtop.h"

static Vtop *ptop;						// Instantiation of module

static vluint64_t main_time = 0;		// Current simulation time
static VerilatedVcdC* tfp = 0;

/**
* Simulate button presses. 
****************************************************************************/
int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);		// initialize Verilator
    ptop = new Vtop;             // Instantiate the top module

	// initialize a log file for a trace dump
	Verilated::traceEverOn(true);
	tfp = new VerilatedVcdC;
	ptop->trace(tfp, 99);		// trace down to 99 levels of design hierarchy
	tfp->open("wave.vcd");

	// run the simulation

	ptop->btn1 = 0;			// simulate the value from btn0 as low
	ptop->eval();			// tell verilator to react to the current state of btn1 
	tfp->dump(main_time);   // write the current state of hardware to log file
	main_time++;            // time passes...

	for (int x=0; x<5; ++x)
	{
		ptop->btn1 = 1;		// the value of btn1 changes from 0 to 1
		ptop->eval();
		tfp->dump(main_time);
		main_time++;

		ptop->btn1 = 0;		// the value of btn1 changes from 1 to 0
		ptop->eval();
		tfp->dump(main_time);
		main_time++;
	}

	tfp->dump(main_time);	// one last dump so last state can be seen in the waveform

	// we are finished running the simulation

	tfp->close();			// close the log file
    ptop->final();			// We are done simulating

	delete tfp;
	delete ptop;

	return 0;
}
