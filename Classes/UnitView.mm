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

UnitView::~UnitView() {
	delete _actionImage;
}

UnitView::UnitView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index) : GameImage(aWidth, aHeight, tex, index){
	_actionImage = new GameImage(32.0f, 32.0f, TextureCatalog::instance()->get("actions"), 0);
	_actions.push_back(0);
	_actions.push_back(1);
	_actions.push_back(2);
	_actions.push_back(3);
	
}


void UnitView::updatePosition(const MPoint& pos) {
	_pos.x = 64.0f + (GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f;
	_pos.y = 64.0f + (GLfloat)pos.y * 50.0f;
}

void UnitView::updateActions(std::vector<int> actions) {
	_actions = actions;
}

void UnitView::draw() {
	this->drawAt(_pos);
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

bool UnitView::wasTouched(GPoint point) {
	if (point.x >= _pos.x - width/2 && point.x <= _pos.x + width/2 && point.y >= _pos.y - height/2 && point.y <= _pos.y + height/2) {
		return true;
	} else {
		return false;
	}
}

GPoint UnitView::getActionPosition(int index) {
	return GPointMake(cos(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index), 
					  sin(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index)) * ACTION_RADIUS;
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
