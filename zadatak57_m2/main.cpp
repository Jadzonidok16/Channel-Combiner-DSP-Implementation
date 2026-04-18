#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WAVheader.h"
#include "iir.h"
#include "common.h"

// I/O Buffer
static DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

static DSPfract limiterThreshold = FRACT_NUM(0.999);

// Enable
static DSPint enabled = 1; // default ON

// Gains
static DSPfract variablesGain[INPUT_NUM_CHANNELS] = { ZERO_DB, ZERO_DB }; // default -0dB

// Output Mode
static mode outputMode = OUTPUT_MODE_3_2_0; // default 3_2_0

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

void signalProcessing(DSPfract pIn[][BLOCK_SIZE], DSPfract pOut[][BLOCK_SIZE])
{
	DSPint i;

	DSPaccum temp1;
	DSPaccum temp2;
	DSPaccum temp3;
	DSPfract tempFract;

	DSPfract *variablesGainPtrL = variablesGain + L_CH;
	DSPfract *variablesGainPtrR = variablesGain + R_CH;

	DSPfract *samplePtrInL = *(pIn + L_CH);
	DSPfract *samplePtrInR = *(pIn + R_CH);

	DSPfract *samplePtrOutL = *(pOut + L_CH);
	DSPfract *samplePtrOutR = *(pOut + R_CH);
	DSPfract *samplePtrOutC = *(pOut + C_CH);
	DSPfract *samplePtrOutLs = *(pOut + LS_CH);
	DSPfract *samplePtrOutRs = *(pOut + RS_CH);

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
		for (int j = 0; j < BLOCK_SIZE; j++)
			sampleBuffer[i][j] = FRACT_NUM(0.0);

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
			variablesGain[R_CH] = FRACT_NUM(pow(10.0, ((rGain) / 20.0)));
		else
		{
			printf("Wrong Input Gain! (-inf, 0]\n");
			return -1;
		}
	}

	// Get Left Input Gain
	if (argc > 4)
	{
		double lGain = atof(argv[4]);
		if (lGain <= 0)
			variablesGain[L_CH] = FRACT_NUM(pow(10.0, ((lGain) / 20.0)));
		else
		{
			printf("Wrong Input Gain! (-inf, 0]\n");
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
					sample = sampleBuffer[k][j].toLong(); // crude, non-rounding
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample / 8, 1, wav_out);
				}
			}
			fflush(wav_out);
		}
	}

	// Close files
	//-------------------------------------------------
	fclose(wav_in);
	fclose(wav_out);
	//-------------------------------------------------

	return 0;
}