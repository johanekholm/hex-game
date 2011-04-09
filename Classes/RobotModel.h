/*
 *  RobotModel.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"
//#include "RobotView.h"

#define GEOM_TILE_SIZE 32.0f

#define GEOM_DIR_NORTH 1
#define GEOM_DIR_EAST 2
#define GEOM_DIR_SOUTH 3
#define GEOM_DIR_WEST 4

class IRobotView;

class RobotModel { //: public IDrawable {
	int _tx, _ty;
	MPoint _tilePos;
	GPoint _transitPos;
	GPoint _vel;
	bool _inTransit;
	int _direction;
	IRobotView* _view;
	
public:
	RobotModel(int tx, int ty);
	void setVelocity(GPoint vel);
	GPoint getPosition();
	void update();
	void registerView(IRobotView* view);
	
};