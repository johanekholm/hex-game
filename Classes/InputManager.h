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

#import <Foundation/Foundation.h>

class InputState;

class InputManager {
public:
	InputManager();
	~InputManager();
	
	bool wasFlicked();
	
	GPoint flickedVelocity();

	InputState* currentState();

	void touchesBegan(const GPoint& touchPoint);
	void touchesMoved(const GPoint& touchPoint);
	void touchesEnded(const GPoint& touchPoint);
	void touchesCancelled(const GPoint& touchPoint);
	void update();
	
private:
	bool _wasFlicked;
	int _historyCount;
	int _historyHead;
	GPoint _flickedVelocity;
	GPointInTime* _touchHistory;
	
	double currentTime();
	void addToHistory(const GPoint& point);
	GPointInTime lastTouchPoint();
	GPointInTime pointInTimeAtIndex(int index);
};

#endif