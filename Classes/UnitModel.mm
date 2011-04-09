/*
 *  UnitModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitModel.h"

UnitModel::~UnitModel() {
	//delete _actions;
}


/*
void UnitModel::registerAction(Action *aAction) {
	actions->add(aAction->register(this));
	
}

*/



bool UnitModel::spendAP(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
		return true;
	} else {
		return false;
	}
}



MPoint UnitModel::getPosition() {
	return MPointMake(this->_x, this->_y);
}

