/* EQ3.tst tests the EQ3.hdl program. The EQ3 chip should return
  true if its two 3-bit inputs are equal and false otehrwise */

load EQ3.hdl,        // Load the HDL program into the simulator
output-file EQ3.out, // Write scripts outputs to this file
compare-to EQ3.cmp,  // Compare script outputs to this file
output-list a b out; // Each subsequent output command should print the values of the variables a, b, and out
set a %B000, set b %B000, eval, output;
set a %B111, set b %B111, eval, output;
set a %B111, set b %B000, eval, output;
set a %B000, set b %B111, eval, output;
set a %B001, set b %B000, eval, output;
// Since the chip has two 3-bit inputs, an exhaustive test requires 2^3*2^3=64 such scenarios.
