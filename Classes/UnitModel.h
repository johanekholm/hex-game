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

#include <map>
#include <vector>
#include <string>
#include "toolkit.h"
#include "Item.h"
#include "Observable.h"
#include "json-forwards.h"

class Action;
class BattleAction;
struct ActionState;

namespace StatNS {
	const int POWER = 1;
	const int SKILL = 2;
	const int DEFENSE = 3;
	const int MAXHP = 4;
	const int MAXAP = 5;
};

struct UnitState {
    MPoint pos;
    int ap;
    int hp;
    int maxAp;
    int maxHp;
    std::vector<ActionState> actions;
};

class UnitModelTemplate {
private:
	static std::map<std::string, UnitModelTemplate*> _templates;
	std::string _templateId;
	std::string _name;
    int _visualType;
    int _basePower;
    int _baseSkill;
    int _baseDefense;
    int _maxAp;
    int _maxHp;
	std::vector<int> _actionIds;

public:
	static std::map<std::string, UnitModelTemplate*> initTemplates();
	static UnitModelTemplate* getTemplate(const std::string& templateId);
	UnitModelTemplate();
	UnitModelTemplate(const std::string& _templateId, const std::string& _name, int visualType, int basePower, int baseSkill, int baseDefense, int maxAp, int maxHp, const std::vector<int>& actionIds);
	std::vector<int> getActionIds();
	std::string getName();
	int getStat(int stat);
	std::string getTemplateId();
	int getVisualType();
};

class UnitModel : public Observable, public ItemEquipper {
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
	UnitModelTemplate* _template;

	std::map<int, BattleAction*> _actions;
    
    void chooseTarget();
	
public:
	
	~UnitModel();
    UnitModel();
	UnitModel(const std::string& templateId, const MPoint& pos, int owner);
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
	void setPosition(const MPoint& pos);
	bool spendAp(int cost);
	void strike(const MPoint& targetPos);
    void tick();
};

#endif