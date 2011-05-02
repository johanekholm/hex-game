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
//class IUnitView;

class UnitModel : public Observable {
	MPoint _pos;
	int _direction;
	int _ap;
	int _health;
	std::map<int, Action*> _actions;
//	IUnitView* _view;
	
public:
	
	~UnitModel();
	UnitModel(int x, int y);
	
	/*Action* getActionWithIndex(int index);
	void performActionWithIndex(int index);
	void registerAction(Action *aAction);
	*/
    
    void tick();
	bool spendAP(int cost);
	void move(int distance);
	void rotate(int rotation);
	void strike();
	
	MPoint getPosition();
	int getDirection();
	std::vector<int> getActions();
	
	//void registerView(IUnitView* view);
	//void updateViews();
	Action* addAction(int action);
	void doAction(int action);
	
};

#endif