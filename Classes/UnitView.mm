/*
 *  UnitView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitView.h"
//#include "UnitModel.h"

UnitView::UnitView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index) : GameImage(aWidth, aHeight, tex, index){
	_actionImage = new GameImage(48.0f, 48.0f, tex, 0);
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
	
}

bool UnitView::wasTouched(GPoint point) {
	if (point.x >= _pos.x - width/2 && point.x <= _pos.x + width/2 && point.y >= _pos.y - height/2 && point.y <= _pos.y + height/2) {
		return true;
	} else {
		return false;
	}
}
