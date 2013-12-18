// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectBus CkEffectBus */
/** @{ */

#pragma once
#include "ck/platform.h"


class CkEffect;

/** Manages a set of audio effects applied to sounds. */
class CkEffectBus
{
public:

    /** Add an effect to this bus.
      An effect can only be on one bus at a time. */
    void addEffect(CkEffect*);

    /** Remove an effect from this bus. */
    void removeEffect(CkEffect*);

    /** Remove all effects from this bus. */
    void removeAllEffects();


    /** Set the bus to which this bus's output is sent.
      If NULL (the default), the audio from this bus is mixed into the final output. */
    void setOutputBus(CkEffectBus*);

    /** Get the bus to which this bus's output is sent.
      If NULL (the default), the audio from this bus is mixed into the final output. */
    CkEffectBus* getOutputBus();


    /** Reset the state of all the effects on this bus. */
    void reset();


    /** Set whether this bus is bypassed.
      A bypassed effect is not applied to the audio. */
    void setBypassed(bool);

    /** Gets whether this bus is bypassed.
      Effects in a bypassed bus are not applied to the audio. */
    bool isBypassed() const;


    /** Sets the wet/dry ratio for this bus.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the bus.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    void setWetDryRatio(float);

    /** Gets the wet/dry ratio for this bus.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the bus.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    float getWetDryRatio() const;

    ////////////////////////////////////////

    /** Create a new effect bus. */
    static CkEffectBus* newEffectBus();

    /** Get the global effect bus.
      Effects on this bus are applied to the final audio before it is output. */
    static CkEffectBus* getGlobalEffectBus();

    /** Destroy this effect bus. */
    void destroy();

private:
    CkEffectBus();
    ~CkEffectBus();
    CkEffectBus(const CkEffectBus&);
    CkEffectBus& operator=(const CkEffectBus&);

};

/** @} */

