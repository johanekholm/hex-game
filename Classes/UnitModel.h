/*
 *  UnitModel.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <map>
#include "toolkit.h"

class Action;
class IUnitView;


class UnitModel {
	MPoint _pos;
	int _direction;
	int _ap;
	int _health;
	std::map<int, Action*> _actions;
	IUnitView* _view;
	
public:
	
	~UnitModel();
	UnitModel(int x, int y);
	
	/*Action* getActionWithIndex(int index);
	void performActionWithIndex(int index);
	void registerAction(Action *aAction);
	*/
	bool spendAP(int cost);
	void move(int distance);
	void rotate(int rotation);
	void strike();
	MPoint getPosition();
	void registerView(IUnitView* view);
	void updateViews();
	Action* addAction(int action);
	void doAction(int action);
	
};
