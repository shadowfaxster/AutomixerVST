#include "AutomixerEditor.h"

#include <stdio.h>

enum {
	//positions - taken from actual graphic in GIMP - see resource folder

	//top row controls 
	kSlider1X = 20,

	kSwitch1X = 126,
	kSwitch2X = 200,

	kSliderY = 40,
	kSwitchY = 23,

	// bottom row controls
	kKnob1X = 40,

	kBottomRowY = 110,

	//Parameter label
	kLabelYOffset = -28,   //slider offset
	kLabelYOffsetSwitch = -10,
	kLabelHeight = 15,

	//Parameter value display 
	kParamDispWidth = 30,
	kParamDispHeight = 14
};

enum {

	//parameter reference numbers to call as indices in display array
	indDetectorType = 0,
	indAnalogTC = 1,
	indNumChannels,
	indMAWindowSize,
	indAttackMs,
	indReleaseMs
};

//-------------------------------------------------------
AutomixerEditor::AutomixerEditor(AudioEffect* effect) : AEffGUIEditor(effect)
{
	hBackground = new CBitmap("background.png"); // set plugin background colour;

	//init the size of the plugin
	rect.left = 0;
	rect.top = 0;
	rect.right = (short)hBackground->getWidth();
	rect.bottom = (short)hBackground->getHeight();
}

AutomixerEditor::~AutomixerEditor()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget();
	hBackground = 0;
}

