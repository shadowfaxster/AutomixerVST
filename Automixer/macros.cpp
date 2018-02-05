#include "macros.h"

#include <stdio.h>
#include <string.h>

//Contains macros and string conversion functions to be used with the GUI

void floatStringConvertMs(float value, char* string)
{
	sprintf(string, "%.2f ms", value);
}

void DetectorTypePrint(float value, char* string)
{
	if (!value) sprintf(string, "PEAK");
	else sprintf(string, "RMS");
}

void AnalogTCPrint(float value, char* string)
{
	if (!value) sprintf(string, "No");
	else sprintf(string, "Yes");
}