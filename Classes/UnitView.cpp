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
#include <math.h>

UnitView::~UnitView() {
	delete _unitImage;
	delete _actionImage;
	delete _directionImage;
	_unitModel = 0;
}

UnitView::UnitView(UnitModel* model, GLfloat aWidth, GLfloat aHeight, int index) {
	_unitModel = model;
	_facing = 0.0f;
	_actionImage = new GameImage(aWidth, aHeight, TextureCatalog::instance()->get("units"), index);
	_actionImage = new GameImage(32.0f, 32.0f, TextureCatalog::instance()->get("actions"), 0);
	_directionImage = new GameImage(16.0f, 16.0f, TextureCatalog::instance()->get("icons"), 0);
	_actions.push_back(0);
	_actions.push_back(1);
	_actions.push_back(2);
	_actions.push_back(3);
	
}

void UnitView::drawActions() {
	float i = 0;
	GPoint actionPos;
	
	for (std::vector<int>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
		actionPos = _pos + this->getActionPosition(i);
		i++;
		_actionImage->drawAtWithSubTexture(actionPos, *it);
	}

}


GPoint UnitView::getActionPosition(int index) {
	return GPointMake(cos(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index), 
					  sin(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index)) * ACTION_RADIUS;
}

void UnitView::updateActions(std::vector<int> actions) {
	_actions = actions;
}

void UnitView::updatePosition(const MPoint& pos, int direction) {
	_pos.x = 64.0f + (GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f;
	_pos.y = 64.0f + (GLfloat)pos.y * 50.0f;
	_facing = (GLfloat)direction;
}


void UnitView::draw() {
	_unitImage->drawAt(_pos);
}

void UnitView::drawGUI() {
	_directionImage->drawAtRotatedWithSubTexture(_pos, (GLfloat)_facing * 60.0f + 180.0f, 0);
	this->drawActions();
}


bool UnitView::handleEvent(const TouchEvent& event) {
	if (event.type == 3) {
		_unitModel->doAction(this->touchedAction(event.point));	
	}
}

int UnitView::touchedAction(GPoint point) {
	GPoint actionPos;
	int i = 0;
	
	for (std::vector<int>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
		actionPos = _pos + this->getActionPosition(i);
		
		if (PointWithin(point, actionPos, 32.0f)) {
			return *it;
		}
		i++;
	}
	return (-1);
}

void UnitView::update() {
	this->updatePosition(_unitModel->getPosition(), _unitModel->getDirection());
	this->updateActions(_unitModel->getActions());	
}



/*bool UnitView::wasTouched(GPoint point) {
	if (point.x >= _pos.x - width/2 && point.x <= _pos.x + width/2 && point.y >= _pos.y - height/2 && point.y <= _pos.y + height/2) {
		return true;
	} else {
		return false;
	}
}*/

