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

/*---------------------------------------------------------------*/

class TransitionViewController : public ViewController {
protected:
    ShapeImage* _background;
    int _counter;
    GLfloat _alpha;

public:
	~TransitionViewController();
	TransitionViewController();
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
    void update();
};


#endif
