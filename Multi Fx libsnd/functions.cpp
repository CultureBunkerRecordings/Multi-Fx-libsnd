
#include <sndfile.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "functions.h"
using namespace std;
//-------------------------------------------------------------------------------------------
// Function to print the RMS to the screen. The return is a float.
////inputSignal: Pointer to an array of floating point audio samples.
//numOutputSamples: number of audio samples including delayTimeInSamples
//PRINTOBJECT::PRINTOBJECT(float* inputSignal, int numOutputSamples, int numToPrint)
//{
	
	//	inputSignal = 0;
		//numOutputSamples = 0;
		//numToPrint = 0;
	    
//}

//PRINTOBJECT::~PRINTOBJECT()
//{
//}

float PRINTOBJECT::getRMS(float* inputSignal, int numOutputSamples)
{
	//some variable are initialised
	float RMS;
	float mean;
	//sum is allocated a value of 0 this will be used to add the amplitudes of each 
	//audio sample within the file
	float sum = 0;
	for (long i = 0; i < numOutputSamples; i++)
	{
		//the cmath function pow is used to multiply the inputSignal by itself.
		//the value of sum will develop with each iteration.
		sum = sum + pow(inputSignal[i], 2);
	}
	//the following equation is used to get the mean, this is cast as a float
	mean = sum / (float)numOutputSamples;
	//the following equation uses cmath function sqrt() to find the square root of the mean
	//resulting in the RMS
	RMS = sqrt(mean);

	return RMS;
}
//-----------------------------------------------------------------------------------------

// Function to print samples to screen. The return is void.


void PRINTOBJECT::printsamples(float* inputSignal, int numToPrint)
{
	/* A for loop is used until numToPrint is reached*/

	for (int i = 0; i < numToPrint; i++)
	{
		/* using namespace std, 'cout' prints the amplitude of the samples to the console
		the index of the floating point array 'inputSignal' is iterated within a for loop */

		cout << inputSignal[i] << endl;
	}

};
//----------------------------------------------------------------------------------------
// Function to scale the signal.The return is void, 
/*FX::FX(float*inputSignal, int numOutputSamples, int modfreq, int fadeTimeInSamples,
	float threshold, float scale, int delayTimeInSamples)
{
	inputSignal = 0;
	numOutputSamples = 0;
	scale = 0;
	threshold = 0;
	fadeTimeInSamples = 0;
	modfreq = 0;
};

FX::~FX()
{
}*/
void FX::scaleSignal(float* inputSignal, int numOutputSamples, float scale)
{
	/* A for loop is used to until 'numSamples' (length of audio file) is met*/

	for (int i = 0; i < numOutputSamples; i++)
	{

		// The index of 'float* inputSignal' is iterated within the for loop 
		// This will multiply 'inputSignal' by 'scale' and overwrite float* inputSignal.
		// If scale is >1 the audio will be amplified if scale <1 the audio will be attenuated.

		inputSignal[i] = inputSignal[i] * scale;
	}
};

//-----------------------------------------------------------------------------------
/* This function will add a delayed version of the inputSignal to itself.
* inputSignal  : Pointer to an array of floating point audio samples.
* numSamples   : The length of the array pointed to by inputSignal (in samples).
* delayTimeInSamples    : The number of samples to delay the signal by.
*/

