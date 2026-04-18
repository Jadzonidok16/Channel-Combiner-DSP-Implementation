#include <stdio.h>
#include <dsplib\wavefile.h>
#include <stdfix.h>
#include <string.h>
#include "common.h"
#include "iir.c"
#include <dsplib\timers.h>	// for profiling

// I/O Buffer
__memY DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

DSPfract limiterThreshold = FRACT_NUM(0.999);

// Enable
DSPint enabled = 1;	// default ON

// Gains
DSPfract variablesGain[INPUT_NUM_CHANNELS] = { FRACT_NUM(0.999999), FRACT_NUM(0.999999) }; // default -0dB

// Output Mode
mode outputMode = OUTPUT_MODE_3_2_0;	// default 3_2_0

DSPfract saturation(DSPaccum in)
{
	if (in > (DSPaccum)limiterThreshold)
	{
		return limiterThreshold;
	}
	else if (in < (DSPaccum)-limiterThreshold)
	{
		return -limiterThreshold;
	}
	return in;
}
#ifdef SIGNALPROC_ASM
extern void signalProcessing(__memY DSPfract pIn[][BLOCK_SIZE], __memY DSPfract pOut[][BLOCK_SIZE]);
#else
void signalProcessing(__memY DSPfract pIn[][BLOCK_SIZE], __memY DSPfract pOut[][BLOCK_SIZE])
{
	DSPint i;

	DSPaccum temp1;
	DSPaccum temp2;
	DSPaccum temp3;
	DSPfract tempFract;

	DSPfract *variablesGainPtrL = variablesGain + L_CH;
	DSPfract *variablesGainPtrR = variablesGain + R_CH;

	__memY DSPfract *samplePtrInL = *(pIn + L_CH);
	__memY DSPfract *samplePtrInR = *(pIn + R_CH);

	__memY DSPfract *samplePtrOutL = *(pOut + L_CH);
	__memY DSPfract *samplePtrOutR = *(pOut + R_CH);
	__memY DSPfract *samplePtrOutC = *(pOut + C_CH);
	__memY DSPfract *samplePtrOutLs = *(pOut + LS_CH);
	__memY DSPfract *samplePtrOutRs = *(pOut + RS_CH);

	for (i = 0; i < BLOCK_SIZE; i++)
    {
        // prva faza, primenjujemo input gainove na ulazne signale
        *samplePtrInL = *samplePtrInL * *variablesGainPtrL;
        *samplePtrInR = *samplePtrInR * *variablesGainPtrR;

        // OBRADA IZ LEVOG KANALA

        // druga faza, primenjujemo LPF 11kHz i rezultat saljemo na izlaz Ls
        temp3 = iir_l_lpf11((DSPaccum)*samplePtrInL);
        *samplePtrOutLs = saturation(temp3);

        // treca faza, primenjujemo HPF 5kHz, smanjujemo za -10dB i rezultat saljemo na izlaz C
        temp1 = iir_l_hpf5((DSPaccum)*samplePtrInL);
        tempFract = temp1 >> 1;
        tempFract = tempFract * MINUS_10DB;
        temp1 = tempFract << 1;
		*samplePtrOutC = saturation(temp1);

        // cetvrta faza, primenjujemo HPF 3kHz, smanjujemo za -4dB, sabiramo sa C izlaznim kanalom i saljemo na izlaz L
        temp2 = iir_l_hpf3(temp3);
        tempFract = temp2 >> 1;
        tempFract = tempFract * MINUS_4DB;
        temp2 = tempFract << 1;
        *samplePtrOutL = saturation(temp2 + temp1);

        // OBRADA IZ DESNOG KANALA

        // peta faza, primenjujemo HPF 5kHz
        temp3 = iir_r_lpf11((DSPaccum)*samplePtrInR);
        *samplePtrOutRs = saturation(temp3);

        // sesta faza, primenjujemo HPF 5kHz, smanjujemo za -9.5dB i smestamo u temp
        temp1 = iir_r_hpf5((DSPaccum)*samplePtrInR);
        tempFract = temp1 >> 1;
        tempFract = tempFract * MINUS_9P5DB;
        temp1 = tempFract << 1;

        // sedma faza, primenjujemo HPF 3kHz, smanjujemo za -3.9dB, sabiramo sa temp i saljemo na izlaz R
        temp2 = iir_r_hpf3(temp3);
        tempFract = temp2 >> 1;
        tempFract = tempFract * MINUS_3P9DB;
        temp2 = tempFract << 1;
        *samplePtrOutR = saturation(temp1 + temp2);

        samplePtrInL++;
        samplePtrInR++;
        samplePtrOutL++;
        samplePtrOutR++;
        samplePtrOutC++;
        samplePtrOutLs++;
        samplePtrOutRs++;
    }
}
#endif
/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>
//
// Function:
// main
//
// @param - argv[1] - Input file name
//        - argv[2] - Output file name
//			Optional arguments
//		  - argv[2] - Enable (0/1)
//		  - argv[3] - Left Input Gain (0 - -inf)
//		  - argv[4] - Right Input Gain (0 - -inf)
//		  - argv[5] - Output Mode (2_0_0/3_2_0)
// @return - nothing
// Comment: main routine of a program
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
 {
    WAVREAD_HANDLE *wav_in;
    WAVWRITE_HANDLE *wav_out;

	char WavInputName[256];
	char WavOutputName[256];

    DSPint inChannels;
    DSPint outChannels;
    DSPint bitsPerSample;
    DSPint sampleRate;
    DSPint iNumSamples;
    DSPint i;
    DSPint j;

    unsigned long long count1, count2, spent_cycles;	// for profiling

	//-------------------------------------------------

	// Init channel buffers
	for(i=0; i<MAX_NUM_CHANNEL; i++)
		for(j=0; j<BLOCK_SIZE; j++)
			sampleBuffer[i][j] = FRACT_NUM(0.0);

	// Open input wav file
	//-------------------------------------------------
	strcpy(WavInputName,argv[0]);
	wav_in = cl_wavread_open(WavInputName);
	if(wav_in == NULL)
    {
        printf("Error: Could not open wavefile.\n");
        return -1;
    }
	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	inChannels = cl_wavread_getnchannels(wav_in);
    bitsPerSample = cl_wavread_bits_per_sample(wav_in);
    sampleRate = cl_wavread_frame_rate(wav_in);
    iNumSamples =  cl_wavread_number_of_frames(wav_in);
	//-------------------------------------------------

	// Open output wav file
	//-------------------------------------------------
	strcpy(WavOutputName,argv[1]);
	//outChannels = inChannels;
	outChannels = (outputMode == 0) ? 2 : 5;	// change number of channels
	wav_out = cl_wavwrite_open(WavOutputName, bitsPerSample, outChannels, sampleRate);
	if(!wav_out)
    {
        printf("Error: Could not open wavefile.\n");
        return -1;
    }
	//-------------------------------------------------
	// Processing loop
	//-------------------------------------------------
#ifndef SIGNALPROC_ASM
	init_filter();
#endif
    {
		int i;
		int j;
		int k;
		int sample;

		// exact file length should be handled correctly...
		for(i=0; i<iNumSamples/BLOCK_SIZE; i++)
		{
			for(j=0; j<BLOCK_SIZE; j++)
			{
				for(k=0; k<inChannels; k++)
				{
					sample = cl_wavread_recvsample(wav_in);
        			sampleBuffer[k][j] = rbits(sample);
				}
			}

			// pozvati processing funkciju ovde
			if(enabled)
			{
				count1 = cl_get_cycle_count();
				signalProcessing(sampleBuffer, sampleBuffer);
				count2 = cl_get_cycle_count();
			}
			spent_cycles = count2 - count1;
			printf("%llu\n", spent_cycles);
			// -----------------------------------

			for(j=0; j<BLOCK_SIZE; j++)
			{
				for(k=0; k<outChannels; k++)
				{
					sample = bitsr(sampleBuffer[k][j]);
					cl_wavwrite_sendsample(wav_out, sample);
				}
			}
		}
	}

	// Close files
	//-------------------------------------------------
    cl_wavread_close(wav_in);
    cl_wavwrite_close(wav_out);
	//-------------------------------------------------

    return 0;
 }
