#pragma once

#ifndef __AutomixerEditor__
#define __AutomixerEditor__

#ifndef __vstgui__
#include "../vstsdk2.4/vstgui.sf/vstgui/vstgui.h"
#endif

#ifndef __aeffgui__
#include "../vstsdk2.4/vstgui.sf/vstgui/aeffguieditor.h"
#endif


#include "AutomixerParameters.h"
#include "macros.h"

class AutomixerEditor : public AEffGUIEditor, public CControlListener
{
public:
	AutomixerEditor(AudioEffect* effect);
	virtual ~AutomixerEditor();

	//from AeffGUIEditor
	virtual bool open(void *ptr);
	virtual void close();

	virtual void setParameter(VstInt32 index, float value);

	//from CControlListener
	virtual void valueChanged(CControl* pControl);

private:
	CHorizontalSlider* nChannelsSlider;
	CHorizontalSlider* WindowSizeSlider;

	CAnimKnob* attackKnob;
	CAnimKnob* releaseKnob;

	CVerticalSwitch* DetectorTypeSwitch;
	CVerticalSwitch* AnalogTCSwitch;

	CParamDisplay* paramDisplay[6];

	CTextLabel* paramLabel[6];

	//Bitmap
	CBitmap* hBackground;
};

#endif //__AutomixerEditor__
