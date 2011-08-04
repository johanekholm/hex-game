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

class StringImage;

class MessageView : public ViewController {
    GPoint _posOffset;
    int _counter;
	StringImage* _stringImage;

    void destroy();

public:
	~MessageView();
	MessageView(GPoint pos, const std::string& string);
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
    void setFocus(bool hasFocus);
};

#endif