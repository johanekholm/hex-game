/*
 *  RobotModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RobotModel.h"
#include "IRobotView.h"
#include <math.h>

RobotModel::RobotModel(int tx, int ty) {
	_tilePos.x = tx;
	_tilePos.y = ty;
	_vel = 0.0f;
	_transitPos = 0.0f;
	_inTransit = false;
}

void RobotModel::setVelocity(GPoint vel) {
	if ((vel.x != 0.0f || vel.y != 0.0f) && _inTransit == false) {
		if (abs(vel.x) >= abs(vel.y)) {
			_vel.x = vel.x;
			if (_vel.x > 0) {
				_direction = GEOM_DIR_EAST;				
			} else if (_vel.x < 0) {
				_direction = GEOM_DIR_WEST;				
			}
		} else {
			_vel.y = vel.y;
			if (_vel.y > 0) {
				_direction = GEOM_DIR_SOUTH;				
			} else if (_vel.y < 0) {
				_direction = GEOM_DIR_NORTH;				
			}
			
		}
		_inTransit = true;	
	}
}

void RobotModel::update() {
	MPoint delta;
	
	if (_inTransit) {
		_transitPos += _vel;
		
		
		if (_transitPos.x > GEOM_TILE_SIZE) {
			delta.x = (int)(_transitPos.x / GEOM_TILE_SIZE);
			delta.y = (int)(_transitPos.y / GEOM_TILE_SIZE);
			
			// adjust tile position and reduce transit position
			_tilePos += delta;
			_transitPos -= delta * GEOM_TILE_SIZE;		
			
			if (true/*!_board->canIPass(_tilePos, _direction)*/) {
				_inTransit = false;
				_vel = 0.0f;
				_transitPos = 0.0f;
			}	
		}

		// send updated position to view
		_view->updatePosition(this->getPosition());
	}
	
	
}

GPoint RobotModel::getPosition() {
	return _tilePos * GEOM_TILE_SIZE + _transitPos; 
}

void RobotModel::registerView(IRobotView* view) {
	_view = view;
}
