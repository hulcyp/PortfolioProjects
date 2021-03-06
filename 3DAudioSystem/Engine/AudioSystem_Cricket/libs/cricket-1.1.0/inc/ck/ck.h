// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup Ck Ck */
/** @{ */

#pragma once
#include "ck/config.h"
#include "ck/audiocallback.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** Initializes Cricket Audio. Returns nonzero if successful. */
int CkInit(CkConfig*);

/** Updates Cricket Audio.
  Should be called once per frame. */
void CkUpdate();

/** Shuts down Cricket Audio. */
void CkShutdown();

/** Suspends Cricket Audio; typically used when an app is made inactive. */
void CkSuspend();

/** Resumes Cricket Audio; typically used when an app is made active after having been inactive. */
void CkResume();


/** Returns an estimate of the load on the audio processing thread.
  This is the fraction of the interval between audio callbacks that was actually spent
  rendering audio; it will range between 0 and 1. */
float CkGetCpuLoad();


/** Sets a callback to be called before each slice of audio is handed off to the operating system for output.
  This will be called on a thread that is not the main thread.  Be sure not to spend too much
  time in the callback. 
 @deprecated Instead, use a custom effect on the global effect bus. */
void CkSetOutputCallback(CkAudioCallback, void* data);

/** Gets a callback to be called before each slice of audio is handed off to the operating system for output. 
 @deprecated Instead, use a custom effect on the global effect bus. */
void CkGetOutputCallback(CkAudioCallback*, void** data);


/** Sets the maximum rate at which volumes change on playing sounds, in ms per full volume scale. 
  Default value is 40 ms. */
void CkSetVolumeRampTime(float ms);

/** Gets the maximum rate at which volumes change on playing sounds, in ms per full volume scale. */
float CkGetVolumeRampTime();

/** Default value of volume ramp time. */
extern const float Ck_volumeRampTimeMsDefault;

#ifdef __cplusplus
} // extern "C"
#endif

/** @} */
