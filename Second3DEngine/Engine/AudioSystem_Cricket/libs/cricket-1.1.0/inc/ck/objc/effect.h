// Copyright 2013 Cricket Technology
// www.crickettechnology.com

#import <Foundation/Foundation.h>
#import "ck/effecttype.h"
#import "ck/effectparam.h"

struct CkEffectImpl;

/** Objective-C interface for CkEffect.
  See ck/effect.h for method documentation. */
@interface CkoEffect : NSObject
{
    struct CkEffectImpl* m_impl;
}

@property(nonatomic, assign) BOOL bypassed;
@property(nonatomic, assign) float wetDryRatio;

- (void) setParam:(int)paramId value:(CkEffectParam)value;

- (void) reset;

- (void) setBypassed:(BOOL)bypass;
- (BOOL) bypassed;

- (void) setWetDryRatio:(float)wetDry;
- (float) wetDryRatio;

+ (CkoEffect*) newEffect:(CkEffectType)type;

@end




