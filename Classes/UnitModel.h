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
#define ATTACK_TYPE_PIERCE 2

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

struct UnitState {
    MPoint pos;
    int direction;
    int ap;
    int hp;
    int maxAp;
    int maxHp;
    std::vector<int> actions;
};

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
    int _owner;
    UnitModel* _target;

	std::map<int, Action*> _actions;
    
    void chooseTarget();
    void turnTowards(const MPoint& targetPos);
    void moveTowards(const MPoint& targetPos);
	
public:
	
	~UnitModel();
	UnitModel(int x, int y, int direction, int owner);
    UnitModel(int x, int y, int direction, int owner, int maxHp, int maxAp, int power, int skill, int defense);
    
    void tick();
	bool spendAp(int cost);
	void move(int distance);
	void rotate(int rotation);
	void strike();
    void fire(int range);
    void defend(UnitModel* attacker, int power, int skill, int attack_type);
    void inflictDamage(int damage);
    bool isDead();
    int getStat(int stat);
    UnitState getState();
    void doAI();
    //int distanceTo(const MPoint& pos);
    bool isFacing(const MPoint& pos);
	
	MPoint getPosition();
    int getOwner();
	int getDirection();
	std::vector<int> getActions();
	
	Action* addAction(int action);
	void doAction(int action);
	
};

#endif