/*
 *  Action.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>

class UnitModel;

class Action {
	int _id;
	UnitModel* _unit;
	
public:
	Action(int anId, UnitModel* unit);
	void doIt();
};
