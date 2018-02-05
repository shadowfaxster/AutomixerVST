#pragma once

#ifndef __AutomixerParameters__
#define __AutomixerParameters__

enum AutomixerParameters
{
	//Parameter Tags
	kNumChannels = 0,
	kMAWindowSize,
	kAttackMs,
	kReleaseMs,
	kDetectorType,
	kAnalogTC,

	kNumParams

	/*the order of the parameters is different from the order in the gui. This is to allow 
	for the efficient use of the Dezipper array and not have empty memory*/
};


#endif //__AutomixerParameters__
