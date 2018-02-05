//
//  automixer.cpp
//  automixer-components
//
//  Created by Iustin Sandu on 20/11/2017.
//  Copyright © 2017 Iustin Sandu. All rights reserved.
//

#include "automixer.hpp"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new automixer(audioMaster);
}

//-----------------------------------------------------------------------------------------

automixer::automixer(audioMasterCallback audioMaster) 
	: AudioEffectX (audioMaster, 1, kNumParams) 
{
	//init
	setNumInputs(MAX_CHANNELS); // 8 channels in
	setNumOutputs(MAX_CHANNELS);			// mono out
	setUniqueID('atmx'); 		// identify
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen); //default programe name
	
	//create gui editor
	editor = new AutomixerEditor(this);

	//parameters initialization
	no_of_channels = 2; // default 2 channel automixer

	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		averager[i].setWindowSize(1); //initialise all averagers with a window size of 1 to work on sample by default;
		detector[i].init(getSampleRate(), MIN_ATTACK_MS, MIN_RELEASE_MS, false, 0, false);
	}
	detector[MAX_CHANNELS].init(getSampleRate(), MIN_ATTACK_MS, MIN_RELEASE_MS, false, 0, false); //initialize the final detector for the sum. 
	
}

//-----------------------------------------------------------------------------------------

automixer::~automixer()
{
	
}

//-----------------------------------------------------------------------------------------

void automixer::setProgramName(char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------

void automixer::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------

void automixer::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case kMAWindowSize:
		windowSize = int(host2param(MIN_WINDOW, MAX_WINDOW, value));
		for (int i = 0; i < no_of_channels; i++)
		{
			averager[i].setWindowSize(windowSize);
		}
		break;

	case kNumChannels:
		no_of_channels = host2param(MIN_CHANNELS, MAX_CHANNELS,(value));   //here the min and max channels need to be written
		break;

	case kDetectorType:
		temp = int(host2param(0, 2, value));
		if (temp == 1) temp = 2;				//no MS detection mode, only PEAK and RMS; 
		for (int i=0; i <= no_of_channels; i++)
		{
			detector[i].setDetectMode(temp);
		}
		break;
		
	case kAttackMs:
		for (int i=0; i <= no_of_channels; i++)
		{
			detector[i].setAttackTime(host2param(MIN_ATTACK_MS, MAX_ATTACK_MS, (value)));
		}
		break;

	case kReleaseMs:
		for (int i=0; i <= no_of_channels; i++)
		{
			detector[i].setReleaseTime(host2param(MIN_RELEASE_MS, MAX_RELEASE_MS, (value)));
		}
		break;

	case kAnalogTC:
		for (int i=0; i <= no_of_channels; i++)
		{
			detector[i].setTCModeAnalog(int(value));
		}
		break;
	}
	if (editor)
		((AEffGUIEditor*)editor)->setParameter(index, value);
}

//-----------------------------------------------------------------------------------------

float automixer::getParameter(VstInt32 index)
{
	float x;
	switch (index)
	{
	case kMAWindowSize:
		return param2host(float(MIN_WINDOW), float(MAX_WINDOW), windowSize);
		break;

	case kNumChannels:
		return param2host(2.f, 8.f, no_of_channels);  // min and max no of channels input manually; 
		break;

	case kDetectorType:
		return param2host(0, 2, detector[0].getDetectorType());
		break;

	case kAttackMs:
		return param2host(MIN_ATTACK_MS, MAX_ATTACK_MS, detector[0].getAttackTime());
		break;

	case kReleaseMs:
		x = param2host(MIN_RELEASE_MS, MAX_RELEASE_MS, detector[0].getReleaseTime());
		return x;
		break;

	case kAnalogTC:
		return param2host(0, 1, detector[0].getTCModeAnalog());
		break;
	}
}

//------------------------------------------------------------------------

void automixer::getParameterName (VstInt32 index, char* label)
{
	switch (index)
	{
	case kMAWindowSize:
		vst_strncpy(label, "Window Size", kVstMaxParamStrLen);
		break;

	case kNumChannels:
		vst_strncpy(label, "No-of-ch", kVstMaxParamStrLen);
		break;

	case kDetectorType:
		vst_strncpy(label, "Detector-Type", kVstMaxParamStrLen);
		break;

	case kAttackMs:
		vst_strncpy(label, "AttackTime", kVstMaxParamStrLen);
		break;

	case kReleaseMs:
		vst_strncpy(label, "ReleaseTime", kVstMaxParamStrLen);
		break;

	case kAnalogTC:
		vst_strncpy(label, "AnalogTC", kVstMaxParamStrLen);
		break;
	}

}

