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

void MessageView::add(const GPoint& pos, const std::string& string) {
    if (string.size() > 0) {
        ViewControllerManager::instance()->add(new MessageView(pos, string));        
    }
}

void MessageView::add(const MPoint& mPos, const std::string& string) {
    GPoint gPos;
    
    gPos.x = 64.0f + (GLfloat)mPos.x * 64.0f + (mPos.y % 2) * 32.0f;
    gPos.y = 64.0f + (GLfloat)mPos.y * 50.0f;

    if (string.size() > 0) {
        ViewControllerManager::instance()->add(new MessageView(gPos, string));
    }
}

void MessageView::add(const MPoint& mPos, const std::string& string, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    GPoint gPos;
    
    gPos.x = 64.0f + (GLfloat)mPos.x * 64.0f + (mPos.y % 2) * 32.0f;
    gPos.y = 64.0f + (GLfloat)mPos.y * 50.0f;
    
    if (string.size() > 0) {
        ViewControllerManager::instance()->add(new MessageView(gPos, string, red, green, blue, alpha));
    }
}


MessageView::~MessageView() {
	delete _stringImage;
}

MessageView::MessageView(const GPoint& pos, const std::string& string) {
	_pos = pos;
    _posOffset = GPointMake(0.0f, -10.0f);
    _counter = 0;
    _width = 0.0f;
    _height = 0.0f;

	_stringImage = new StringImage(string, 1.0f, 1.0f, 1.0f, 1.0f);
}

MessageView::MessageView(const GPoint& pos, const std::string& string, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	_pos = pos;
    _posOffset = GPointMake(0.0f, -10.0f);
    _counter = 0;
    _width = 0.0f;
    _height = 0.0f;
    
	_stringImage = new StringImage(string, red, green, blue, alpha);
}

void MessageView::destroy() {
    //ViewControllerManager::instance()->deregister(this);	
    //delete this;
}

void MessageView::draw(const GPoint& cameraPos) {
	return;
}

void MessageView::drawGUI(const GPoint& cameraPos) {
    _stringImage->drawCenteredAt(_pos + _posOffset);
}

bool MessageView::handleEvent(const TouchEvent& event) {
	return false;
}

void MessageView::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
}

void MessageView::update() {
    _posOffset.y -= 0.2f;
    
    if (_posOffset.y < -20.0f) {
        ViewControllerManager::instance()->removeSoft(this);
    }    
}

