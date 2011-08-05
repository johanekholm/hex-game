/*
 *  MessageView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MessageView.h"
#include "StringImage.h"
#include "geometry.h"
#include "InputManager.h"
#include "ViewControllerManager.h"
#include "toolkit.h"

#include <math.h>
#include <iostream>


MessageView::~MessageView() {
	delete _stringImage;
}

MessageView::MessageView(GPoint pos, const std::string& string) {
	_pos = pos;
    _posOffset = 0.0f;
    _counter = 0;
    _width = 0.0f;
    _height = 0.0f;

	_stringImage = new StringImage(string);
}

void MessageView::destroy() {
    //ViewControllerManager::instance()->deregister(this);	
    //delete this;
}

void MessageView::draw() {
	return;
}

void MessageView::drawGUI() {
    _stringImage->drawAt(_pos + _posOffset);
}

bool MessageView::handleEvent(const TouchEvent& event) {
	return false;
}

void MessageView::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
}

void MessageView::update() {
    _posOffset.y -= 0.1f;
    
    if (_posOffset.y < -10.0f) {
        ViewControllerManager::instance()->removeSoft(this);
    }    
}

