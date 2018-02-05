//
//  CEnvelopeDetector.hpp
//  automixer-components
//  Based on CEnvelopeDetector in Will Pirkle's RackAFX
//
//  Created by Iustin Sandu on 20/11/2017.
//  Copyright © 2017 Iustin Sandu. All rights reserved.
//

#ifndef CEnvelopeDetector_hpp
#define CEnvelopeDetector_hpp

typedef unsigned int        UINT;

#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */
#define FLT_MIN_MINUS        -1.175494351e-38         /* min negative value */

#include <stdio.h>

const float DIGITAL_TC = -4.60517019; // ln(1%)
const float ANALOG_TC =  -1.00239343; // ln(36.7%)
const float METER_UPDATE_INTERVAL_MSEC = 50.0;
const float RAFX_METER_UPDATE_INTERVAL_MSEC = 50.0; //125.0;
const float METER_MIN_DB = -60.0;
enum DType {PEAK, MS, RMS};

class CEnvelopeDetector

{
public:
    CEnvelopeDetector(void);
    ~CEnvelopeDetector(void);
    
    // Call the Init Function to initialize and setup all at once; this can be called as many times
    // as you want
    void init(float samplerate, float attack_in_ms, float release_in_ms, bool bAnalogTC, UINT uDetect, bool bLogDetector);
    
    // these functions allow you to change modes and attack/release one at a time during
    // realtime operation
    void setTCModeAnalog(bool bAnalogTC); // {m_bAnalogTC = bAnalogTC;}
	bool getTCModeAnalog() { return m_bAnalogTC; }
    
    // THEN do these after init
    void setAttackTime(float attack_in_ms);
	float getAttackTime() { return m_fAttackTime_mSec; }
    void setReleaseTime(float release_in_ms);
	float getReleaseTime() { return m_fReleaseTime_mSec; }
    
    void setDetectMode(UINT uDetect) {m_uDetectMode = uDetect;} // set detection mode
	UINT getDetectorType() { return m_uDetectMode; }
    
    void setSampleRate(float f) {m_fSampleRate = f;} // set sample rate
    
    void setLogDetect(bool b) {m_bLogDetector = b;} // set Logarithmic or Linear detection;
    
    float detect(float fInput);
    
    void prepareforplay();
    
protected:
    int  m_nSample;
    float m_fAttackTime;
    float m_fReleaseTime;
    float m_fAttackTime_mSec;
    float m_fReleaseTime_mSec;
    float m_fSampleRate;
    float m_fEnvelope;
    UINT  m_uDetectMode;
    bool  m_bAnalogTC;
    bool  m_bLogDetector;
    
};

#endif /* CEnvelopeDetector_hpp */
