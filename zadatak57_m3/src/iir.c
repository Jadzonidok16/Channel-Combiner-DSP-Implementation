#include "common.h"

#ifndef SIGNALPROC_ASM

#define COEFF_SIZE 6
#define HISTORY_NUM 12	// 6x2 - 6 poziva filtera
#define HISTORY_SIZE 2

// KOEFICIJENTI GENERISANI UZ POMOC WINFILTER-a
// skalirani tako sto su podeljeni sa 2 (x >> 1)
// morali smo da prebacimo fajl iz .c u .cpp zato sto u c function call is not allowed in a constant expression
static DSPfract hpf_3khz_coeffs[COEFF_SIZE] = {
	FRACT_NUM(0.3785533586),
	FRACT_NUM(-0.7571067172),
	FRACT_NUM(0.3785533586),
	FRACT_NUM(0.5),
	FRACT_NUM(-0.7271217931),
	FRACT_NUM(0.2870309576)
};

static DSPfract hpf_5khz_coeffs[COEFF_SIZE] = {
	FRACT_NUM(0.3134227107),
	FRACT_NUM(-0.6268454214),
	FRACT_NUM(0.3134227107),
	FRACT_NUM(0.5),
	FRACT_NUM(-0.5546143963),
	FRACT_NUM(0.199076147)
};

static DSPfract lpf_11khz_coeffs[COEFF_SIZE] = {
	FRACT_NUM(0.1277843146),
	FRACT_NUM(0.2555686291),
	FRACT_NUM(0.1277843146),
	FRACT_NUM(0.5),
	FRACT_NUM(-0.07673238622),
	FRACT_NUM(0.08786964451)
};

static DSPfract history[HISTORY_NUM][HISTORY_SIZE];

void init_filter()
{
	DSPfract* historyPtr = *history;
	DSPint i;

    for (i = 0; i < HISTORY_NUM; i++) {
		*historyPtr = FRACT_NUM(0.0);
		*(historyPtr + 1) = FRACT_NUM(0.0);
		historyPtr += 2;
    }
}

// OVA FUNKCIJA ISPOD JE DATA UZ POSTAVKU PROJEKTA

DSPaccum second_order_IIR(DSPfract input, DSPfract* coefficients, DSPfract* x_history, DSPfract* y_history) {
    DSPaccum output = FRACT_NUM(0.0);

	DSPfract* coeffPtr = coefficients;

	output += (*coeffPtr) * input;
	coeffPtr += 1;
	output += (*coeffPtr) * (*x_history);
	coeffPtr += 1;
	output += (*coeffPtr) * (*(x_history + 1));
	coeffPtr += 2;
	output -= (*coeffPtr) * (*y_history);
	coeffPtr += 1;
	output -= (*coeffPtr) * (*(y_history + 1));

	output = output << 1; // mnozenje sa 2 zato sto su svi koeficijenti podeljeni sa 2

	*(y_history + 1) = *y_history;
	*y_history = output;
	*(x_history + 1) = *x_history;
	*x_history = input;

    return output;
}

// LPF 11kHz na signal iz leve grane
DSPaccum iir_l_lpf11(const DSPaccum sample)
{
	return second_order_IIR(sample, lpf_11khz_coeffs, history[0], history[1]);
}
// HPF 3kHz na signal iz leve grane
DSPaccum iir_l_hpf3(const DSPaccum sample)
{
	return second_order_IIR(sample, hpf_3khz_coeffs, history[2], history[3]);
}
// HPF 5kHz na signal iz leve grane
DSPaccum iir_l_hpf5(const DSPaccum sample)
{
	return second_order_IIR(sample, hpf_5khz_coeffs, history[4], history[5]);
}
// LPF 11kHz na signal iz desne grane
DSPaccum iir_r_lpf11(const DSPaccum sample)
{
	return second_order_IIR(sample, lpf_11khz_coeffs, history[6], history[7]);
}
// HPF 3kHz na signal iz desne grane
DSPaccum iir_r_hpf3(const DSPaccum sample)
{
	return second_order_IIR(sample, hpf_3khz_coeffs, history[8], history[9]);
}
// HPF 5kHz na signal iz desne grane
DSPaccum iir_r_hpf5(const DSPaccum sample)
{
	return second_order_IIR(sample, hpf_5khz_coeffs, history[10], history[11]);
}

#endif
