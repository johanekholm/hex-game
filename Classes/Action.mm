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

Action::Action(int anId) {
	_id = anId;	
}

void Action::doAction() {
	switch (_id) {
		case 1:
			_unit->move(1);			
			break;
			
		default:
			break;
	}

}
/*
void Action::registerView(IActionView view) {
	_view = view;
}*/