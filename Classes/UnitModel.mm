/*
 *  UnitModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitModel.h"
#include "IUnitView.h"
#include "geometry.h"

UnitModel::~UnitModel() {
	//delete _actions;
}

UnitModel::UnitModel(int x, int y) {
	_pos.x = x;
	_pos.y = y;
	_direction = GEOM_DIR_E;
}


/*
void UnitModel::registerAction(Action *aAction) {
	actions->add(aAction->register(this));
	
}

*/

void UnitModel::registerView(IUnitView* view) {
	_view = view;
	this->updateViews();
}

bool UnitModel::spendAP(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
		return true;
	} else {
		return false;
	}
}

void UnitModel::updateViews() {
	_view->updatePosition(_pos);
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

void UnitModel::move(int distance) {

	MPoint v = getHexVector(_direction, _pos);
	_pos += v;
	
	this->updateViews();
}

