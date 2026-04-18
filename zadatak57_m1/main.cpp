#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WAVheader.h"
#include "iir.c"

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

// Number of channels
#define INPUT_NUM_CHANNELS 2

// Output Mode
enum mode
{
	OUTPUT_MODE_2_0_0,
	OUTPUT_MODE_3_2_0
};

// Channel IDs.
// values of L_CH and R_CH must be 0 and 1 due to the relationship with the input channels
// other values are set relative to the output modes
#define L_CH 0
#define R_CH 1
#define C_CH 2
#define LS_CH 3
#define RS_CH 4

// Gain linear values.
// 10^(x/20) - x value in dB
#define ZERO_DB 0.999999 // 1
#define MINUS_4DB 0.630957
#define MINUS_10DB 0.316228
#define MINUS_3P9DB 0.638263
#define MINUS_9P5DB 0.334965

// I/O Buffer
static double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

static double limiterThreshold = 0.999;

// Enable
static int enabled = 1; // default ON

// Gains
static double variablesGain[INPUT_NUM_CHANNELS] = {ZERO_DB, ZERO_DB}; // default -0dB

// Output Mode
static mode outputMode = OUTPUT_MODE_3_2_0; // default 3_2_0

double saturation(double in)
{
	if (in > limiterThreshold)
	{
		return fmin(in, limiterThreshold);
	}
	else if (in < -limiterThreshold)
	{
		return fmax(in, -limiterThreshold);
	}
	return in;
}