void automixer::resume()
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		detector[i].prepareforplay();
		averager[i].resumePlay();
	}
	detector[MAX_CHANNELS].prepareforplay();       // always remember the extra detector for the sumed mix of all channels;
}

//------------------------------------------------------------------------

void automixer::getParameterDisplay (VstInt32 index, char* text)
{
	unsigned int temp2;

	switch (index)
	{
	case kMAWindowSize:
		int2string(windowSize, text, kVstMaxParamStrLen);
		break;

	case kNumChannels:
		int2string(no_of_channels, text, kVstMaxParamStrLen);
		break;

	case kDetectorType:
		temp2 = detector[0].getDetectorType();
		switch (temp2)
		{
		case 0:
			vst_strncpy(text, "PEAK", kVstMaxParamStrLen);
			break;
		case 1:
			vst_strncpy(text, "MS", kVstMaxParamStrLen);
			break;
		case 2:
			vst_strncpy(text, "RMS", kVstMaxParamStrLen);
			break;
		}
		break;

	case kAttackMs:
		float2string(detector[0].getAttackTime(), text, kVstMaxParamStrLen);
		break;

	case kReleaseMs:
		float2string(detector[0].getReleaseTime(), text, kVstMaxParamStrLen);
		break;

	case kAnalogTC:
		temp2 = detector[0].getTCModeAnalog();
		switch (temp2)
		{
		case 0:
			vst_strncpy(text, "No", kVstMaxParamStrLen);
			break;
		case 1:
			vst_strncpy(text, "Yes", kVstMaxParamStrLen);
			break;
		}
		break;
	}
}

//------------------------------------------------------------------------

void automixer::getParameterLabel (VstInt32 index, char* label)
{
	switch (index)
	{

	case kMAWindowSize:
		vst_strncpy(label, "samples", kVstMaxParamStrLen);
		break;

	case kNumChannels:
		vst_strncpy(label, "", kVstMaxParamStrLen);
		break;

	case kDetectorType:
		vst_strncpy(label, "", kVstMaxParamStrLen);
		break;
	
	case kAttackMs:
	case kReleaseMs:
		vst_strncpy(label, "ms", kVstMaxParamStrLen);
		break;

	case kAnalogTC:
		vst_strncpy(label, "", kVstMaxParamStrLen);
		break;
	}
}

//------------------------------------------------------------------------

bool automixer::getEffectName (char* name)
{
	vst_strncpy (name, "AutomixerMultiChannel", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------

bool automixer::getProductString (char* text)
{
	vst_strncpy(text, "ATMX", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------

bool automixer::getVendorString (char* text)
{
	vst_strncpy (text, "IDSTech", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------

void automixer::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float mix, outputMix, mix_env, temp_env;
	
	for (int i = 0; i < sampleFrames; i++)
	{
		mix = 0.f;

		// Envelope detection
		for (int j = 0; j < no_of_channels; j++)
		{
			averager[j].addToAverage(detector[j].detect(inputs[j][i]));	        //detect sample i of channel j and add it to the moving average for channel j
			in_envelopes[j] = averager[j].getAverage();                        //need to send the sample number i as a param so the filter starts returning the averages when it has been given enough samples to compute the average
			mix += inputs[j][i];                                                // add above sample to the overall mix for later detection
		} 

		mix_env = detector[no_of_channels].detect(mix);	                        // compute the envelopes for sample i of all channels mixed together; 
		
		// Attenuation and output
		outputMix = 0.f;
		for (int j = 0; j < no_of_channels; j++)
		{
			if (multichannel)	outputs[j][i] = inputs[j][i] * (in_envelopes[j] / mix_env);       //for multichannel output each sample/channel with respective attenuation

			else outputMix += inputs[j][i] * (in_envelopes[j] / mix_env);						 //for mono output 
		}
		if (!multichannel) outputs[0][i] = outputMix;

	}
}

	