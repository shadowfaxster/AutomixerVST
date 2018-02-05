#pragma once

#ifndef __macros__
#define __macros__

#define MAX_CHANNELS 8 // maximum number of channels
#define MIN_CHANNELS 2 // min no of channels 2
#define MIN_WINDOW 1
#define MAX_WINDOW 256
#define MIN_ATTACK_MS 1
#define MAX_ATTACK_MS 15
#define MIN_RELEASE_MS 30
#define MAX_RELEASE_MS 300

//Contains macros and string conversion functions to be used with the GUI

#define host2param(a, b, v) v*(b - a) + a
#define param2host(a,b,v) (v-a)/(b-a)

void floatStringConvertMs(float value, char* string);

void DetectorTypePrint(float value, char* string);
void AnalogTCPrint(float value, char* string);

#endif
