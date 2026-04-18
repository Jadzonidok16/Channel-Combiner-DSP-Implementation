#define COEFF_SIZE 6
#define HISTORY_NUM 12 // 6x2 - 6 poziva filtera
#define HISTORY_SIZE 2

// KOEFICIJENTI GENERISANI UZ POMOC WINFILTER-a

static double hpf_3khz_coeffs[COEFF_SIZE] = {
	0.75710671719859524000,
	-1.51421343439719050000,
	0.75710671719859524000,
	1.00000000000000000000,
	-1.45424358628876130000,
	0.57406191511085536000
};

static double hpf_5khz_coeffs[COEFF_SIZE] = {
	0.62684542140619903000,
	-1.25369084281239810000,
	0.62684542140619903000,
	1.00000000000000000000,
	-1.10922879267058330000,
	0.39815229395226642000
};

static double lpf_11khz_coeffs[COEFF_SIZE] = {
	0.25556862914388878000,
	0.51113725828777756000,
	0.25556862914388878000,
	1.00000000000000000000,
	-0.15346477243847909000,
	0.17573928901377697000
};

static double history[HISTORY_NUM][HISTORY_SIZE];

void init_filter()
{
    for (int i = 0; i < HISTORY_NUM; i++) {
        for (int j = 0; j < HISTORY_SIZE; j++) {
            history[i][j] = 0;
        }
    }
}

// OVA FUNKCIJA ISPOD JE DATA UZ POSTAVKU PROJEKTA

double second_order_IIR(double input, double* coefficients, double* x_history, double* y_history) {
    double output = 0;

	output += coefficients[0] * input;          /* A0 * x(n)    */
	output += coefficients[1] * x_history[0];   /* A1 * x(n-1)  */
    output += coefficients[2] * x_history[1];   /* A2 * x(n-2)  */
    output -= coefficients[4] * y_history[0];   /* B1 * y(n-1)  */
    output -= coefficients[5] * y_history[1];   /* B2 * y(n-2)  */

    
    y_history[1] = y_history[0];    /* y(n-2) = y(n-1) */
    y_history[0] = output;          /* y(n-1) = y(n)   */
    x_history[1] = x_history [0];   /* x(n-2) = x(n-1) */
    x_history[0] = input;           /* x(n-1) = x(n)   */

    return output;
}

// LPF 11kHz na signal iz leve grane
double iir_l_lpf11(const double sample)
{
	return second_order_IIR(sample, lpf_11khz_coeffs, history[0], history[1]);
}
// HPF 3kHz na signal iz leve grane
double iir_l_hpf3(const double sample)
{
	return second_order_IIR(sample, hpf_3khz_coeffs, history[2], history[3]);
}
// HPF 5kHz na signal iz leve grane
double iir_l_hpf5(const double sample)
{
	return second_order_IIR(sample, hpf_5khz_coeffs, history[4], history[5]);
}
// LPF 11kHz na signal iz desne grane
double iir_r_lpf11(const double sample)
{
	return second_order_IIR(sample, lpf_11khz_coeffs, history[6], history[7]);
}
// HPF 3kHz na signal iz desne grane
double iir_r_hpf3(const double sample)
{
	return second_order_IIR(sample, hpf_3khz_coeffs, history[8], history[9]);
}
// HPF 5kHz na signal iz desne grane
double iir_r_hpf5(const double sample)
{
	return second_order_IIR(sample, hpf_5khz_coeffs, history[10], history[11]);
}
