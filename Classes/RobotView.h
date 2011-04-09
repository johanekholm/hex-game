/*
 *  RobotView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameImage.h"
#include "IRobotView.h"
//#include "RobotModel.h"

class RobotModel;

class RobotView : public GameImage, public IRobotView {
	RobotModel *_model;
	GPoint _pos;
	
public:
	RobotView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	void setModel(RobotModel* robot);
	void draw();
	void updatePosition(const GPoint& pos);
};