bool AutomixerEditor::open(void* ptr)
{

	AEffGUIEditor::open(ptr);

	//load bitmaps:
	CBitmap* hSliderBody = new CBitmap("hsbackS700.png");
	CBitmap* hSliderHandle = new CBitmap("hshandS700.png");

	CBitmap* hKnob = new CBitmap("fxknob.png");

	CBitmap* hSwitch = new CBitmap("bigtoggleswitch.png");

	CBitmap* hLabelBg = new CBitmap("labelbg.png");


	CRect size(0, 0, hBackground->getWidth(), hBackground->getHeight());
	CFrame* lFrame = new CFrame(size, ptr, this);
	lFrame->setBackground(hBackground);

	//init controls 
	int minPos = kSlider1X;
	int maxPos = kSlider1X + hSliderBody->getWidth() - hSliderHandle->getWidth() - 1;
	int minPos1, maxPos1; //min/max pos for the second slider

	CPoint point(0, 0);
	CPoint offset(1, 0);

	//temporary variables
	float newX, newY; //position variables
	float knobHeight = hKnob->getHeight() / 80; //for knob with multiple pixmaps

	//--------------------------------------------- Detector Type -------------------------------------

	//Detector Switch
	size(kSwitch1X, kSwitchY, kSwitch1X + hSwitch->getWidth(), kSwitchY + hSwitch->getHeight() / 2 - 8);								// substract extra 8 px so the switch rectangle doesn't go over the param display 
	DetectorTypeSwitch = new CVerticalSwitch(size, this, kDetectorType, 2, 
		hSwitch->getHeight() / 2, 2, hSwitch, point);
	DetectorTypeSwitch->setValue(effect->getParameter(kDetectorType));
	lFrame->addView(DetectorTypeSwitch);

	//Detector label   ---- need to do these after each parameter as it's easier to offset the SIZE rectangle relative to the respective param position
	size.setHeight(kLabelHeight);
	size.offset(0, kLabelYOffsetSwitch);
	paramLabel[indDetectorType] = new CTextLabel(size, "Det. Type", hLabelBg, kCenterText);
	paramLabel[indDetectorType]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indDetectorType]);

	//Detector value display
	newY = kSwitchY - 60.f + hSwitch->getHeight();	// this will be used as the Y coord for all top row controls
	size(kSwitch1X, newY, kSwitch1X + hSwitch->getWidth(), newY + kParamDispHeight);
	paramDisplay[indDetectorType] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indDetectorType]->setFont(kNormalFontSmaller);
	paramDisplay[indDetectorType]->setBackColor(kGreyCColor);
	paramDisplay[indDetectorType]->setValue(effect->getParameter(kDetectorType));
	paramDisplay[indDetectorType]->setStringConvert(DetectorTypePrint);
	lFrame->addView(paramDisplay[indDetectorType]);


	//-------------------------ANALOG TC---------------------------

	//AnalogTC switch
	size(kSwitch2X, kSwitchY, kSwitch2X + hSwitch->getWidth(), kSwitchY + hSwitch->getHeight() / 2 - 8); // substract extra 8 px so the switch rectangle doesn't go over the param display 
	AnalogTCSwitch = new CVerticalSwitch(size, this, kAnalogTC, 2, hSwitch->getHeight() / 2, 2, hSwitch, point);
	AnalogTCSwitch->setValue(effect->getParameter(kAnalogTC));
	lFrame->addView(AnalogTCSwitch);

		//Analog Switch Label
	size.setHeight(kLabelHeight);
	size.offset(0, kLabelYOffsetSwitch);
	paramLabel[indDetectorType] = new CTextLabel(size, "AnalogTC", hLabelBg, 1);
	paramLabel[indDetectorType]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indDetectorType]);

		//AnalogTC Switch Display
	size(kSwitch2X, newY, kSwitch2X + hSwitch->getWidth(), newY + kParamDispHeight);  
	paramDisplay[indAnalogTC] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indAnalogTC]->setFont(kNormalFontSmaller);
	paramDisplay[indAnalogTC]->setBackColor(kGreyCColor);
	paramDisplay[indAnalogTC]->setValue(effect->getParameter(kAnalogTC));
	paramDisplay[indAnalogTC]->setStringConvert(AnalogTCPrint);
	lFrame->addView(paramDisplay[indAnalogTC]);


	//------------------------------- NUMBER OF CHANNELS SLIDER------------------------------

	//Number of channels
	size(kSlider1X, kSliderY,
		kSlider1X + hSliderBody->getWidth(), kSliderY + hSliderBody->getHeight());
	nChannelsSlider = new CHorizontalSlider(size, this, kNumChannels, minPos, maxPos, hSliderHandle, hSliderBody, point, kLeft);
	nChannelsSlider->setOffsetHandle(offset);
	nChannelsSlider->setValue(effect->getParameter(kNumChannels));
	lFrame->addView(nChannelsSlider);

		// Number of channels label
	size.offset(0, kLabelYOffset);
	paramLabel[indNumChannels] = new CTextLabel(size, "No. of ch", hLabelBg, 1);
	paramLabel[indNumChannels]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indNumChannels]);

		//Number of channels display
	size(kSlider1X, newY, kSlider1X + hSliderBody->getWidth(), newY + kParamDispHeight);
	paramDisplay[indNumChannels] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indNumChannels]->setFont(kNormalFontSmaller);
	paramDisplay[indNumChannels]->setBackColor(kGreyCColor);
	paramDisplay[indNumChannels]->setValue(host2param(MIN_CHANNELS, MAX_CHANNELS, (effect->getParameter(kNumChannels))));
	lFrame->addView(paramDisplay[indNumChannels]);


	//-------------------------------------- WINDOW SIZE SLIDER ---------------------------------------

	//Window Size
	newX = hBackground->getWidth() - 20 - hSliderBody->getWidth();
	size(newX, kSliderY, newX + hSliderBody->getWidth(), kSliderY + hSliderBody->getHeight());
	minPos1 = (size.getTopLeft()).x;
	maxPos1 = (size.getTopRight()).x-hSliderHandle->getWidth()-1;
	WindowSizeSlider = new CHorizontalSlider(size, this, kMAWindowSize, minPos1, maxPos1, hSliderHandle, hSliderBody, point, kLeft);
	WindowSizeSlider->setOffsetHandle(offset);
	WindowSizeSlider->setValue(effect->getParameter(kMAWindowSize));
	lFrame->addView(WindowSizeSlider);

		//Window Size Label
	size.offset(0, kLabelYOffset);
	paramLabel[indMAWindowSize] = new CTextLabel(size, "WindowSize", hLabelBg, 1);
	paramLabel[indMAWindowSize]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indMAWindowSize]);

		//Window Size Display
	size(newX, newY, newX + hSliderBody->getWidth(), newY + kParamDispHeight);       // newX is the calculated X point for the WindowSize SLider
	paramDisplay[indMAWindowSize] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indMAWindowSize]->setFont(kNormalFontSmaller);
	paramDisplay[indMAWindowSize]->setBackColor(kGreyCColor);
	paramDisplay[indMAWindowSize]->setValue((1000.0f/(effect->getSampleRate())) * host2param(MIN_WINDOW, MAX_WINDOW, (effect->getParameter(kMAWindowSize))));
	paramDisplay[indMAWindowSize]->setStringConvert(floatStringConvertMs);
	lFrame->addView(paramDisplay[indMAWindowSize]);


	//-------------------------------------- ATTACK TIME --------------------------------------

	//Attack time
	size(kKnob1X, kBottomRowY, kKnob1X + hKnob->getWidth(), kBottomRowY + knobHeight);
	attackKnob = new CAnimKnob(size, this, kAttackMs, 80, knobHeight, hKnob, point);
	attackKnob->setValue(effect->getParameter(kAttackMs));
	lFrame->addView(attackKnob);

		//Attack Time Label
	size.offset(0, kLabelYOffset);
	paramLabel[indAttackMs] = new CTextLabel(size, "Attack", hLabelBg, 1);
	paramLabel[indAttackMs]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indAttackMs]);

		//Attack Time Display
	newY = kBottomRowY + knobHeight;
	size(kKnob1X, newY, kKnob1X + hKnob->getWidth(), newY + kParamDispHeight);       
	paramDisplay[indAttackMs] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indAttackMs]->setFont(kNormalFontSmaller);
	paramDisplay[indAttackMs]->setBackColor(kGreyCColor);
	paramDisplay[indAttackMs]->setValue(host2param(MIN_ATTACK_MS, MAX_ATTACK_MS, (effect->getParameter(kAttackMs))));
	paramDisplay[indAttackMs]->setStringConvert(floatStringConvertMs);
	lFrame->addView(paramDisplay[indAttackMs]);


	//------------------------------- RELEASE TIME ----------------------------------------

	//Release time knob
	newX = hBackground->getWidth() - 40 - hKnob->getWidth();
	size(newX, kBottomRowY, newX + hKnob->getWidth(), kBottomRowY + knobHeight);
	releaseKnob = new CAnimKnob(size, this, kReleaseMs, 80, knobHeight, hKnob, point);
	releaseKnob->setValue(effect->getParameter(kReleaseMs));
	lFrame->addView(releaseKnob);

		//Release time label
	size.offset(0, kLabelYOffset);
	paramLabel[indReleaseMs] = new CTextLabel(size, "Release", hLabelBg, 1);
	paramLabel[indReleaseMs]->setFont(kNormalFontSmall);
	lFrame->addView(paramLabel[indReleaseMs]);

		//Release time display
	size(newX - 5, newY, newX + hKnob->getWidth() + 5, newY + kParamDispHeight);       
	paramDisplay[indReleaseMs] = new CParamDisplay(size, hLabelBg, kCenterText);
	paramDisplay[indReleaseMs]->setFont(kNormalFontSmaller);
	paramDisplay[indReleaseMs]->setBackColor(kGreyCColor);
	paramDisplay[indReleaseMs]->setValue(host2param(MIN_RELEASE_MS, MAX_RELEASE_MS, (effect->getParameter(kReleaseMs))));
	paramDisplay[indReleaseMs]->setStringConvert(floatStringConvertMs);
	lFrame->addView(paramDisplay[indReleaseMs]);
	

	//Switch displays

	hSliderBody->forget();
	hSliderHandle->forget();
	hSwitch->forget();
	hKnob->forget();

	frame = lFrame;

	return true;
}

