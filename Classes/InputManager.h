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

const int HISTORY_SIZE = 20;

class InputState;

struct TouchEvent {
	int type;
	GPoint point;
    GPoint translatedPoint;
	GPoint previousPoint;
	TouchEvent() : type(0), point() {}
	TouchEvent(int t, GPoint p, GPoint pp) : type(t), point(p), previousPoint(pp) {}
};

class InputManager {
public:
	InputManager();
	~InputManager();

	void touchesBegan(const GPoint& touchPoint);
	void touchesMoved(const GPoint& touchPoint);
	void touchesEnded(const GPoint& touchPoint);
	void touchesCancelled(const GPoint& touchPoint);

	TouchEvent popEvent();
	bool hasEvent();

private:
	class PrivateData;
	PrivateData* d;
	void registerEvent(int aType, const GPoint& aPoint);
};

#endif
