/*
 *  Action.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Action.h"
#include "UnitModel.h"
#include <string>

//using namespace std;

Action::Action(int anId, UnitModel* unit) {
	_id = anId;	
	_unit = unit;
}

void Action::doIt() {
	switch (_id) {
		case 0:
			_unit->move(1);			
			break;
		case 1:
			_unit->rotate(1);			
			break;
		case 2:
			_unit->rotate(-1);			
			break;
		case 3:
			_unit->strike();			
			break;
			
		default:
			break;
	}
}