//----------------------------------------------------------------------------------------
void AutomixerEditor :: close()
{
	delete frame;
	frame = 0;
}

//----------------------------------------------------------------------------------------
void AutomixerEditor::valueChanged(CControl* pControl)
{
	long tag = pControl->getTag();
	switch (tag)
	{
	case kNumChannels:
	case kMAWindowSize:
	case kDetectorType:
	case kAttackMs:
	case kReleaseMs:
	case kAnalogTC:
		effect->setParameterAutomated(tag, pControl->getValue());
		pControl->setDirty();
	break;
	}

}

void AutomixerEditor::setParameter(VstInt32 index, float value)
{
	if (frame == 0)
		return;

	switch (index)
	{
	case kNumChannels:
		if (nChannelsSlider)
			nChannelsSlider->setValue(effect->getParameter(index));
		if (paramDisplay[indNumChannels])
			paramDisplay[indNumChannels]->setValue(host2param(MIN_CHANNELS,MAX_CHANNELS,(effect->getParameter(index))));
		break;

	case kMAWindowSize:
		if (WindowSizeSlider)
			WindowSizeSlider->setValue(effect->getParameter(index));
		if (paramDisplay[indMAWindowSize])
			paramDisplay[indMAWindowSize]->setValue((1000.0f / (effect->getSampleRate())) * host2param(MIN_WINDOW, MAX_WINDOW, (effect->getParameter(index))));
		break;

	case kDetectorType:
		if (DetectorTypeSwitch)
			DetectorTypeSwitch->setValue(effect->getParameter(index));
		if (paramDisplay[indDetectorType])
			paramDisplay[indDetectorType]->setValue(effect->getParameter(index));
		break;

	case kAttackMs:
		if (attackKnob)
			attackKnob->setValue(effect->getParameter(index));
		if (paramDisplay[indAttackMs])
			paramDisplay[indAttackMs]->setValue(host2param(MIN_ATTACK_MS, MAX_ATTACK_MS, (effect->getParameter(index))));
		break;

	case kReleaseMs:
		if (releaseKnob)
			releaseKnob->setValue(effect->getParameter(index));
		if (paramDisplay[indReleaseMs])
			paramDisplay[indReleaseMs]->setValue(host2param(MIN_RELEASE_MS, MAX_RELEASE_MS, (effect->getParameter(index))));
		break;

	case kAnalogTC:
		if (AnalogTCSwitch)
			AnalogTCSwitch->setValue(effect->getParameter(index));
		if (paramDisplay[indAnalogTC])
			paramDisplay[indAnalogTC]->setValue(effect->getParameter(index));
		break;
	}

}