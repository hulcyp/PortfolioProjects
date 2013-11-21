// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectParam CkEffectParam */
/** @{ */

#pragma once
#include "ck/platform.h"

/** A simple variant type used for effect parameters. */
union _CkEffectParam
{
    int intValue;
    float floatValue;
    bool boolValue;
    void* addrValue;

#ifdef __cplusplus
    _CkEffectParam() {}
    _CkEffectParam(float value) { floatValue = value; }
    _CkEffectParam(int value) { intValue = value; }
    _CkEffectParam(bool value) { boolValue = value; }
#endif
};

typedef union _CkEffectParam CkEffectParam;


/** @} */


