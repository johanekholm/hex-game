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

UnitModel::~UnitModel() {
	//delete _actions;
}

UnitModel::UnitModel(int x, int y) {
	_pos.x = x;
	_pos.y = y;
	_direction = GEOM_DIR_E;
    _maxHp = 5;
    _hp = _maxHp;
    _basePower = 3;
    _baseSkill = 2;
    _baseDefense = 1;
	addAction(1);
	addAction(0);
	addAction(2);
	addAction(3);
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

bool UnitModel::spendAP(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
		return true;
	} else {
		return false;
	}
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
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
    
    target = ModelManager::instance()->getUnitAtPos(_pos + getHexVector(_direction, _pos));
    target->defend(this, this->getStat(STAT_POWER), this->getStat(STAT_SKILL), ATTACK_TYPE_SLICE);
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
    
    //attacker->reportHits(damage);
    this->inflictDamage(damage);
}

void UnitModel::inflictDamage(int damage) {
	_hp -= damage;
    this->updateObservers();
}

void UnitModel::tick() {
    if (this->_ap < this->getStat(STAT_MAXAP)) {
        this->_ap += 1;
    }
	
}