float* FX::getSignalWithDelay(float* inputSignal, int numSamples, int delayTimeinSamples)
{
	/* long int outputSize is initialised
	outputSize is declared as numSamples(length of audio file in samples) + delayTimeInSamples
	(number of samples delayed by) This is used to determine the length of the for loop.*/

	long int outputSize = numSamples + delayTimeinSamples;

	/* A pointer to an array of floating point numbers (the length of outputSize) is assigned to delaySignal*/

	float* delaySignal = new float[outputSize];

	/*A for loop is used to iterate the index of inputSignal and delaySignal*/

	for (int i = 0; i < outputSize; i++)
	{
		/* if statements are used to determine three states

		1. if the loops iteration is lower than delayTimeinSamples the delaySignal
		will remain the same as inputSignal*/
		if (i < delayTimeinSamples)
		{
			delaySignal[i] = inputSignal[i];
		}
		//2. if the loops iteration is higher than delayTimeInSamples and lower than numSamples
		//	 the delaySignal will equal the original inputSignal plus the inputSignal delayed by delayTimeinSamples

		else if (i >= delayTimeinSamples && i < numSamples)
		{
			delaySignal[i] = inputSignal[i] + inputSignal[i - delayTimeinSamples];
		}
		//3. if the loops iteration surpasses numSamples the delaySignal will equal the inputSignal delayed
		//	 by delayTimeInSamples
		else
		{
			delaySignal[i] = inputSignal[i - delayTimeinSamples];
		}
	}
	//function returns floating point array delaySignal
	return delaySignal;
}
//-----------------------------------------------------------------------------------
/*This function will add a clipping distortion to the inputSignal

inputSignal: pointer to an array of floating point audio samples
numOutputSamples: number of audio samples including delayTimeInSamples
threshold: declared by user (the amplitude at which clipping occurs)
*/
float* FX::getSignalWithDistortion(float* inputSignal, int numOutputSamples, float threshold)
{
	long int i;

	//A for loop is used to iterate the index of inputSignal until numOutputSamples is reached 

	for (i = 0; i < numOutputSamples; i++)
	{
		/*if statements are used to determine 2 states

		1. if the inputSignal iteration is equal or greater than threshold, inputSignal equals threshold,
		this forces the cone of the speaker to a fixed point creating clipping a*/

		if (inputSignal[i] >= threshold)
		{
			inputSignal[i] = threshold;
		}

		/*2. if the inputSignal iteration is lower or equal to the negative of threshold, inputSignal
		equals the negative threshold.
		As we are dealing with an ac signal this is needed to create clipping on the
		negative half of inputSignal*/

		else if (inputSignal[i] <= -threshold)
		{
			inputSignal[i] = -threshold;
		}

	}

	//returns a pointer to an array of floating point audio samples 

	return inputSignal;
}
//--------------------------------------------------------------------------
/* This function will add a fade in to the audio file.

inputSignal: pointer to an array of floating point audio samples
fadeTimeInSamples: declared by user at runtime, number of samples in fade */

float* FX::getSignalWithFadein(float* inputSignal, int fadeTimeInSamples)
{
	/* float 'A' (amplitude) is initialised*/

	float A;

	/* A for loop is used untill fadeTimeInSamples is reached */

	for (long i = 0; i < fadeTimeInSamples; i++)
	{
		/*float 'A' (amplitude) is declared as the iteration divided by int 'fadeTimeInSamples'.
		This is cast as a float*/
		A = i / (float)fadeTimeInSamples;
		/* The iteration of inputSignal is multiplyed by 'A' causing a gradual fade untill the end of file */
		inputSignal[i] = inputSignal[i] * A;
	}

	/* returns a pointer to an array of floating point audio samples  */

	return inputSignal;
}

//------------------------------------------------------------------------------------------

/* This function will add amplitude modulation to the inputSignal

inputSignal: pointer to an array of floating point audio samples
numOutputSamples: number of samples in the file including the addition of delayTimeInSamples
modfreq: the frequency declared by the user to modulate*/

float* FX::getSignalWithAmpmod(float* inputSignal, int numOutputSamples, int modfreq)
{
	//initialisation of float t (time)
	float t;

	//initialisation of a pointer 
	//declared as an array of floating point numbers (length of numOuputSamples)
	//memory allocated to array. Size of float (4 bytes) multiplyed by numOutputSamples.

	float* A = new float[sizeof(float)*numOutputSamples];

	// for loop is used until numOutputSamples is reached

	for (long i = 0; i < numOutputSamples; i++)
	{
		// float t is assigned the equation to generate time 

		t = i / (float)numOutputSamples;

		// The index of the floating point array 'A' is assigned the equation for 
		// generating a sine wave. modfreq is declared by user and controls the frequency of sine wave 

		A[i] = (float)sin(2 * M_PI*modfreq*t);

		// the index of inputSignal is then multiplyed by the index of 'A' to create an 
		// amplitude modulation effect.

		inputSignal[i] = inputSignal[i] * A[i];
	}

	//returns a pointer to an array of floating point audio samples

	return inputSignal;
}

//This function will apply a moving average filter to the inputSignal

//inputSignal: pointer to an array of floating point audio samples
//numOutputSamples: number of samples in the file including the addition of delayTimeInSamples

