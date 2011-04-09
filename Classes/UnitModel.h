/*
 *  UnitModel.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "toolkit.h"

class Action;

using namespace std;

class UnitModel {
	int _x, _y;	
	int _ap;
	int _health;
	vector<Action*> _actions;
	
public:
	
	~UnitModel();
	UnitModel(int x, int y);
	
	/*Action* getActionWithIndex(int index);
	void performActionWithIndex(int index);
	void registerAction(Action *aAction);
	*/
	bool spendAP(int cost);
	//void move(int distance);
	MPoint getPosition();
	
};
