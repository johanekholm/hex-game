/*
 *  RobotModel.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"
#include "IDrawable.h"

class RobotModel : public IDrawable {
	int _tx, _ty;
	GPoint _transitPos;
	GPoint _vel;
	bool _inTransit;
	
public:
	RobotModel(int tx, int ty);
	void setVelocity(GPoint vel);
	GPoint getDrawPosition();
	void update();
	
};