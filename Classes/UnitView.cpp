/*
 *  UnitView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitView.h"
#include "geometry.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "ViewControllerManager.h"
#include "toolkit.h"
#include "Action.h"

#include <math.h>
#include <iostream>


UnitView::~UnitView() {
	delete _unitImage;
	delete _actionImage;
	delete _directionImage;
    _actionPoints.clear();
	_unitModel = 0;
}

UnitView::UnitView(UnitModel* model, GLfloat width, GLfloat height, int index) {
	_unitModel = model;
    _width = width;
    _height = height;
	_unitImage = new GameImage(width, height, TextureCatalog::instance()->get("units"), index);
	_actionImage = new GameImage(32.0f, 32.0f, TextureCatalog::instance()->get("actions"), 0);
	_directionImage = new GameImage(16.0f, 16.0f, TextureCatalog::instance()->get("icons"), 0);	
}

void UnitView::drawActions() {
	//float i = 0;
	//GPoint actionPos;
	
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
		//actionPos = transformModelPositionToView((*it).pos); //this->getActionPosition(i);
		//i++;
        if (!(*it).active) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
		_actionImage->drawAtWithSubTexture((*it).pos, (*it).actionId);
	}
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}


GPoint UnitView::getActionPosition(int index) {
	return GPointMake(cos(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index), 
					  sin(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index)) * ACTION_RADIUS;
}

void UnitView::updateActions() {
	ActionView actionView;
    
    _actionPoints.clear();
    
    for (std::vector<ActionState>::iterator it = _state.actions.begin(); it != _state.actions.end(); ++it) {
        actionView.pos = transformModelPositionToView((*it).pos);
        actionView.actionId = (*it).actionId;
        actionView.active = (*it).active;
        actionView.statePoint = &(*it);
        _actionPoints.push_back(actionView);
    }	
}

void UnitView::updatePosition(const MPoint& pos) {
	_pos.x = 64.0f + (GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f;
	_pos.y = 64.0f + (GLfloat)pos.y * 50.0f;
}


void UnitView::draw() {
	_unitImage->drawAt(_pos);
}

void UnitView::drawGUI() {
    this->drawHpBar();
    this->drawApBar();
	if (this->_hasFocus) {
        this->drawActions();
    }
}

void UnitView::drawHpBar() {
    GLfloat width, height, yOffset, ratio, length;
    
    ratio = (GLfloat)_state.hp / (GLfloat)_state.maxHp;
    width = 32.0f;
    length = ratio * width;
    height = 3.0f;
    yOffset = 32.0f;
    
    GLfloat	vertices[] = {	0.0f, -height/2.0f, 0.0f,
		length, -height/2.0f, 0.0f,
		0.0f, height/2.0f, 0.0f,
		0.0f, height/2.0f, 0.0f,
		length, -height/2.0f, 0.0f,		
		length, height/2.0f, 0.0f };
	
    glDisable(GL_TEXTURE_2D);
    
    if (ratio >= 0.75f) {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);        
    } else if (ratio >= 0.25f) {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);                
    } else {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);                        
    }
	glLoadIdentity();
	glTranslatef(_pos.x - width/2.0f, _pos.y + yOffset, 0.0f);
		
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
}

void UnitView::drawApBar() {
    GLfloat width, height, yOffset, ratio, length;
    
    ratio = (GLfloat)_state.ap / (GLfloat)_state.maxAp;
    width = 32.0f;
    length = ratio * width;
    height = 6.0f;
    yOffset = 26.0f;
    
    GLfloat	vertices[] = {	0.0f, -height/2.0f, 0.0f,
		length, -height/2.0f, 0.0f,
		0.0f, height/2.0f, 0.0f,
		0.0f, height/2.0f, 0.0f,
		length, -height/2.0f, 0.0f,		
		length, height/2.0f, 0.0f };
	
    glDisable(GL_TEXTURE_2D);
    
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);                        
	glLoadIdentity();
	glTranslatef(_pos.x - width/2.0f, _pos.y + yOffset, 0.0f);
    
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
}



bool UnitView::handleEvent(const TouchEvent& event) {
    ActionState* statePoint;
    
	if (event.type == 3) {
        statePoint = this->touchedAction(event.point);
        
        if (statePoint != 0) {
            _unitModel->doAction(*statePoint);
        }
			
	}
	return true;
}

ActionState* UnitView::touchedAction(GPoint point) {
	//GPoint actionPos;
	//int i = 0;
	
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
		//actionPos = _pos + this->getActionPosition(i);
		
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return (*it).statePoint;
		}
		//i++;
	}
	return (0);
}

void UnitView::update() {
    _state = _unitModel->getState();
	this->updatePosition(_state.pos);
	this->updateActions();	
}

void UnitView::destroyed() {
    ViewControllerManager::instance()->remove(this);	
}

void UnitView::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
    this->update();
}



/*bool UnitView::wasTouched(GPoint point) {
	if (point.x >= _pos.x - width/2 && point.x <= _pos.x + width/2 && point.y >= _pos.y - height/2 && point.y <= _pos.y + height/2) {
		return true;
	} else {
		return false;
	}
}*/

