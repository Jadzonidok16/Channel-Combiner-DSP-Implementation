#ifndef _IIR_H
#define _IIR_H

#include "common.h"

void init_filter();

DSPaccum iir_l_lpf11(DSPaccum sample);
DSPaccum iir_l_hpf3(DSPaccum sample);
DSPaccum iir_l_hpf5(DSPaccum sample);
DSPaccum iir_r_lpf11(DSPaccum sample);
DSPaccum iir_r_hpf3(DSPaccum sample);
DSPaccum iir_r_hpf5(DSPaccum sample);

#endif //_IIR_H