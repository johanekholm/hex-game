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
#define ATTACK_TYPE_FIRE 3

#define STAT_POWER 1
#define STAT_SKILL 2
#define STAT_DEFENSE 3
#define STAT_MAXHP 4
#define STAT_MAXAP 5

#include <map>
#include <vector>
#include <string>
#include "toolkit.h"
#include "Observable.h"
#include "json-forwards.h"

class Action;
class BattleAction;
struct ActionState;

struct UnitState {
    MPoint pos;
    int ap;
    int hp;
    int maxAp;
    int maxHp;
    std::vector<ActionState> actions;
};

class UnitModel : public Observable {
	MPoint _pos;
    int _id;
	int _ap;
	int _hp;
    int _basePower;
    int _baseSkill;
    int _baseDefense;
    int _maxAp;
    int _maxHp;
	std::string _name;
    int _owner;
    int _visualType;
    UnitModel* _target;

	std::map<int, BattleAction*> _actions;
    
    void chooseTarget();
	
public:
	
	~UnitModel();
    UnitModel();
    UnitModel(int x, int y, int owner, int maxHp, int maxAp, int power, int skill, int defense, std::vector<int> actionIds, int visualType);
    Json::Value serialize();
    void deserialize(Json::Value& root);
    
	BattleAction* addAction(int action);
    int chooseMovementTarget(const std::vector<ActionState>& targets);
    void defend(UnitModel* attacker, int power, int skill, int attack_type);
	void doAction(const ActionState& statePoint);
    void doAI();
	std::vector<ActionState> getActions();
	std::string getDescription();
    int getOwner();
	int getId();
	MPoint getPosition();
    int getStat(int stat);
    bool isFacing(const MPoint& pos);
    UnitState getState();
    int getVisualType();
    void inflictDamage(int damage);
    void fire(const MPoint& targetPos);
	void move(const MPoint& targetPos);
    void setId(int unitId);
	bool spendAp(int cost);
	void strike(const MPoint& targetPos);
    void tick();
};

#endif