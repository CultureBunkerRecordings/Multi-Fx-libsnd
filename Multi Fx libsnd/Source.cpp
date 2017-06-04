#include <iostream>
#include <stdio.h>
#include <limits>
#include "functions.h"
#include <string>
#include <ctype.h>
using namespace std;

int main()
{

	/*--------------------------------------------------------------------------------------------------
	* PART 1: READING THE WAV FILE
	*/

	//Ask user for the path to a short duration mono wav file
	string filename;
	cout << "Please enter the full path of the .wav file to load: " << std::endl;
	getline(cin, filename);

	//Ask user for output path (should be here if you use std::cin>>)
	string outputFilename;
	cout << "Please enter the full path of the .wav file to write to: " << std::endl;
	getline(cin, outputFilename);

	WAVINOUT in;

	//numSamples holds the length of the wav file in samples
	int numSamples;

	//fs is the sampling frequency in Hz e.g. 44100
	int fs;

	/*wavread returns a pointer to an array of floating point audio samples into the variable
	inputSignal.
	int numSamples, int fs and string filename are input as function arguments */

	float* inputSignal = in.wavread(numSamples, fs, filename.c_str());

	//print the length of file in samples and sampling frequency
	cout << endl;
	cout << "This file has\t" << numSamples << "\t" << "samples." << std::endl;
	cout << endl;
	cout << "Sample frequency:\t" << fs << "Hz" << std::endl;
	cout << endl;

	/*------------------------------------------------------------------------------------------------
	* PART 2: AUDIO PROCESSING
	*/

	/* Some variables are initialised outside the scope of the do while loop*/
	int select = 0;
	float delayTime;
	int delayTimeInSamples = 0;
	float scale;
	int numToPrint;
	float threshold;
	int numOutputSamples;

	/* A do while loop is used to repeat processing while select < 8 */
	do
	{
		/* int numSamples must include the addition of the int delayedTimeinSamples
		int numOutputSamples is used in all functions after delay*/

		numOutputSamples = numSamples + delayTimeInSamples;

		/* the variable select is declared by user to choose the effect.
		int select is used in if else statements for each effect */

		cout << "\nPlease enter:\n1. for RMS\n2. for scale\n3. for delay\n4. for distortion\n5. for fade in\n6. for amplitude modulation" << endl;
		cout << "7. for lowpass filter\n8. to exit\n";

		cin >> select;

		if (select == 1)
		{
			//1. RMS. getRMS function is called.
			//getRMS returns a float (the Root Mean Square of all values within the aray inputSignal)

			PRINTOBJECT RMS;

			cout << "The RMS is\t" << RMS.getRMS(inputSignal, numOutputSamples) << endl;
			system("pause");
		}

		else if (select == 2)
		{
			// 2. scale the entire signal by a factor

			//do while loop is used to repeat question if char ans = 'n'

			char ans;

			do
			{
				/*int numToPrint is declared by user*/

				cout << "How many samples would you like to view?\n";
				cin >> numToPrint;
				cout << "Before scaling\n";

				/* printsamples function is called to display samples to screen before scaling
				printsamples function return is void
				float* inputSignal and int numToPrint are input as function arguments */
				PRINTOBJECT print;
				print.printsamples(inputSignal, numToPrint);
				cout << endl;
				/*float scale is declared by user*/

				cout << "By what factor would you like to scale the samples?\n";
				cin >> scale;

				/* scaleSignal function is called.
				scaleSignal function return is void
				float* inputSignal, int numSamples and float scale are input as function arguments*/
				FX scaler;
				scaler.scaleSignal(inputSignal, numOutputSamples, scale);
				cout << "After scaling\n";
				print.printsamples(inputSignal, numToPrint);
				cout << "continue?(y/n)";
				cin >> ans;

			} while (ans == 'n');
		}



		else if (select == 3)
		{
			//3. Delay, a do while loop is used to repeat the question if delay too long  
			do
			{
				//float delayTime is delcared by user(in seconds)
				cout << "How long a delay time would you like to apply? (secs)\n";
				cin >> delayTime;

				//delayTimeInSamples is needed for function this is worked out by delayTime*fs

				delayTimeInSamples = long int(delayTime * fs);
				//if statement is used to display error message
				if (delayTimeInSamples > numSamples)
				{
					cout << "Error delay too long";
					system("pause");
				}
			} while (delayTimeInSamples > numSamples);

			/* getSignalWithDelay function is called. float* inputSignal,
			int numSamples and int delayTimeInSamples are input as arguments*/
			FX delayer;
			inputSignal = delayer.getSignalWithDelay(inputSignal, numSamples, delayTimeInSamples);


		}
		else if (select == 4)
		{
			//4. Distortion. float threshold is declared by user 

			cout << "Enter a threshold for distortion\n";
			cin >> threshold;

			/*getSignalWithDistortion function is called.
			getSignalWithDistortion returns a floating point array into inputSignal.
			float* inputSignal, int numOutputSamples, float threshold are input as function arguments*/
			FX distorter;
			inputSignal = distorter.getSignalWithDistortion(inputSignal, numOutputSamples, threshold);
		}

		else if (select == 5)
		{
			/*5. Fade in.  float fadeTime is declared by user */
			float fadeTime;
			int fadeTimeInSamples;
			//A do while loop is used to repeat the question if the fadein is longer than the file.

			do
			{
				cout << "How long a fade in would you like to apply?(secs)\n";
				cin >> fadeTime;

				//first using this equation 'fadeTime' in seconds is converted into samples and stored in 
				//the variable fadeTimeInSamples

				fadeTimeInSamples = int(fadeTime * fs);

				//An if statement if used to display the error message

				if (fadeTimeInSamples > numOutputSamples)
				{
					cout << "Error, fade too long\n";
					system("pause");
				}
			} while (fadeTimeInSamples > numOutputSamples);

			/* getSignalWithFadein function is called.
			getSignalWithFadein returns a floating point array into inputSignal
			float* inputSignal and int fadeTimeinSamples are input as function arguments */
			FX fader;
			inputSignal = fader.getSignalWithFadein(inputSignal, fadeTimeInSamples);

		}

		else if (select == 6)
		{
			/*6. Amplitude modulation. int modfreq is declared by user*/
			int modfreq;
			cout << "Enter a frequency for amplitude modulation.\n";
			cin >> modfreq;

			/* getSignalWithAmpmod function is called.
			getSignalWithAmpmod returns a floating point array into inputSignal
			float* inputSignal, numOutputSamples and modfreq are input as function arguments*/
			FX modulator;
			inputSignal = modulator.getSignalWithAmpmod(inputSignal, numOutputSamples, modfreq);
		}

		else if (select == 7)
		{
			/*7. Lowpass filter. ans2 is declared by user*/
			char ans2;

			cout << "Would you like to apply a lowpass filter?\n";
			cin >> ans2;

			/* if statement is used to determine if user wants to apply a lowpass filter*/

			if (ans2 == 'y')
			{
				/* getSignalWithFilter function is called.
				getSignalWithFilter returns a floating point array into inputSignal
				float* inputSignal and numOutputSamples are input as function arguments*/
				FX filterer;
				inputSignal = filterer.getSignalWithFilter(inputSignal, numOutputSamples);
			}
		}
	} while (select<8);


	/*-------------------------------------------------------------------------------------------------
	* PART 3: WRITE THE PROCESSED AUDIO OUT
	*/
	WAVINOUT out;

	/* wavwrite function is called
	wavwrite return is void
	float* inputSignal, numOutputSamples, fs, OutputFilename are input as function arguments*/

	out.wavwrite(inputSignal, numOutputSamples, fs, outputFilename.c_str());

	//The floating point array inputSignal is deleted to free memory before closing program
	//system function is called to pause screen

	
}