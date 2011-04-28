/*
 *  ViewController.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ViewController.h"
#include "toolkit.h"
#include "InputManager.h"

bool ViewController::isWithin(const GPoint& point) {
	if (point.x >= _pos.x - _width/2 && point.x <= _pos.x + _width/2 && point.y >= _pos.y - _height/2 && point.y <= _pos.y + _height/2) {
		return true;
	} else {
		return false;
	}
}

/*bool ViewController::handleEvent(const TouchEvent& event) {
	if (this->isWithin(event.point)) {
		this->reactToEvent(event.type);
		return true;
	}
	return false;
}*/
