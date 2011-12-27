/*
 *  TransitionViewController.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef TRANSITIONVIEWCONTROLLER_H
#define TRANSITIONVIEWCONTROLLER_H

#include "ViewController.h"
#include <vector>
#include <string>

class ShapeImage;
class StringImage;
class ControlCallback;

/*---------------------------------------------------------------*/

class TransitionViewController : public ViewController {
protected:
    int _counter;
    ControlCallback& _returnControl;
    
public:
	virtual ~TransitionViewController();
	TransitionViewController(ControlCallback& control);
	virtual void draw(const GPoint& cameraPos) = 0;
	virtual void drawGUI(const GPoint& cameraPos) = 0;
	bool handleEvent(const TouchEvent& event);
    virtual void update() = 0;
};

class FadeOutTransition : public TransitionViewController {
protected:
    ShapeImage* _background;
    GLfloat _alpha;
    
public:
	virtual ~FadeOutTransition();
	FadeOutTransition(ControlCallback& control);
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
    void update();
};

/*---------------------------------------------------------------*/

class VictoryTransition : public TransitionViewController {
protected:
    ShapeImage* _background;
    StringImage* _message;
    GLfloat _alpha;
    
public:
	virtual ~VictoryTransition();
	VictoryTransition(ControlCallback& control);
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
    void update();
    
};

#endif
