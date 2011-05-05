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

#define ATTACK_TYPE_SLICE 1

#define STAT_POWER 1
#define STAT_SKILL 2
#define STAT_DEFENSE 3
#define STAT_MAXHP 4
#define STAT_MAXAP 5

#include <map>
#include <vector>
#include "toolkit.h"
#include "Observable.h"

class Action;

class UnitModel : public Observable {
	MPoint _pos;
	int _direction;
	int _ap;
	int _hp;
    int _basePower;
    int _baseSkill;
    int _baseDefense;
    int _maxAp;
    int _maxHp;

	std::map<int, Action*> _actions;
	
public:
	
	~UnitModel();
	UnitModel(int x, int y);
    
    void tick();
	bool spendAP(int cost);
	void move(int distance);
	void rotate(int rotation);
	void strike();
    void defend(UnitModel* attacker, int power, int skill, int attack_type);
    void inflictDamage(int damage);
    int getStat(int stat);
	
	MPoint getPosition();
	int getDirection();
	std::vector<int> getActions();
	
	Action* addAction(int action);
	void doAction(int action);
	
};

#endif