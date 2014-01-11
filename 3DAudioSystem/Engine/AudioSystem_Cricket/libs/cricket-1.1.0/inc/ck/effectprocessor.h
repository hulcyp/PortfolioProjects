// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectProcessor CkEffectProcessor */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/effectparam.h"

/** Performs audio processing for effects.

  To implement your own custom audio effects, create a subclass, then pass an instance
  of it to CkEffect::newCustomEffect(). */

class CkEffectProcessor
{
public:
    virtual ~CkEffectProcessor() {}

    /** Set an effect parameter.
      Note that this should not be called directly; it is called by CkEffect::setParam(). */
    virtual void setParam(int paramId, CkEffectParam value) = 0;

    /** Reset this effect's state. */
    virtual void reset() = 0;

    /** If true, the effect is processed in-place; the input and output will be the
      same buffer.  If false, input and output will be separate buffers. */
    virtual bool isInPlace() const = 0;

    /** Process audio.
      The buffer contains interleaved stereo data; each sample is a signed 8.24 fixed-point value.
      If isInPlace() returns true, inBuf and outBuf will be pointers to the same buffer. 
      It is OK to modify the data in inBuf, even if the effect is not in-place.
      (Remember that because the data is stereo, the number of samples is 2*frames.) */
    virtual void process(int* inBuf, int* outBuf, int frames) = 0;

    /** Get the sample rate of the audio data.  This value may be different on different
      devices, but will not change during app execution. */
    static int getSampleRate();
};


/** @} */