float* FX::getSignalWithFilter(float*inputSignal, int numOutputSamples)
{
	//int N controls the point at which the lowpass filter acts, higher numbers generate a lower cut off

	int N = 5;

	// a for loop is used untill the value of 'numOutputSamples' minus 'N'-1 is reached
	//this first loop is used to work out the average of 'N' samples and store them in the 
	//index of inputSignal.

	for (long i = 0; i < numOutputSamples - (N - 1); i++)
	{
		//
		float sum = 0;
		//this second loop adds the amplitudes of 'N' samples and stores the value in float 'sum'
		for (long j = 0; j < N; j++)
		{
			sum = sum + inputSignal[i + j];
		}
		inputSignal[i] = sum / N;
	}

	//returns a pointer to an array of floating point audio samples

	return inputSignal;
};
//----------Read Wave Files----------
void WAVINOUT::wavread()
{
	numSamples = 0;
	fs = 0;
	fileName = 0;
};
float* WAVINOUT::wavread(int &numSamples, int &fs, const char *fileName)
{
	// Open sound file
	//struct SF_INFO is assigned to sndInfo
	SF_INFO sndInfo;
	//string fileName is declared by user and iput as an argument for sf_open function 
	//the adress of sndInfo is passed into sf_open function by reference
	//SFM_READ is an enum of libsndfile (sound file mode read)

	SNDFILE *sndFile = sf_open(fileName, SFM_READ, &sndInfo);

	//if statement is used to display erreor if filename is invalid
	if (sndFile == NULL) {
		fprintf(stderr, "Error reading source file '%s': %s\n", fileName, sf_strerror(sndFile));
	}

	// if statement to check format - 16bit PCM
	if (sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16))
	{
		fprintf(stderr, "Input should be 16bit Wav\n");
		sf_close(sndFile);
	}

	// if statement to check channels - mono
	if (sndInfo.channels != 1) {
		fprintf(stderr, "Wrong number of channels\n");
		sf_close(sndFile);
	}

	// Allocate memory to an array of floats (length of sndInfo.frames) named buffer this is returned 
	// and declared as inputSignal in main  
	float* buffer = new float[sndInfo.frames];
	if (buffer == NULL) {
		fprintf(stderr, "Could not allocate memory for file\n");
		sf_close(sndFile);
	}

	// Load data
	//numFrames is returned from function
	long numFrames = sf_readf_float(sndFile, buffer, sndInfo.frames);

	// Check correct number of samples loaded
	if (numFrames != sndInfo.frames)
	{
		fprintf(stderr, "Did not read enough frames for source\n");
		sf_close(sndFile);
	}

	//numSamples is declared which shall be used throughout code

	numSamples = numFrames;

	// fs is declared as equal to an element of the struct sndInfo, samplerate, this shall be used 
	// throughout code 
	fs = sndInfo.samplerate;

	//close sound file
	sf_close(sndFile);

	//buffer is returned and assigned to inputSignal in main

	return buffer;
}

//----------Write Wave Files---------- 
void WAVINOUT::wavwrite()
{
	inputBuffer = 0;
	bufferLength = 0;
	fs = 0;
	fileNameOut = 0;
}
void WAVINOUT::wavwrite(float *inputBuffer, int bufferLength, int fs, const char *fileNameOut)
{
	// Set file settings, 16bit Mono PCM, sample frequency 44100

	SF_INFO info;
	info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	info.channels = 1;
	info.samplerate = fs;

	// Open sound file for writing
	// the libsndfile function sf_open is used to return file settings to sndfile
	//fileNameOut is declared by user and input as a function argument
	//SFM_WRITE is an enum of libsndfile
	//the adrress of struct info is input by reference
	SNDFILE *sndFile = sf_open(fileNameOut, SFM_WRITE, &info);
	if (sndFile == NULL) {
		fprintf(stderr, "Error opening sound file '%s': %s\n", fileNameOut, sf_strerror(sndFile));
	}

	// Write frames
	long writtenFrames = sf_writef_float(sndFile, inputBuffer, bufferLength);
	//
	// Check correct number of frames saved
	if (writtenFrames != bufferLength) {
		fprintf(stderr, "Did not write enough frames for source\n");
		sf_close(sndFile);
	}

	// Tidy up
	sf_write_sync(sndFile);
	sf_close(sndFile);
}