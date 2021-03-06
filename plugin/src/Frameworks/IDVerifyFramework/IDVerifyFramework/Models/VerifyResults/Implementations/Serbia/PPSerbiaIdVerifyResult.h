//
//  PPSerbiaIdVerifyResult.h
//  IDVerifyFramework
//
//  Created by Jura Skrlec on 03/07/2017.
//  Copyright © 2017 MicroBlink. All rights reserved.
//

#import "PPIdVerifyResult.h"
#import "PPSerbiaIdViewResult.h"

@interface PPSerbiaIdVerifyResult : PPIdVerifyResult

- (instancetype)init NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithViewResult:(PPIdViewResult *)viewResult NS_UNAVAILABLE;

/**
 * Convenience method to get view result from superclass in appropriate type
 *
 * @return view result in the correct type
 */
- (PPSerbiaIdViewResult *)serbiaViewResult;

@end
