/*
 *  ControlCallback.h
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef CONTROLCALLBACK_H
#define CONTROLCALLBACK_H

class ControlCallback {
public:
	virtual ~ControlCallback() {}
	virtual void callbackVoid() {};
    virtual void callbackNumber(int num) {};
};

#endif
