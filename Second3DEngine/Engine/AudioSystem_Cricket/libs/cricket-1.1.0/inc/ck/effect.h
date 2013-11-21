// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffect CkEffect */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/effectparam.h"
#include "ck/effecttype.h"

class CkEffectProcessor;


/** An audio effect. */
class CkEffect
{
public:
    /** Set an effect parameter. */
    void setParam(int paramId, CkEffectParam value);


    /** Reset the state of this effect. */
    void reset();


    /** Set whether this effect is bypassed.
      A bypassed effect is not applied to the audio. */
    void setBypassed(bool);

    /** Gets whether this effect is bypassed.
      A bypassed effect is not applied to the audio. */
    bool isBypassed() const;


    /** Sets the wet/dry ratio for this effect
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the ffect.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    void setWetDryRatio(float);

    /** Gets the wet/dry ratio for this effect.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the effect.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    float getWetDryRatio() const;


    /** Create a built-in effect. */
    static CkEffect* newEffect(CkEffectType);

    /** Create a custom effect.
      Note that the CkEffectProcessor object will be deleted when the CkEffect object is destroyed. */
    static CkEffect* newCustomEffect(CkEffectProcessor*);

    /** Destroy the effect. */
    void destroy();

private:
    CkEffect();
    ~CkEffect();
    CkEffect(const CkEffect&);
    CkEffect& operator=(const CkEffect&);
};


/** @} */
