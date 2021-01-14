#include <xc.h>

//  CONFIGURATION BITS
///////////////////////////////////////////////////////////////////////////////
// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1 and PGD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)
#pragma config FWDTEN = OFF

// The dsPIC will be clocked by the primary oscillator with a 10MHz crystal.
// We want to use the PLL to obtain Fosc = 80MHz ( <=> 40MIPS ).
// Problem : with a 10MHz crystal, PLL constraints are not met with the
// default parameter.
// Solution :
//	- boot using the internal FRC oscillator as clock source,
//	- set the right PLL parameters to obtain Fosc = 80MHz, without violating
//	  the PLL constraints,
//	- switch the clock source to the PLL output (with FRC as PLL input)
//	- wait for the end of the clock switch
//
#pragma config FNOSC = FRC              // Oscillator Mode (Internal Fast RC (FRC))
#pragma config POSCMD = NONE            // Primary Oscillator Source (XT Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)




/* Configures PLL prescaler, PLL postscaler, PLL divisor to obtain Fosc = 80MHz
 * with the FRC oscillator (Fin = 7.37MHz). 
 * We obtain Fosc = 7.37MHz*65/(*2) = 79.96MHz */
void frcPllConfig(void) {
	// Fosc = Fin*M/(N1*N2), where :
	//		M = PLLFBD + 2
	// 		N1 = PLLPRE + 2
	// 		N2 = 2 x (PLLPOST + 1)
    PLLFBD = 63;
    CLKDIVbits.PLLPRE = 1;
    CLKDIVbits.PLLPOST = 0;

	// Initiate Clock Switch to FRC with PLL
	__builtin_write_OSCCONH( 0x01 );
	__builtin_write_OSCCONL( OSCCON | 0x01 );
	// Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);
    // Wait for PLL to lock
    while(OSCCONbits.LOCK != 1);
}