/*
 *  InputManagerCPP.cpp
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"
#include "InputManager.h"
#include <math.h>

InputManager::~InputManager() {
}

InputManager::InputManager() {
}

void InputManager::touchesBegan(const GPoint& touchPoint) {
  d->startPoint = touchPoint;
  this->registerEvent(TOUCH_EVENT_PRESS, touchPoint);
}

void InputManager::touchesMoved(const GPoint& touchPoint) {
	this->registerEvent(TOUCH_EVENT_MOVE, touchPoint);
}

void InputManager::touchesEnded(const GPoint& touchPoint) {
	this->registerEvent(TOUCH_EVENT_LIFT, touchPoint);
}

void InputManager::touchesCancelled(const GPoint& touchPoint) {
	// TODO: Implement me
}

/* 
 * Private functions
 */

void InputManager::registerEvent(int aType, const GPoint& aPoint) {
	TouchEvent event(aType, aPoint);
	_events.push_back(event);
}

TouchEvent InputManager::popEvent() {
	TouchEvent event;
	event = _events.back();
	_events.pop_back();
	return event;
}

bool InputManager::hasEvent() {
	return (!_events.empty());
}
