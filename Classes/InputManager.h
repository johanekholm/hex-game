//
//  InputManager.h
//  OGLGame
//
//  Created by Johan Ekholm on 2010-03-20.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import	"InputState.h"

@interface InputManager : NSObject {
	BOOL wasTouched;
	InputState *currentState;
	InputState *previousState;
	InputState *queryState;
	
	
}

@property (nonatomic, readonly) InputState *currentState;
@property (nonatomic, readonly) InputState *previousState;


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view WithTimer:(NSTimer *)deltaTimer;
- (BOOL)isTouched;
- (BOOL)isButtonPressed;
- (void) update;

@end



/*
 #import <Foundation/Foundation.h>

#import "InputState.h"

@interface InputManager : NSObject
{
@private
	bool isLandscape;
	InputState *currentState;
	InputState *previousState;
	InputState *queryState;
}

@property (nonatomic, readwrite) bool isLandscape;
@property (nonatomic, readonly) InputState *currentState;
@property (nonatomic, readonly) InputState *previousState;

- (void) update:(float)deltaTime;

//
//	Touch events
//

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view  WithTimer:(NSTimer *)deltaTimer;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event InView:(UIView *)view WithTimer:(NSTimer *)deltaTimer;

//
//	Helper Methods
//
- (void) update:(float)deltaTime;
- (bool) isButtonPressed:(CGRect)rect;
- (bool) isButtonHeld:(CGRect)rect;
- (void) convertCoordinatesToLandscape;

//
//	Class methods
//
+ (bool) doesRectangle:(CGRect)rect ContainPoint:(CGPoint)point;

@end
*/