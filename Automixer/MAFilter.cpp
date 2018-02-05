#include "MAFilter.h"

#include <math.h>
#include <string.h>

MAFilter::MAFilter()
{
	// Can be overloaded to initialize straight with the desired MaxWindowSize 

	buffer = new float[MaxWindSize]; 
	memset(buffer, 0, sizeof(float)*MaxWindSize);

	readPtr = 0;
	currAverage = 0.f;
	sample_sum = 0;
	writePtr = 0;
}

MAFilter::MAFilter(UINT value)              // overloaded constructor for initalisation of the filter with a specific value;
{
	MaxWindSize = value;					
	buffer = new float[MaxWindSize];		
	memset(buffer, 0, sizeof(float)*MaxWindSize);

	readPtr = 0;
	currAverage = 0.f;
	sample_sum = 0;
	writePtr = 0;
}

MAFilter::~MAFilter()
{
	delete[] buffer;
	buffer = NULL;
}

void MAFilter::setWindowSize(UINT value)
{
	wSize = (value);
}

void MAFilter::addToAverage(float sample)
{
	buffer[writePtr] = sample;
	computeSampleSum();
	currAverage = sample_sum / wSize;
	writePtr++;
	readPtr++;

	if (writePtr >= MaxWindSize) writePtr = 0;
}

void MAFilter::computeSampleSum()
{
	readPtr = writePtr - wSize + 1;
	if (readPtr < 0) readPtr += MaxWindSize;  
	int readPtrCopy = readPtr;
	sample_sum = 0.f;

	for (int counter = wSize; counter > 0; counter--) 
	{
		sample_sum += buffer[readPtrCopy];
		readPtrCopy++;
		if (readPtrCopy >= MaxWindSize) readPtrCopy = 0;
	}
}



float MAFilter::getAverage()
{
	if (k==wSize) filterDelayReached = true; 
	else k++;

	if (!filterDelayReached) return 0.f;

	return currAverage;
}

void MAFilter::resumePlay()
{
	readPtr = 0;
	writePtr = 0;
	sample_sum = 0.f;
	this->clearBuffer();
	filterDelayReached = false;
	k = 0;
}

void MAFilter::clearBuffer()
{
	memset(buffer, 0, sizeof(float)*MaxWindSize);
}