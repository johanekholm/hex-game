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
#include "geometry.h"
#include <iostream>

UnitModel::~UnitModel() {
    this->updateObserversDestroyed();
	//delete _actions;
}

UnitModel::UnitModel(int x, int y, int direction, int owner) {
    _owner = owner;
	_pos.x = x;
	_pos.y = y;
	_direction = direction;
    _maxHp = 3;
    _maxAp = 4;
    _hp = 1; //_maxHp;
    _ap = 0;
    _basePower = 3;
    _baseSkill = 2;
    _baseDefense = 1;
    _target = 0;
	addAction(1);
	addAction(0);
	addAction(2);
	addAction(3);
}

UnitModel::UnitModel(int x, int y, int direction, int owner, int maxHp, int maxAp, int power, int skill, int defense) {
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
	this->addAction(1);
	this->addAction(0);
	this->addAction(2);
	this->addAction(3);
   	this->addAction(4);
}


/*
void UnitModel::registerAction(Action *aAction) {
	actions->add(aAction->register(this));
	
}

*/


Action* UnitModel::addAction(int action) {
	_actions[action] = new Action(action, this);
	return _actions[action];
}

void UnitModel::doAction(int action) {
	if (_actions.find(action) != _actions.end()) {
		_actions[action]->doIt();
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

std::vector<int> UnitModel::getActions() {
	std::vector<int> v;
	
	for(std::map<int, Action*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        v.push_back(it->first);
    }
    
	return v;
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

void UnitModel::move(int distance) {

	MPoint v = getHexVector(_direction, _pos);
    if (ModelManager::instance()->getUnitAtPos(_pos + v) == 0) {
        _pos += v;        
    }
	
	//NSLog(@"moved - x: %i, y: %i", v.x, v.y);
	
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
	
	//NSLog(@"direction: %i", _direction);
}

void UnitModel::strike() {
    UnitModel* target;
    
    std::cout << "Strike!" << std::endl;
    target = ModelManager::instance()->getUnitAtPos(_pos + getHexVector(_direction, _pos));
    if (target != 0) {
        target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_SLICE);    
    }
}

void UnitModel::fire(int range) {
    UnitModel* target;
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
    }
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
	_hp -= damage;
    if (_hp <= 0) {
        _hp = 0;
        //std::cout << "Destroyed" << std::endl;
        //ModelManager::instance()->remove(this);
    }
    this->updateObservers();
}

bool UnitModel::isDead() {
    return (_hp == 0);
}

void UnitModel::tick() {
    if (this->_ap < this->getStat(STAT_MAXAP)) {
        this->_ap += 1;
        this->doAI();
        this->updateObservers();
    }
	
}

void UnitModel::doAI() {
    MPoint targetPos;

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
    }
}

void UnitModel::chooseTarget() {
    _target = ModelManager::instance()->getClosestTo(_pos);
}

void UnitModel::turnTowards(const MPoint& targetPos) {
    int newDirection, delta;
    
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
    }
}

void UnitModel::moveTowards(const MPoint& targetPos) {
    int newDirection, delta;
    
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
    
}


bool UnitModel::isFacing(const MPoint& targetPos) {
    return (hexDistance(_pos, targetPos) == 1 && sightDirection(_pos, targetPos) == _direction);
}

