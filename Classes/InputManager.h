/*
 *  InputManagerCPP.h
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

class InputState;

class InputManager {
public:
	InputManager();
	~InputManager();
	
	bool isButtonPressed();
	
	InputState* currentState();

	void touchesBegan(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer);
	void touchesMoved(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer);
	void touchesEnded(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer);
	void touchesCancelled(NSSet *touches, UIEvent *event, UIView *view, NSTimer *deltaTimer);
	void update();
	
private:
	InputState *_currentState;
	InputState *_previousState;
	InputState *_queryState;
};
