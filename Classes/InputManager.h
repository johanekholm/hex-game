/*
 *  InputManagerCPP.h
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define TOUCH_EVENT_PRESS 1
#define TOUCH_EVENT_MOVE 2
#define TOUCH_EVENT_LIFT 3

#include "toolkit.h"
#include <vector>

class InputState;

struct TouchEvent {
	int type;
	GPoint point;
	TouchEvent() : type(0), point() {}
	TouchEvent(int t, GPoint p) : type(t), point(p) {}
};

class InputManager {
public:
	InputManager();
	~InputManager();
	
	bool wasFlicked();
	bool wasPressed();
	bool wasClicked();
	
	GPoint clickPoint();
	GPoint pressPoint();
		
	GPoint flickedVelocity();

	InputState* currentState();

	void touchesBegan(const GPoint& touchPoint);
	void touchesMoved(const GPoint& touchPoint);
	void touchesEnded(const GPoint& touchPoint);
	void touchesCancelled(const GPoint& touchPoint);

	TouchEvent popEvent();
	bool hasEvent();

private:
	bool _wasFlicked;
	bool _wasClicked;
	bool _wasPressed;
	int _historyCount;
	int _historyHead;
	GPoint _flickedVelocity;
	GPointInTime* _touchHistory;
	GPoint _clickPos;
	GPoint _pressPos;
	std::vector<TouchEvent> _events;
	
	double currentTime();
	double linearMap(double value, double minValue, double maxValue, double minTarget, double maxTarget);
	double linearInterpolate(double from, double to, double percent);
	void addToHistory(const GPoint& point);
	GPointInTime lastTouchPoint();
	GPointInTime firstTouchPoint();
	GPointInTime pointInTimeAtIndex(int index);
	void registerEvent(int aType, const GPoint& aPoint);
};

#endif
