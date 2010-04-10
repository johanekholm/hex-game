//
//  InputManager.m
//  OGLGame
//
//  Created by Johan Ekholm on 2010-03-20.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "InputManager.h"


@implementation InputManager

@synthesize currentState;
@synthesize previousState;

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		
		//
		//	Allocate memory for all of the possible states
		//
		currentState = [[InputState alloc] init];
		previousState = [[InputState alloc] init];
		queryState = [[InputState alloc] init];
		
		
		/*
		//
		//	Set the initial coords for the touch locations.
		//
		currentState.touchLocation = CGPointMake(0, 0);
		previousState.touchLocation = CGPointMake(0, 0);
		queryState.touchLocation = CGPointMake(0, 0);
		 */
		
		wasTouched = NO;
	}
	return self;
}

//
//	Deallocation
//
- (void) dealloc
{
	[currentState release];
	[previousState release];
	[queryState release];
	[super dealloc];
}

- (BOOL)isTouched {
	return wasTouched;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer
{
	wasTouched = YES;
	queryState.isBeingTouched = YES;
	queryState.touchLocation = [[touches anyObject] locationInView:view];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer
{
	queryState.isBeingTouched = YES;
	queryState.touchLocation = [[touches anyObject] locationInView:view];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer
{
	queryState.isBeingTouched = NO;
	queryState.touchLocation = [[touches anyObject] locationInView:view];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view WithTimer:(NSTimer *)deltaTimer
{
	queryState.isBeingTouched = NO;
}

- (void) update {
	//	Sets previous state to current state
	previousState.isBeingTouched = currentState.isBeingTouched;
	previousState.touchLocation = currentState.touchLocation;
	
	//	Sets the current state to the query state
	currentState.isBeingTouched = queryState.isBeingTouched;
	currentState.touchLocation = queryState.touchLocation;
}

- (BOOL) isButtonPressed //:(CGRect)rect 
{
	if ( !currentState.isBeingTouched &&
		previousState.isBeingTouched ) {
		
		return 	YES; //[InputManager doesRectangle:rect ContainPoint:currentState.touchLocation];
	}
	
	return NO;
}

@end
