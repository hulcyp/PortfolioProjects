// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectType CkEffectType */
/** @{ */

#pragma once
#include "ck/platform.h"

/** Types of built-in effects available */
typedef enum 
{
    /** Biquadratic filter. */
    kCkEffectType_BiquadFilter,

    /** Bit Crusher effect reduces bit resolution and/or bit rate, producing a retro
      low-fi 8-bit sound .*/
    kCkEffectType_BitCrusher,

    /** Ring Mod effect modulates the audio by a sine wave. */
    kCkEffectType_RingMod,

} CkEffectType;


////////////////////////////////////////


/** Parameters for the Biquad Filter effect */
enum
{
    /** Type of filter; should be one of the filter types listed in the next enum. */
    kCkEffectParam_BiquadFilter_FilterType_Int,

    /** Center frequency of the filter. */
    kCkEffectParam_BiquadFilter_Freq_Float,

    /** Q of the filter. */
    kCkEffectParam_BiquadFilter_Q_Float,

    /** Gain in dB (for Peak, LowShelf, and HighShelf filter types) */
    kCkEffectParam_BiquadFilter_Gain_Float,
};


/** Filter types, used for the kCkEffectParam_BiquadFilter_FilterType_Int param. */
enum
{
    /** Low pass filter blocks high frequencies. */
    kCkBiquadFilter_FilterType_LowPass,

    /** High pass filter blocks low frequencies. */
    kCkBiquadFilter_FilterType_HighPass,
    
    /** Band pass filter blocks frequencies above and below the center frequency. */
    kCkBiquadFilter_FilterType_BandPass,

    /** Notch filter blocks a narrow band of frequencies. */
    kCkBiquadFilter_FilterType_Notch,

    /** Peak filter boosts a narrow band of frequencies. */
    kCkBiquadFilter_FilterType_Peak,

    /** Low shelf filter boosts low frequencies. */
    kCkBiquadFilter_FilterType_LowShelf,

    /** High shelf filter boosts high frequencies. */
    kCkBiquadFilter_FilterType_HighShelf
};


////////////////////////////////////////


/** Parameters for the Bit Crusher effect */
enum
{
    /** Number of bits of resolution to keep in the input samples, in [1..24]. */
    kCkEffectParam_BitCrusher_BitResolution_Int,

    /** Milliseconds to hold each sample value.  Higher values result in more
      reduction in the effective sample rate.  Values around 1 ms provide a reasonable effect. */
    kCkEffectParam_BitCrusher_HoldMs_Float,
};


////////////////////////////////////////


/** Parameters for the Ring Mod effect */
enum
{
    /** Frequency (Hz) of the modulating signal. Values around 500 Hz are typical. */
    kCkEffectParam_RingMod_Freq_Float,
};