void signalProcessing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE])
{
	double temp;

	double *variablesGainPtrL = variablesGain + L_CH;
	double *variablesGainPtrR = variablesGain + R_CH;

	double *samplePtrInL = *(pIn + L_CH);
	double *samplePtrInR = *(pIn + R_CH);

	double *samplePtrOutL = *(pOut + L_CH);
	double *samplePtrOutR = *(pOut + R_CH);
	double *samplePtrOutC = *(pOut + C_CH);
	double *samplePtrOutLs = *(pOut + LS_CH);
	double *samplePtrOutRs = *(pOut + RS_CH);

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		// prva faza, primenjujemo input gainove na ulazne signale
		*samplePtrInL = *samplePtrInL * *variablesGainPtrL;
		*samplePtrInR = *samplePtrInR * *variablesGainPtrR;

		// OBRADA IZ LEVOG KANALA

		// druga faza, primenjujemo LPF 11kHz i rezultat saljemo na izlaz Ls
		*samplePtrOutLs = saturation(iir_l_lpf11(*samplePtrInL));

		// treca faza, primenjujemo HPF 5kHz, smanjujemo za -10dB i rezultat saljemo na izlaz C
		*samplePtrOutC = iir_l_hpf5(*samplePtrInL);
		temp = *samplePtrOutC * MINUS_10DB;
		*samplePtrOutC = saturation(temp);

		// cetvrta faza, primenjujemo HPF 3kHz, smanjujemo za -4dB, sabiramo sa C izlaznim kanalom i saljemo na izlaz L
		*samplePtrOutL = iir_l_hpf3(*samplePtrOutLs);
		*samplePtrOutL = *samplePtrOutL * MINUS_4DB;
		*samplePtrOutL = saturation(*samplePtrOutL + temp);

		// OBRADA IZ DESNOG KANALA

		// peta faza, primenjujemo HPF 5kHz
		*samplePtrOutRs = saturation(iir_r_lpf11(*samplePtrInR));

		// sesta faza, primenjujemo HPF 5kHz, smanjujemo za -9.5dB i smestamo u temp
		temp = iir_r_hpf5(*samplePtrInR);
		temp = temp * MINUS_9P5DB;

		// sedma faza, primenjujemo HPF 3kHz, smanjujemo za -3.9dB, sabiramo sa temp i saljemo na izlaz R
		*samplePtrOutR = iir_r_hpf3(*samplePtrOutRs);
		*samplePtrOutR = *samplePtrOutR * MINUS_3P9DB;
		*samplePtrOutR = saturation(*samplePtrOutR + temp);

		samplePtrInL++;
		samplePtrInR++;
		samplePtrOutL++;
		samplePtrOutR++;
		samplePtrOutC++;
		samplePtrOutLs++;
		samplePtrOutRs++;
	}
}

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
	FILE *wav_in = NULL;
	FILE *wav_out = NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr, outputWAVhdr;

	if (argc < 3 || argc > 7)
	{
		printf("Wrong number of arguments!\n");
		printf("Usage: INPUT_FILE OUTPUT_FILE [Enable] [G1] [G2]  [MODE]\n");
		printf("Enable = [0|1]\nG1,G2 = (-inf, 0]\nMODE = [\"2_0_0\"|\"3_2_0\"]\n");
		return -1;
	}

	// Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		memset(&sampleBuffer[i], 0, BLOCK_SIZE);

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName, argv[1]);
	wav_in = OpenWavFileForRead(WavInputName, "rb");
	strcpy(WavOutputName, argv[2]);
	wav_out = OpenWavFileForRead(WavOutputName, "wb");
	//-------------------------------------------------

	// Get Control values from arguments
	//-------------------------------------------------
	// Get Output Mode
	if (argc > 6)
	{
		if (strcmp(argv[6], "2_0_0") == 0)
			outputMode = OUTPUT_MODE_2_0_0;
		else if (strcmp(argv[6], "3_2_0") == 0)
			outputMode = OUTPUT_MODE_3_2_0;
		else
		{
			printf("Wrong mode!\n");
			return -1;
		}
	}

	// Get Right Input Gain
	if (argc > 5)
	{
		double rGain = atof(argv[5]);
		if (rGain <= 0)
			variablesGain[R_CH] = pow(10.0, ((rGain) / 20.0));
		else
		{
			printf("Wrong Input Gain! (-int, 0]\n");
			return -1;
		}
	}

	// Get Left Input Gain
	if (argc > 4)
	{
		double lGain = atof(argv[4]);
		if (lGain <= 0)
			variablesGain[L_CH] = pow(10.0, ((lGain) / 20.0));
		else
		{
			printf("Wrong Input Gain! (-int, 0]\n");
			return -1;
		}
	}

	// Get Enable control
	if (argc > 3)
	{
		if (strcmp(argv[3], "0") == 0)
			enabled = 0;
		else if (strcmp(argv[3], "1") != 0)
		{
			printf("Wrong Enable control!\n");
			return -1;
		}
	}

	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in, inputWAVhdr);
	//-------------------------------------------------

	// Set up output WAV header
	//-------------------------------------------------
	outputWAVhdr = inputWAVhdr;
	outputWAVhdr.fmt.NumChannels = (outputMode == 0) ? 2 : 5; // change number of channels

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size / inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate / inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign / inputWAVhdr.fmt.NumChannels;

	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign * outputWAVhdr.fmt.NumChannels;

	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out, outputWAVhdr);

	init_filter();

	// Processing loop
	//-------------------------------------------------
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample / 8;
		const double SAMPLE_SCALE = -(double)(1 << 31); // 2^31
		int iNumSamples = inputWAVhdr.data.SubChunk2Size / (inputWAVhdr.fmt.NumChannels * inputWAVhdr.fmt.BitsPerSample / 8);

		// exact file length should be handled correctly...
		for (int i = 0; i < iNumSamples / BLOCK_SIZE; i++)
		{
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < inputWAVhdr.fmt.NumChannels; k++)
				{
					sample = 0; // debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				 // scale sample to 1.0/-1.0 range
				}
			}

			if (enabled)
				signalProcessing(sampleBuffer, sampleBuffer);


			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < outputWAVhdr.fmt.NumChannels; k++)
				{
					sample = sampleBuffer[k][j] * SAMPLE_SCALE; // crude, non-rounding
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample / 8, 1, wav_out);
				}
			}
		}
	}

	// Close files
	//-------------------------------------------------
	fclose(wav_in);
	fclose(wav_out);
	//-------------------------------------------------

	return 0;
}