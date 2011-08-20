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
#include "ShapeImage.h"

#include <math.h>
#include <iostream>


UnitView::~UnitView() {
	delete _unitImage;
	delete _actionImage;
	delete _directionImage;
    delete _hpBar;
    delete _apBar;
    delete _hpBarSlot;
    delete _apBarSlot;
    delete _actionMarker;
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
    _hpBar = new RectangleImage(RGBAMake(0.0f, 1.0f, 0.0f, 1.0f), 32.0f, 4.0f, true);
    _apBar = new RectangleImage(RGBAMake(0.0f, 0.0f, 1.0f, 1.0f), 32.0f, 6.0f, true);
    _hpBarSlot = new RectangleImage(RGBAMake(0.5f, 0.5f, 0.5f, 1.0f), 32.0f, 4.0f, true);
    _apBarSlot = new RectangleImage(RGBAMake(0.5f, 0.5f, 0.5f, 1.0f), 32.0f, 6.0f, true);
    _actionMarker = new EllipseImage(RGBAMake(0.0f, 1.0f, 0.0f, 1.0f), 32.0f, 32.0f, 16, false);
    _selectedActionView = 0;
}

void UnitView::destroyed() {
    ViewControllerManager::instance()->remove(this);	
}

void UnitView::draw() {
	_unitImage->drawAt(_pos);
}

void UnitView::drawActions() {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
        if (!(*it).active) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
		_actionImage->drawAtWithSubTexture((*it).pos, (*it).actionId);
        
        if (_selectedActionView != 0) {
            _actionMarker->drawCenteredAt((*_selectedActionView).pos);
        }        
	}
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void UnitView::drawGUI() {
    _hpBarSlot->drawAt(GPointMake(_pos.x - 16.0f, _pos.y + 32.0f));
    _apBarSlot->drawAt(GPointMake(_pos.x- 16.0f, _pos.y + 26.0f));
    
    _hpBar->drawAt(GPointMake(_pos.x - 16.0f, _pos.y + 32.0f));
    _apBar->drawAt(GPointMake(_pos.x- 16.0f, _pos.y + 26.0f));

	if (this->_hasFocus) {
        this->drawActions();
    }
}

GPoint UnitView::getActionPosition(int index) {
	return GPointMake(cos(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index), 
					  sin(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index)) * ACTION_RADIUS;
}

ActionState* UnitView::getTouchedActionState(GPoint point) {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return (*it).statePoint;
		}
	}
	return (0);
}

ActionView* UnitView::getTouchedActionView(GPoint point) {
	
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {		
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return &(*it);
		}
	}
	return (0);
}

bool UnitView::handleEvent(const TouchEvent& event) {
    ActionState* statePoint;
    
	if (event.type == 3) {
        statePoint = this->getTouchedActionState(event.point);
        
        if (statePoint != 0) {
            _unitModel->doAction(*statePoint);
        }
	}
    
    if (event.type == 2) {
        _selectedActionView = this->getTouchedActionView(event.point);
	}
	return true;
}

void UnitView::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
    this->update();
}

void UnitView::update() {
    _state = _unitModel->getState();
	this->updatePosition(_state.pos);
	this->updateActions();
	this->updateBars();
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

void UnitView::updateBars() {
    GLfloat ratioAp, ratioHp, lengthAp, lengthHp;
    RGBA color;
    
    ratioHp = (GLfloat)_state.hp / (GLfloat)_state.maxHp;
    lengthHp = ratioHp * 32.0f;

    ratioAp = (GLfloat)_state.ap / (GLfloat)_state.maxAp;
    lengthAp = ratioAp * 32.0f;

    if (ratioHp > 0.75f) {
        color.makeGreen(); 
    } else if (ratioHp > 0.25f) {
        color.makeYellow(); 
    } else {
        color.makeRed(); 
    }
    
    _hpBar->setSize(lengthHp, 4.0f);
    _hpBar->setColor(color);
    
    _apBar->setSize(lengthAp, 6.0f);
}

void UnitView::updatePosition(const MPoint& pos) {
	_pos.x = 64.0f + (GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f;
	_pos.y = 64.0f + (GLfloat)pos.y * 50.0f;
}

