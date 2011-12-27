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

class InputManager::PrivateData {
public:
	std::vector<TouchEvent> events;
	GPoint previousTouchPoint;
};

InputManager::~InputManager() {
	delete d;
}

InputManager::InputManager() {
	d = new PrivateData();
}

bool InputManager::hasEvent() {
	return (!d->events.empty());
}

TouchEvent InputManager::popEvent() {
	TouchEvent event;
	event = d->events.front();
	d->events.erase(d->events.begin());
	return event;
}

void InputManager::touchesBegan(const GPoint& touchPoint) {
	this->registerEvent(TOUCH_EVENT_PRESS, touchPoint);
	d->previousTouchPoint = touchPoint;
}

void InputManager::touchesMoved(const GPoint& touchPoint) {
	this->registerEvent(TOUCH_EVENT_MOVE, touchPoint);
	d->previousTouchPoint = touchPoint;
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
	if (aType = TOUCH_EVENT_MOVE) {
		event.previousPoint = d->previousTouchPoint;
	}	
	d->events.push_back(event);
}

