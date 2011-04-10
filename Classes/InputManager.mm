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
#include <math.h>

const double FLICK_TIME_BACK = 0.07;
const double FLICK_DAMPENING = 0.05;
const int HISTORY_SIZE = 20;
const int FLICK_TRESHOLD = 30;
const int FLICK_MAXSPEED = 10;


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
	_wasClicked = false;
}

bool InputManager::wasFlicked() {
	if (_wasFlicked) {
		_wasFlicked = false;
		return true;
	}
	return false;
}

bool InputManager::wasPressed() {
	if (_wasPressed) {
		_wasPressed = false;
		return true;
	}
	return false;
}

bool InputManager::wasClicked() {
	if (_wasClicked) {
		_wasClicked = false;
		return true;
	}
	return false;
}

GPoint InputManager::clickPoint() {
	return _clickPos;
}

GPoint InputManager::pressPoint() {
	return _pressPos;
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
	
	_wasPressed = true;
	_pressPos = touchPoint;
}

void InputManager::touchesMoved(const GPoint& touchPoint) {
	this->addToHistory(touchPoint);
}

void InputManager::touchesEnded(const GPoint& touchPoint) {
	this->addToHistory(touchPoint);
	
	//GPointInTime lastPoint = this->lastTouchPoint();
	GPointInTime firstPoint = this->firstTouchPoint();
	
	if (fmax(fabs(touchPoint.x - firstPoint.x), fabs(touchPoint.y - firstPoint.y)) < 10.0f) {
		_wasClicked = true;
		_clickPos = touchPoint;
	}
	
	
/*	double startTime = lastPoint.time - FLICK_TIME_BACK;
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
	double startTimePercent = linearMap(startTime, startTouchBeforeTime.time, startTouchAfterTime.time, 0.0, 1.0);
	double flickX = linearInterpolate(startTouchBeforeTime.x, startTouchAfterTime.x, startTimePercent);
	double flickY = linearInterpolate(startTouchBeforeTime.y, startTouchAfterTime.y, startTimePercent);

	if (fabs(flickX-lastPoint.x) < FLICK_TRESHOLD) {
		flickX = lastPoint.x;
	}
	if (fabs(flickY-lastPoint.y) < FLICK_TRESHOLD) {
		flickY = lastPoint.y;
	}
	
	if ((flickX == lastPoint.x) && (flickY == lastPoint.y)) {
		_wasFlicked = false;
		return;
	}
	
	double speedX = (lastPoint.x - flickX) * FLICK_DAMPENING;
	double speedY = (lastPoint.y - flickY) * FLICK_DAMPENING;
	
	double absX = fabs(speedX);
	double absY = fabs(speedY);
	
	if (absX > FLICK_MAXSPEED && absX >= absY) {
		double speedModifier = FLICK_MAXSPEED / absX;
		speedX *= speedModifier;
		speedY *= speedModifier;
	} else if (absY > FLICK_MAXSPEED) {
		double speedModifier = FLICK_MAXSPEED / absY;
		speedX *= speedModifier;
		speedY *= speedModifier;
	}
	
	_flickedVelocity.x = speedX;
	_flickedVelocity.y = speedY;
	
	_wasFlicked = true;*/
}

void InputManager::touchesCancelled(const GPoint& touchPoint) {
	// IMPLEMENT ME
}

/* 
 * Private functions
 */

double InputManager::currentTime() {
	return [[NSDate date] timeIntervalSince1970];
}

double InputManager::linearMap(double value, double minValue, double maxValue, double minTarget, double maxTarget) {
    double zeroValue = value - minValue;
    double valueRange = maxValue - minValue;
    double targetRange = maxTarget - minTarget;
    double zeroTargetValue = zeroValue * (targetRange / valueRange);
    double targetValue = zeroTargetValue + minTarget;
	return targetValue;
}

double InputManager::linearInterpolate(double from, double to, double percent) {
	return (from * (1.0f - percent)) + (to * percent);
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

GPointInTime InputManager::firstTouchPoint() {
	return this->pointInTimeAtIndex(0);
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