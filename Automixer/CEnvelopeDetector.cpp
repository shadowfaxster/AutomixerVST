//
//  CEnvelopeDetector.cpp
//  automixer-components
//  Following CEnvelopeDetector in Will Pirkle's RackAFX
//
//  Created by Iustin Sandu on 20/11/2017.
//  Copyright © 2017 Iustin Sandu. All rights reserved.
//

#include "CEnvelopeDetector.hpp"
#include "math.h"

CEnvelopeDetector::CEnvelopeDetector()
{
    // Initialise all variables in case user does not
    m_fAttackTime_mSec = 1;
    m_fReleaseTime_mSec = 15;
	m_fSampleRate = 44100;
    m_fEnvelope = 0.0;
    m_uDetectMode = 0;
    m_nSample = 0;
    m_bAnalogTC = false;
    m_bLogDetector = false;

	setAttackTime(m_fAttackTime_mSec);
	setReleaseTime(m_fReleaseTime_mSec);
}

CEnvelopeDetector::~CEnvelopeDetector()
{
    
}

void CEnvelopeDetector::prepareforplay()
{
    m_fEnvelope = 0.0f;
    m_nSample = 0.0f;
}


void CEnvelopeDetector::init(float samplerate, float attack_in_ms, float release_in_ms, bool bAnalogTC, UINT uDetect, bool bLogDetector)
{
    m_fEnvelope = 0.0;
    m_fSampleRate = samplerate;
    m_bAnalogTC = bAnalogTC;
    m_fAttackTime_mSec = attack_in_ms;
    m_fReleaseTime_mSec = release_in_ms;
    m_uDetectMode = uDetect;
    m_bLogDetector = bLogDetector;
    
    // set themm_uDetectMode = uDetect;
    setAttackTime(attack_in_ms);
    setReleaseTime(release_in_ms);
}


void CEnvelopeDetector::setAttackTime(float attack_in_ms)
{
    m_fAttackTime_mSec = attack_in_ms;
    
    if(m_bAnalogTC)
        m_fAttackTime = exp(ANALOG_TC/( attack_in_ms * m_fSampleRate * 0.001));
    else
        m_fAttackTime = exp(DIGITAL_TC/( attack_in_ms * m_fSampleRate * 0.001));
}


void CEnvelopeDetector::setReleaseTime(float release_in_ms)
{
    m_fReleaseTime_mSec = release_in_ms;
    
    if(m_bAnalogTC)
        m_fReleaseTime = exp(ANALOG_TC/( release_in_ms * m_fSampleRate * 0.001));
    else
        m_fReleaseTime = exp(DIGITAL_TC/( release_in_ms * m_fSampleRate * 0.001));
}

void CEnvelopeDetector::setTCModeAnalog(bool bAnalogTC)
{
    m_bAnalogTC = bAnalogTC;
    setAttackTime(m_fAttackTime_mSec);
    setReleaseTime(m_fReleaseTime_mSec);
}

float CEnvelopeDetector::detect(float fInput)
{
    switch(m_uDetectMode)
    {
        case 0:
            fInput = fabs(fInput);
            break;
        case 1:
        case 2: // --- both MS and RMS require squaring the input
            fInput = fabs(fInput) * fabs(fInput);
            break;
        default:
            fInput = (float)fabs(fInput);
            break;
    }
    
    float fCurrEnvelope = 0.0;

    if(fInput> m_fEnvelope)
        fCurrEnvelope = m_fAttackTime * (m_fEnvelope - fInput) + fInput;
    else
        fCurrEnvelope = m_fReleaseTime * (m_fEnvelope - fInput) + fInput;
    
    if(fCurrEnvelope > 0.0 && fCurrEnvelope < FLT_MIN_PLUS) fCurrEnvelope = 0;
    if(fCurrEnvelope < 0.0 && fCurrEnvelope > FLT_MIN_MINUS) fCurrEnvelope = 0;
    
    // --- bound them; can happen when using pre-detector gains of more than 1.0
    fCurrEnvelope = fmin(fCurrEnvelope, (float)1.0);
    fCurrEnvelope = fmax(fCurrEnvelope, (float)0.0);
    
    // --- store envelope prior to sqrt for RMS version
    m_fEnvelope = fCurrEnvelope;
    
    // --- if RMS, do the SQRT
    if(m_uDetectMode == 2)
        fCurrEnvelope =  pow(fCurrEnvelope, (float)0.5);
    
    // --- 16-bit scaling!
    if(m_bLogDetector)
    {
        if(fCurrEnvelope <= 0)
            return -96.0; // 16 bit noise floor
        
        return 20*log10(fCurrEnvelope);
    }
    
    return fCurrEnvelope;
}
