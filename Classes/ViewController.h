/*
 *  ViewController.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "toolkit.h"

namespace MapLayer {
    const int BUILDING = 1;
    const int UNIT = 2;
    const int PLAYER = 3;
    const int GUI = 4;
    const int ABOVE_GUI = 5;
};

//struct GPoint;
struct TouchEvent;

class ViewController {
protected:	
    GPoint _pos;
	GLfloat _width, _height;
	bool _hasFocus;
    int _layer;
    
	//virtual void reactToEvent(int eventType) = 0;
	
public:
    ViewController();
    ViewController(const GPoint& pos, GLfloat width, GLfloat height, int layer);
	bool isWithin(const GPoint& point);
    int getLayer();
	int getHeight();
    GPoint getPosition();
	int getWidth();
	virtual bool catchFocus();
    virtual void setFocus(bool hasFocus);
	virtual bool handleEvent(const TouchEvent& event) = 0;
	virtual void draw(const GPoint& cameraPos) = 0;
	virtual void drawGUI(const GPoint& cameraPos) = 0;
	void setPosition(const GPoint& pos);
    virtual void update();
};

#endif