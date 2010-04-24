/*
 *  InputManagerCPP.cpp
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit_iphone.h"
#include "InputManager.h"
#include "InputState.h"

InputManager::~InputManager() {
	delete _currentState;
	delete _previousState;
	delete _queryState;
}

InputManager::InputManager() {
	_currentState = new InputState();
	_previousState = new InputState();
	_queryState = new InputState();
}

bool InputManager::isButtonPressed() {
	if ( !_currentState->touched() && _previousState->touched() ) {
		return true;
	}
	return false;
}

InputState * InputManager::currentState() {
	return _currentState;
}

void InputManager::touchesBegan(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer) {
	_queryState->setTouched(true);
	GPoint point = GPointFromCGPoint([[touches anyObject] locationInView:view]);
	_queryState->setTouchLocation(point);
}

void InputManager::touchesMoved(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer) {
	GPoint point = GPointFromCGPoint([[touches anyObject] locationInView:view]);
	_queryState->setTouchLocation(point);
}

void InputManager::touchesEnded(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer) {
	_queryState->setTouched(false);
	GPoint point = GPointFromCGPoint([[touches anyObject] locationInView:view]);
	_queryState->setTouchLocation(point);
}

void InputManager::touchesCancelled(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer) {
	_queryState->setTouched(false);
}

void InputManager::update() {
	//	Sets previous state to current state
	_previousState->setTouched( _currentState->touched() );
	_previousState->setTouchLocation( _currentState->touchLocation() );

	//	Sets the current state to the query state
	_currentState->setTouched( _queryState->touched() );
	_currentState->setTouchLocation( _queryState->touchLocation() );
}