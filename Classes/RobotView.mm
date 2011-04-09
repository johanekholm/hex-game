/*
 *  RobotView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RobotView.h"
#include "RobotModel.h"

RobotView::RobotView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index) : GameImage(aWidth, aHeight, tex, index){
	_model = NULL;
}

void RobotView::setModel(RobotModel* model) {
	_model = model;
	_pos = _model->getPosition();
}

void RobotView::updatePosition(const GPoint& pos) {
	_pos = pos;
}

void RobotView::draw() {
	this->drawAt(_pos);
}