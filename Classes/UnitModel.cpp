/*
 *  UnitModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitModel.h"
#include "Action.h"
#include "ModelManager.h"
#include "MessageView.h"
#include "HexMapModel.h"
#include "geometry.h"
#include "json.h"
#include <cstdlib>
#include <iostream>

UnitModel::~UnitModel() {
    this->updateObserversDestroyed();
}

UnitModel::UnitModel() {}

UnitModel::UnitModel(int x, int y, int owner, int maxHp, int maxAp, int power, int skill, int defense, std::vector<int> actionIds, int visualType) {
    _owner = owner;
	_pos.x = x;
	_pos.y = y;
    _maxHp = maxHp;
    _maxAp = maxAp;
    _hp = _maxHp;
    _ap = rand() % 8; //_maxAp;
    _basePower = power;
    _baseSkill = skill;
    _baseDefense = defense;
    _target = 0;
    _visualType = visualType;
	_name = "UNIT";

    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
	
}

Json::Value UnitModel::serialize() {
    Json::Value root;
    
    Json::Value& actions = root["actions"];
    Json::Value& equip = root["equip"];
	
    root["x"] = _pos.x;
    root["y"] = _pos.y;
    root["ap"] = _ap;
    root["hp"] = _hp;
    root["basePower"] = _basePower;
    root["baseSkill"] = _baseSkill;
    root["baseDefense"] = _baseDefense;
    root["maxAp"] = _maxAp;
    root["maxHp"] = _maxHp;
    root["owner"] = _owner;
    root["visualType"] = _visualType;
    root["id"] = _id;
    
    // serialize actions
    for (std::map<int, BattleAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        actions.append(it->first);
    }
	
	// serialize items
	equip = this->serializeEquippedItems();

    return root;
}

void UnitModel::deserialize(Json::Value& root) {
    _pos.x = root.get("x", 0.0f).asFloat();
    _pos.y = root.get("y", 0.0f).asFloat();
    _ap = root.get("ap", 0).asInt();
    _hp = root.get("hp", 0).asInt();
    _basePower = root.get("basePower", 0).asInt();
    _baseSkill = root.get("baseSkill", 0).asInt();
    _baseDefense = root.get("baseDefense", 0).asInt();
    _maxAp = root.get("maxAp", 0).asInt();
    _maxHp = root.get("maxHp", 0).asInt();
    _owner = root.get("owner", 0).asInt();
    _visualType = root.get("visualType", 0).asInt();
    _id = root.get("id", 0).asInt();
	_name = "UNIT";
    
    // deserialize actions
    for (Json::ValueIterator it = root["actions"].begin(); it != root["actions"].end(); it++) {
        this->addAction((*it).asInt());
    }
	
	this->deserializeEquippedItems(root["equip"]);
}


BattleAction* UnitModel::addAction(int action) {
	_actions[action] = BattleAction::build(action, this); //new Action(action, this);
	return _actions[action];
}

void UnitModel::chooseTarget() {
    _target = ModelManager::instance()->getClosestTo(_pos);
}

int UnitModel::chooseMovementTarget(const std::vector<ActionState>& targets) {
    int distance, minDistance;
    int index = 0, bestAction = 0;
    
    for (std::vector<ActionState>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
        distance = ModelManager::instance()->getDistanceToClosestEnemy(2, (*it).pos);
        
        if (distance < minDistance) {
            minDistance = distance;
            bestAction = index;
        }
        index++;
    }
    
    return bestAction;
}

void UnitModel::defend(UnitModel* attacker, int power, int skill, int attackType) {
    int skillDiff, damage, roll;
    
    damage = 0;
    skillDiff = skill - this->getStat(STAT_DEFENSE);
    
    for (int i=0; i < power; i++) {
        roll = rand() % 8 + 1;
        DEBUGLOG("%i + %i", roll, skillDiff);
        
        if (roll + skillDiff >= 5 || roll == 8) {
            damage++;
        }
    }
    
    //attacker->reportHits(damage);
    this->inflictDamage(damage);
}

void UnitModel::doAction(const ActionState& statePoint) {
	if (_actions.find(statePoint.actionId) != _actions.end()) {
		_actions[statePoint.actionId]->doIt(statePoint);
	}
}

void UnitModel::doAI() {
    bool hasOffensiveAction = false;
    bool hasMovement = false;
    std::vector<ActionState> actionPoints, offensives, movements;
        
    if (_owner == 2) {
        actionPoints = this->getActions();
        
        for (std::vector<ActionState>::iterator it = actionPoints.begin(); it != actionPoints.end(); ++it) {
            switch ((*it).actionType) {
                case ActionNS::TYPE_ATTACK:
                    hasOffensiveAction = true;
                    if ((*it).active) {
                        offensives.push_back(*it);                        
                    }
                    break;
                    
                case ActionNS::TYPE_MOVEMENT:
                    hasMovement = true;
                    movements.push_back(*it);
                    break;
                    
                default:
                    break;
            }
        }
        
        if (hasOffensiveAction) {
            
            // choose offensive action randomly
            if (offensives.size() > 0) {
                this->doAction(offensives.at(rand() % offensives.size()));
            } else {
                // unit has to wait until offensive action becomes active, do nothing this tick
            }
        } else {
            // move closer to one unit
            if (movements.size() > 0) {
                this->doAction(movements.at(this->chooseMovementTarget(movements)));
            }
        }
    }
}

void UnitModel::fire(const MPoint& targetPos) {
    UnitModel* target;
        
    target = ModelManager::instance()->getUnitAtPos(targetPos);
    
    if (target != 0) {
        target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_PIERCE);    
    }
}

std::vector<ActionState> UnitModel::getActions() {
	std::vector<ActionState> actionPoints, temp;
    std::map<int, HexState> hexes;
    std::vector<UnitModel*> units;
    std::vector<MPoint> targets;
    //ActionState a;
    
    
    hexes = ModelManager::instance()->getMap()->getAllHexes();
	units = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, BattleAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(_ap, hexes, units);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

std::string UnitModel::getDescription() {
    std::stringstream stream;
	
    stream << _name << " " << _hp << "/" << _maxHp;
    return stream.str();
}


int UnitModel::getId() {
	return _id;
}

int UnitModel::getOwner() {
	return _owner;
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}


int UnitModel::getStat(int stat) {
    switch (stat) {
        case STAT_POWER:
            return _basePower;

        case STAT_SKILL:
            return _baseSkill;

        case STAT_DEFENSE:
            return _baseDefense;

        case STAT_MAXHP:
            return _maxHp;

        case STAT_MAXAP:
            return _maxAp;

        default:
            break;
    }
    return 0;
}

UnitState UnitModel::getState() {
    UnitState state;
    
    state.pos = _pos;
    state.ap = _ap;
    state.hp = _hp;
    state.maxAp = _maxAp;
    state.maxHp = _maxHp;
    state.actions = this->getActions();
    
    return state;
}

int UnitModel::getVisualType() {
    return _visualType;
}

void UnitModel::inflictDamage(int damage) {
    std::stringstream ss;
    

    if (damage > 0) {
        ss << damage;
        MessageView::add(_pos, ss.str(), 1.0f, 0.0f, 0.0f, 1.0f);
    } else if (damage == 0) {
        MessageView::add(_pos, "MISS");
    } else if (damage < 0) {
        ss << -damage;
        MessageView::add(_pos, ss.str(), 0.0f, 1.0f, 0.0f, 1.0f);
    }
    
	_hp -= damage;
    
    if (_hp <= 0) {
        _hp = 0;
        ModelManager::instance()->removeUnit(_id);
        return;
    } else if (_hp > _maxHp) {
        _hp = _maxHp;
    }
    this->updateObservers();
}

void UnitModel::move(const MPoint& targetPos) {
    if (ModelManager::instance()->getUnitAtPos(targetPos) == 0) {
        _pos = targetPos;        
    }
	
	this->updateObservers();
}

void UnitModel::setId(int unitId) {
    _id = unitId;
}

bool UnitModel::spendAp(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
        this->updateObservers();
		return true;
	} else {
		return false;
	}
}

void UnitModel::strike(const MPoint& targetPos) {
    UnitModel* target;
    
    target = ModelManager::instance()->getUnitAtPos(targetPos);
    if (target != 0) {
        target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_SLICE);    
    }
}

void UnitModel::tick() {
    if (this->_ap < this->getStat(STAT_MAXAP)) {
        this->_ap += 1;
        this->doAI();
        this->updateObservers();
    }
	
}


