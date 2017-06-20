
#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
// The header file is used to link the source.cpp and functions.cpp
// It contains the function declarations

#include <iostream>
using namespace std;

class PRINTOBJECT
{
public:
	PRINTOBJECT()
		:inputSignal(0), numOutputSamples(0), numToPrint(0)
	{
		cout << "object created..\n";
	}
	~PRINTOBJECT() 
	{ 
		cout << "object deleted..\n";
	}

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
	FX()
		:inputSignal(0), numOutputSamples(0), modfreq(0), fadeTimeInSamples(0), threshold(0), scale(0), delayTimeInSamples(0)
	{
		cout << "object created..\n";
	}

	~FX()
	{
		cout << "object deleted\n";

	}

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
		:bufferLength(0), inputBuffer(0), fs(0), numSamples(0)
	{
		//add initialisations here...
		cout << "object created..\n";
		
	}

	~WAVINOUT()
	{
		cout << "object deleted..\n";
		delete[] inputBuffer;
	}

	float* wavread(int&, int&, const char*);
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
