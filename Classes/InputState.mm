//
//  InputState.m
//  OGLGame
//
//  Created by Johan Ekholm on 2010-04-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "InputState.h"

InputState::~InputState() {
}

InputState::InputState() {
	_touched = false;
	_touchLocation = CGPointMake(0, 0);
}

bool InputState::touched() {
	return _touched;
}

CGPoint InputState::touchLocation() {
	return _touchLocation;
}

void InputState::setTouchLocation(CGPoint touchLocation) {
	_touchLocation = touchLocation;
}

void InputState::setTouched( bool touched) {
	_touched = touched;
}