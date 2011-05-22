/*
 *  Action.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ACTION_H
#define ACTION_H

#include <string>

class UnitModel;

struct ActionState {
    int actionId;
    bool active;
    int cost;
};

class Action {
	int _id;
	UnitModel* _unit;
	
public:
	Action(int anId, UnitModel* unit);
	void doIt();
    int getCost();
};

#endif