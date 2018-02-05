/**
Moving Average Filter
*/

#pragma once

#ifndef MAFilterH
#define MAFilterH

#include <string.h>

#include "macros.h"


typedef unsigned int UINT;

class MAFilter
{
public:
	MAFilter();
	MAFilter(UINT value);		// constructor with a circular buffer of given size;
	~MAFilter ();

	void setWindowSize(UINT wSize);
	unsigned int getWindowSize() { return wSize; };

	void addToAverage(float sample);
	float getAverage(); //takes a parameter to check how many samples have been added so far

	void computeSampleSum();

	void setMaxWindowSize(UINT value) { MaxWindSize = value; };

	void resumePlay(); 
	void clearBuffer();

	bool isFilterDelayReached() { return filterDelayReached; }

private: 
	int readPtr, writePtr, MaxWindSize = MAX_WINDOW, k = 0;  // could be implemented straight as a static #define
	float* buffer;
	float currAverage, sample_sum;
	UINT wSize = 5;

	bool filterDelayReached = false;   // used to delay the 

};

#endif
