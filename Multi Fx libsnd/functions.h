
#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
// The header file is used to link the source.cpp and functions.cpp
// It contains the function declarations

class PRINTOBJECT
{
public:

	float getRMS(float*, int);
	void printsamples(float*, int);

private:
	float* inputSignal;
	int numOutputSamples;
	int numToPrint;
};

class FX
{

public:
	float* getSignalWithFilter(float*, int);
	float* getSignalWithAmpmod(float*, int, int);
	float* getSignalWithFadein(float*, int);
	float* getSignalWithDistortion(float*, int, float);
	void scaleSignal(float*, int, float);
	float* getSignalWithDelay(float*, int, int);
private:
	float*inputSignal;
	int numOutputSamples;
	int modfreq;
	int fadeTimeInSamples;
	float threshold;
	float scale;
	int delayTimeInSamples;

};

class WAVINOUT
{
public:
	// these are the bits I added
	WAVINOUT()
	{
		//add initialisations here...
		numSamples = 0;
		fs = 0;
		
	}

	~WAVINOUT()
	{
		delete[] inputBuffer;
	}

	void wavread();
	float* wavread(int&, int&, const char*);
	void wavwrite();
	void wavwrite(float *, int, int, const char *);

private:
	const char *fileName;
	const char *fileNameOut;
	int bufferLength;
	float *inputBuffer;
	int fs;
	int numSamples;
};






#endif
