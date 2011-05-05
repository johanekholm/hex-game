/*
 *  UnitModel.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <map>
#include <vector>
#include "toolkit.h"
#include "Observable.h"

class Action;

class UnitModel : public Observable {
	MPoint _pos;
	int _direction;
	int _ap;
	int _health;
	std::map<int, Action*> _actions;
	
public:
	
	~UnitModel();
	UnitModel(int x, int y);
    
    void tick();
	bool spendAP(int cost);
	void move(int distance);
	void rotate(int rotation);
	void strike();
	
	MPoint getPosition();
	int getDirection();
	std::vector<int> getActions();
	
	Action* addAction(int action);
	void doAction(int action);
	
};

#endif