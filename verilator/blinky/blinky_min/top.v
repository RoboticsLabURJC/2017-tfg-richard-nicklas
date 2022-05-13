/**
* This will turn on led1 when btn1 is pressed.
**************************************************************************/
module top (
    input wire btn1,		// a single-bit input signal called btn1
    output wire led1		// a single-bit output signal called led1
	);

	assign led1 = btn1;		// connect the btn1 input to the led1 output

endmodule
