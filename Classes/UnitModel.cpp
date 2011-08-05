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
#include <cstdlib>
#include <iostream>

UnitModel::~UnitModel() {
    this->updateObserversDestroyed();
}

UnitModel::UnitModel(int x, int y, int direction, int owner, int maxHp, int maxAp, int power, int skill, int defense, std::vector<int> actionIds) {
    _owner = owner;
	_pos.x = x;
	_pos.y = y;
	_direction = direction;
    _maxHp = maxHp;
    _maxAp = maxAp;
    _hp = _maxHp;
    _ap = 0;
    _basePower = power;
    _baseSkill = skill;
    _baseDefense = defense;
    _target = 0;

    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
	
    //this->addAction(0);
	//this->addAction(3);
   	//this->addAction(4);
}


void UnitModel::setId(int unitId) {
    _id = unitId;
}

Action* UnitModel::addAction(int action) {
	_actions[action] = new Action(action, this);
	return _actions[action];
}

void UnitModel::doAction(const ActionState& statePoint) {
    std::cout << "Do action " << statePoint.actionId << std::endl;
	if (_actions.find(statePoint.actionId) != _actions.end()) {
		_actions[statePoint.actionId]->doIt(statePoint);
	}
	
	
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

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

int UnitModel::getOwner() {
	return _owner;
}

int UnitModel::getDirection() {
	return _direction;
}

std::vector<ActionState> UnitModel::getActions() {
	std::vector<ActionState> actionPoints, temp;
    std::map<int, HexState> hexes;
    std::vector<UnitModel*> units;
    std::vector<MPoint> targets;
    //ActionState a;


    hexes = ModelManager::instance()->getMap()->getAllHexes();
	units = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, Action*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(_ap, hexes, units);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

int UnitModel::getStat(int stat) {
    switch (stat) {
        case STAT_POWER:
            return _basePower;
            break;

        case STAT_SKILL:
            return _baseSkill;
            break;

        case STAT_DEFENSE:
            return _baseDefense;
            break;

        case STAT_MAXHP:
            return _maxHp;
            break;

        case STAT_MAXAP:
            return _maxAp;
            break;

        default:
            break;
    }
    return 0;
}

UnitState UnitModel::getState() {
    UnitState state;
    
    state.pos = _pos;
    state.direction = _direction;
    state.ap = _ap;
    state.hp = _hp;
    state.maxAp = _maxAp;
    state.maxHp = _maxHp;
    state.actions = this->getActions();
    
    return state;
}

void UnitModel::move(const MPoint& targetPos) {
    if (ModelManager::instance()->getUnitAtPos(targetPos) == 0) {
        _pos = targetPos;        
    }
	
	this->updateObservers();
}

void UnitModel::rotate(int rotation) {
	_direction += rotation;
	
	if (_direction < 1) {
		_direction += 6;
	} else if (_direction > 6) {
		_direction -= 6;
	}
	
	this->updateObservers();
}

void UnitModel::strike(const MPoint& targetPos) {
    UnitModel* target;
    
    std::cout << "Strike!" << std::endl;

    target = ModelManager::instance()->getUnitAtPos(targetPos);
    if (target != 0) {
        target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_SLICE);    
    }
}

void UnitModel::fire(const MPoint& targetPos) {
    UnitModel* target;

    std::cout << "Fire!" << std::endl;
    
    target = ModelManager::instance()->getUnitAtPos(targetPos);
    
    if (target != 0) {
        target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_PIERCE);    
    }
    
    /*    UnitModel* target;
    MPoint projectileVector;
    
    if (range < 1) {
        return;
    }
    
    std::cout << "Fire!" << std::endl;
    projectileVector = getHexVector(_direction, _pos);
    
    for (int d = 1; d <= range; d++) {
        std::cout << "p-vec: (" << projectileVector.x << ", " << projectileVector.y << ")" << std::endl;
        
        target = ModelManager::instance()->getUnitAtPos(_pos + projectileVector);

        if (target != 0) {
            target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_PIERCE);
            return;
        }
        projectileVector += projectileVector;
    }*/
}

void UnitModel::defend(UnitModel* attacker, int power, int skill, int attackType) {
    int skillDiff, damage, roll;
    
    damage = 0;
    skillDiff = this->getStat(STAT_DEFENSE) - skill;
    
    for (int i=0; i < power; i++) {
        roll = rand() % 8 + 1;
        if (roll + skillDiff >= 5 || roll == 8) {
            damage++;
        }    
    }
    
    std::cout << "Damage: " << damage << std::endl;
    //attacker->reportHits(damage);
    this->inflictDamage(damage);
}

void UnitModel::inflictDamage(int damage) {
    std::stringstream ss;
    ss << damage;
    MessageView::add(_pos, ss.str());
    
	_hp -= damage;
    if (_hp <= 0) {
        _hp = 0;
        std::cout << "Destroyed" << std::endl;
        ModelManager::instance()->remove(_id);
        return;
    }
    this->updateObservers();
}

void UnitModel::tick() {
    if (this->_ap < this->getStat(STAT_MAXAP)) {
        this->_ap += 1;
        //this->doAI();
        this->updateObservers();
    }
	
}

void UnitModel::doAI() {
/*    MPoint targetPos;

    if (_owner == 2) {

        if (_target == 0) {
            this->chooseTarget();
        } else {
            targetPos = _target->getPosition();
        
            if (this->isFacing(targetPos)) {
                this->doAction(3);
            } else {
                this->moveTowards(targetPos);
            }
        }    
    }*/
}

void UnitModel::chooseTarget() {
    _target = ModelManager::instance()->getClosestTo(_pos);
}

void UnitModel::turnTowards(const MPoint& targetPos) {
/*    int newDirection, delta;
    
    newDirection = directionTowards(_direction, _pos, targetPos);
    
    std::cout << "Turn towards: " << newDirection << std::endl;
    
    if (newDirection != _direction) {
        delta = newDirection - _direction;
        
        if (delta > 3) {
            delta -= 6;
        }
        
        if (delta > 0) {
            this->doAction(1);
        } else {
            this->doAction(2);
        }
    }*/
}

void UnitModel::moveTowards(const MPoint& targetPos) {
/*    int newDirection, delta;
    
    newDirection = directionTowards(_direction, _pos, targetPos);
    
    std::cout << "Turn towards: " << newDirection << ", old: " << _direction << std::endl;
    
    if (newDirection != _direction) {
        delta = newDirection - _direction;
        
        if (delta > 3) {
            delta -= 6;
        }
        
        if (delta > 0) {
            this->doAction(1);
        } else {
            this->doAction(2);
        }
    } else {
        if (hexDistance(_pos, targetPos) > 1) {
            this->doAction(0);
        }
    }
 */   
}


bool UnitModel::isFacing(const MPoint& targetPos) {
    return (hexDistance(_pos, targetPos) == 1 && sightDirection(_pos, targetPos) == _direction);
}


