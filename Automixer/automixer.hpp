//
//  automixer.hpp
//  automixer-components
//
//  Created by Iustin Sandu on 20/11/2017.
//  Copyright © 2017 Iustin Sandu. All rights reserved.
//

#ifndef automixer_hpp
#define automixer_hpp


#include "../../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"


#include <stdio.h>
#include "CEnvelopeDetector.hpp"
#include "MAFilter.h"
#include "DeZipper.h"
#include "AutomixerParameters.h"
#include "macros.h"

#ifndef __AutomixerEditor__
#include "AutomixerEditor.h"
#endif


class automixer : public AudioEffectX
{
public:
	automixer(audioMasterCallback audioMaster);
	~automixer();

	//---from AudioEffect-----------------------
	virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);

	virtual void setProgramName(char* name);
	virtual void getProgramName(char* name);

	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char* label);
	virtual void getParameterDisplay(VstInt32 index, char* text);
	virtual void getParameterName(VstInt32 index, char* text);

	virtual void resume();

	virtual bool getEffectName(char* name);
	virtual bool getVendorString(char* text);
	virtual bool getProductString(char* text);
	virtual VstInt32 getVendorVersion() { return 1000; }

	virtual VstPlugCategory getPlugCategory() { return kPlugCategEffect; }

private:
	CEnvelopeDetector detector[MAX_CHANNELS + 1]; // +1 for the mix envelope
	MAFilter averager[MAX_CHANNELS];
	float in_envelopes[MAX_CHANNELS];

	unsigned int no_of_channels, windowSize = 1;
	bool multichannel = false; //multichannel option for testing; 
	char programName[kVstMaxProgNameLen+1];

	//temporary ad other values variables
	float temp;  //temporary var for Detector mode and AnalogTC (setParameter)

};

#endif

	