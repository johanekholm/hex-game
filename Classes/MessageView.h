/*
 *  UnitView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include "ViewController.h"
#include <string>
#include <sstream>

class StringImage;

class MessageView : public ViewController {
    GPoint _posOffset;
    int _counter;
	StringImage* _stringImage;

    void destroy();

public:
	~MessageView();
	MessageView(const GPoint& pos, const std::string& string);
    MessageView(const GPoint& pos, const std::string& string, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
    void setFocus(bool hasFocus);
    void update();
    
    static void add(const GPoint& pos, const std::string& string);
    static void add(const MPoint& mPos, const std::string& string);
    static void add(const MPoint& mPos, const std::string& string, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
};

#endif