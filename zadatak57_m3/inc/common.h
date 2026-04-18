#ifndef COMMON_H
#define COMMON_H

#include <stdfix.h>

// potrebno prekopirati sa pocetka stdfix_emu.h ili ukljuciti ceo stdfix_emu.h!
#if defined(__CCC)

#include <stdfix.h>

#define FRACT_NUM(x) (x##r)
#define LONG_FRACT_NUM(x) (x##lr)
#define ACCUM_NUM(x) (x##lk)

#define FRACT_NUM_HEX(x) (x##r)

#define FRACT_TO_INT_BIT_CONV(x) (bitsr(x))
#define INT_TO_FRACT_BIT_CONV(x) (rbits(x))

#define long_accum long accum
#define long_fract long fract

#endif

/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
/////////////////////////////////////////////////////////////////////////////////
#define BLOCK_SIZE		16
#define MAX_NUM_CHANNEL	8

// Number of channels
#define INPUT_NUM_CHANNELS 2

// Output Mode
typedef enum mode_e { OUTPUT_MODE_2_0_0, OUTPUT_MODE_3_2_0 } mode;

// Channel IDs.	 
// values of L_CH and R_CH must be 0 and 1 due to the relationship with the input channels
// other values are set relative to the output modes
#define L_CH	0	
#define R_CH	1
#define C_CH	2
#define LS_CH	3
#define RS_CH	4

// Gain linear values.
// 10^(x/20) - x value in dB
#define ZERO_DB 	FRACT_NUM(0.999999);	// 1
#define MINUS_4DB  	FRACT_NUM(0.630957);
#define MINUS_10DB  FRACT_NUM(0.316228);
#define MINUS_3P9DB FRACT_NUM(0.638263);
#define MINUS_9P5DB FRACT_NUM(0.334965);

#define SIGNALPROC_ASM	// comment if we do not want assembler processing
/////////////////////////////////////////////////////////////////////////////////

/* DSP type definitions */
typedef short DSPshort;				/* DSP integer */
typedef unsigned short DSPushort;	/* DSP unsigned integer */
typedef int DSPint;					/* native integer */
typedef fract DSPfract;				/* DSP fixed-point fractional, data reg, memory word format s.31 */
typedef long_accum DSPaccum;		/* DSP fixed-point fractional, accum reg format s8.63 */

#endif
