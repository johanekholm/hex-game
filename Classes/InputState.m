//
//  InputState.m
//  OGLGame
//
//  Created by Johan Ekholm on 2010-04-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "InputState.h"

@implementation InputState

@synthesize isBeingTouched;
@synthesize touchLocation;

- (id) init
{
	self = [super init];
	if (self != nil) {
		isBeingTouched = NO;
		touchLocation = CGPointMake(0, 0);
	}
	return self;
}

@end