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
class ControlCallback;

/*---------------------------------------------------------------*/

class TransitionViewController : public ViewController {
protected:
    ShapeImage* _background;
    int _counter;
    GLfloat _alpha;
    ControlCallback& _returnControl;
    
public:
	~TransitionViewController();
	TransitionViewController(ControlCallback& control);
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
    void update();
};


#endif
