/*
 *  InputManagerCPP.cpp
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 *	Based on FlickDynamics by Dave Peck <http://davepeck.org>
 */

#include "toolkit.h"
#include "InputManager.h"
#include "InputState.h"
#include <math.h>

const int HISTORY_SIZE = 20;
const double FLICK_TIME_BACK = 0.07;

InputManager::~InputManager() {
	free(_touchHistory);
}

InputManager::InputManager() {
	_touchHistory = (GPointInTime*) malloc(sizeof(GPointInTime) * HISTORY_SIZE);
	_historyCount = 0;
	_historyHead = 0;
	_flickedVelocity.x = 0;
	_flickedVelocity.y = 0;
	_wasFlicked = false;
}

bool InputManager::wasFlicked() {
	if (_wasFlicked) {
		_wasFlicked = false;
		return true;
	}
	return false;
}

GPoint InputManager::flickedVelocity() {
	return _flickedVelocity;
}

void InputManager::touchesBegan(const GPoint& touchPoint) {
	_historyCount = 0;
	_historyHead = 0;
	_flickedVelocity.x = 0;
	_flickedVelocity.y = 0;	
	this->addToHistory(touchPoint);	
}

void InputManager::touchesMoved(const GPoint& touchPoint) {
	this->addToHistory(touchPoint);
}

void InputManager::touchesEnded(const GPoint& touchPoint) {
	this->addToHistory(touchPoint);
	
	GPointInTime lastPoint = this->lastTouchPoint();
	
	double startTime = lastPoint.time - FLICK_TIME_BACK;
	int startIndex = 0;
	
	for (int testIndex = 0; testIndex < _historyCount; testIndex++) {
		GPointInTime testPoint = this->pointInTimeAtIndex(testIndex);
		
		if (testPoint.time > startTime) {
			startIndex = testIndex;
			break;
		}
	}
	
	if (startIndex == 0) {
		startIndex++;
	}
	
	GPointInTime startTouchBeforeTime = this->pointInTimeAtIndex(startIndex - 1); 
	GPointInTime startTouchAfterTime = this->pointInTimeAtIndex(startIndex);
	double startTimePercent = FLICK_TIME_BACK / (startTouchAfterTime.time - startTouchBeforeTime.time);
	NSLog(@"Percent: %f (by %f / (%f - %f)", startTimePercent, FLICK_TIME_BACK, startTouchAfterTime.time, startTouchBeforeTime.time);

	NSLog(@"beforeX: %f",startTouchBeforeTime.x);
	NSLog(@"beforeY: %f",startTouchBeforeTime.y);
	NSLog(@"afterX: %f",startTouchAfterTime.x);
	NSLog(@"afterY: %f",startTouchAfterTime.y);

	double flickX = startTouchAfterTime.x * (1.0 - startTimePercent) + startTouchBeforeTime.x * startTimePercent;
	double flickY = startTouchAfterTime.y * (1.0 - startTimePercent) + startTouchBeforeTime.y * startTimePercent;
	
	if (abs(flickX) < 50) {
		flickX = lastPoint.x;
	}
	if (abs(flickY) < 50) {
		flickY = lastPoint.y;
	}
	if (abs(flickY) < 50 && abs(flickX) < 50) {
		_wasFlicked = false;
		return;
	}
	_flickedVelocity.x = (lastPoint.x - flickX) * 0.05;
	_flickedVelocity.y = (lastPoint.y - flickY) * 0.05;
	NSLog(@"X: %f",_flickedVelocity.x);
	NSLog(@"Y: %f",_flickedVelocity.y);

	_wasFlicked = true;
}

void InputManager::touchesCancelled(const GPoint& touchPoint) {
	NSLog(@"Cancelled, crashing!");
//	_queryState->setTouched(false);
}

void InputManager::update() {
	//	Sets previous state to current state
//	_previousState->setTouched( _currentState->touched() );
//	_previousState->setTouchLocation( _currentState->touchLocation() );

	//	Sets the current state to the query state
//	_currentState->setTouched( _queryState->touched() );
//	_currentState->setTouchLocation( _queryState->touchLocation() );
}


/* 
 * Private functions
 */

double InputManager::currentTime() {
	return [[NSDate date] timeIntervalSince1970];
}

GPointInTime InputManager::pointInTimeAtIndex(int index) {
	int rawIndex = _historyHead + index;
	
	if (rawIndex >= HISTORY_SIZE)
	{
		rawIndex -= HISTORY_SIZE;
	}
	
	return _touchHistory[rawIndex];
}

GPointInTime InputManager::lastTouchPoint() {
	return this->pointInTimeAtIndex(_historyCount - 1);
}

void InputManager::addToHistory(const GPoint& point) {
	int rawIndex;

	if (_historyCount < HISTORY_SIZE) {
		rawIndex = _historyCount;
		_historyCount++;
	} else {
		rawIndex = _historyHead;
		_historyHead++;
		
		if (_historyHead == HISTORY_SIZE) {
			_historyHead = 0;
		}
	}
	
	_touchHistory[rawIndex].x = point.x;
	_touchHistory[rawIndex].y = point.y;
	_touchHistory[rawIndex].time = this->currentTime();	
}