/*
 *  RobotModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RobotModel.h"
#include <math.h>

RobotModel::RobotModel(int tx, int ty) {
	_tx = tx;
	_ty = ty;
	_vel.x = 0.0f;
	_vel.y = 0.0f;
	_transitPos.x = 0.0f;
	_transitPos.y = 0.0f;
	_inTransit = false;

}

void RobotModel::setVelocity(GPoint vel) {
	if ((vel.x != 0.0f || vel.y != 0.0f) && _inTransit == false) {
		if (abs(vel.x) >= abs(vel.y)) {
			_vel.x = vel.x;
		} else {
			_vel.y = vel.y;
		}
		_inTransit = true;	
	}
}

void RobotModel::update() {
	if (_inTransit) {
		_transitPos += _vel;		
	}
	
	if (_transitPos.x > 100.0f || _transitPos.x < -100.0f || _transitPos.y > 100.0f || _transitPos.y < -100.0f) {
		_vel.x = 0.0f;
		_vel.y = 0.0f;
		_transitPos.x = 0.0f;
		_transitPos.y = 0.0f;		
		_inTransit = false;
	}
}

GPoint RobotModel::getDrawPosition() {
	return GPointMake(_tx * 32.0f + _transitPos.x, _ty * 32.0f + _transitPos.y); //GPointMake(100.0f, 100.0f);
}
