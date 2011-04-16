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
#include "Action.h"
#include "geometry.h"

UnitModel::~UnitModel() {
	//delete _actions;
}

UnitModel::UnitModel(int x, int y) {
	_pos.x = x;
	_pos.y = y;
	_direction = GEOM_DIR_E;
	addAction(0);
	addAction(1);
	addAction(2);
	addAction(3);
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

Action* UnitModel::addAction(int action) {
	_actions[action] = new Action(action, this);
	return _actions[action];
}

void UnitModel::doAction(int action) {
	if (_actions.find(action) != _actions.end()) {
		_actions[action]->doIt();
	}
	
	
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
	_view->updatePosition(_pos, _direction);
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

void UnitModel::move(int distance) {

	MPoint v = getHexVector(_direction, _pos);
	_pos += v;
	
	NSLog(@"moved - x: %i, y: %i", v.x, v.y);
	
	this->updateViews();
}

void UnitModel::rotate(int rotation) {
	_direction += rotation;
	
	if (_direction < 1) {
		_direction += 6;
	} else if (_direction > 6) {
		_direction -= 6;
	}
	
	this->updateViews();
	
	NSLog(@"direction: %i", _direction);
}

void UnitModel::strike() {
	
}


