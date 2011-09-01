/*
 *  Action.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Action.h"
#include "UnitModel.h"
#include "geometry.h"
#include "ModelManager.h"
#include "HexMapModel.h"
#include "MessageView.h"
#include "MapObject.h"
#include "Sound.h"
#include <iostream>
#include <string>


Action::Action(int anId, UnitModel* unit) {
	_id = anId;	
	_unit = unit;
    
    switch (_id) {
		case ACTION_MOVE:
            _cost = 2;
            _targetType = TARGET_HEX;
            _type = ACTION_TYPE_MOVEMENT;
            _name = "MOVE";
            break;
		case ACTION_STRIKE:
            _cost = 2;
            _targetType = TARGET_UNIT;
            _type = ACTION_TYPE_ATTACK;
            _name = "STRIKE";
            break;
		case ACTION_FIRE:
            _cost = 2;
            _targetType = TARGET_UNIT;
            _type = ACTION_TYPE_ATTACK;
            _name = "FIRE";
            break;
        case ACTION_BURN:
            _cost = 4;
            _targetType = TARGET_UNIT;
            _type = ACTION_TYPE_ATTACK;
            _name = "BURN";
            break;
        case ACTION_GALE:
            _cost = 2;
            _targetType = TARGET_UNIT;
            _type = ACTION_TYPE_DEFENSE;
            _name = "GALE";
            break;
        case ACTION_HEAL:
            _cost = 3;
            _targetType = TARGET_UNIT;
            _type = ACTION_TYPE_DEFENSE;
            _name = "HEAL";
            break;

		default:
			_cost = 0;
            _name = "";
	}

}

void Action::doIt(const ActionState& statePoint) {
    UnitModel* target;
    MPoint v;
    //std::cout << "Action of id " << _id << std::endl;

    if (_unit->spendAp(this->getCost())) {
        MessageView::add(_unit->getPosition(), _name);
        Sound *sound = Sound::instance();
        
        switch (_id) {
            case ACTION_MOVE:
                _unit->move(statePoint.pos);			
                break;
            case ACTION_STRIKE:
                
                _unit->strike(statePoint.pos);
                sound->play("strike");

                break;
            case ACTION_FIRE:
                _unit->fire(statePoint.pos);
                sound->play("fire");
                break;
            case ACTION_BURN:
                //std::cout << "Burn!" << std::endl;
                
                target = ModelManager::instance()->getUnitAtPos(statePoint.pos);
                if (target != 0) {
                    target->defend(_unit, 4, 4, ATTACK_TYPE_FIRE);    
                }
                break;
                
            case ACTION_GALE:
                //std::cout << "Gale!" << std::endl;
                
                target = ModelManager::instance()->getUnitAtPos(statePoint.pos);
                if (target != 0) {
                    v = statePoint.pos + (statePoint.pos - _unit->getPosition());
                    //std::cout << "x: " << v.x << ", y: " << v.y << std::endl;
                    target->move(statePoint.pos + (statePoint.pos - _unit->getPosition()));
                }
                break;
                
            case ACTION_HEAL:
                target = ModelManager::instance()->getUnitAtPos(statePoint.pos);
                if (target != 0) {
                    target->inflictDamage(-2);
                }
                
            default:
                break;
        }
    }
}

int Action::getCost() {
    return _cost;
}

bool Action::isAvailableAtHex(const MPoint& hex) {
    int distance = hexDistance(_unit->getPosition(), hex);
    
	switch (_id) {
		case ACTION_MOVE:
            if (distance == 1) {
                return (ModelManager::instance()->getUnitAtPos(hex) == 0);
            }
            return false;
		default:
			return false;
	}
}

bool Action::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
	switch (_id) {
        case ACTION_STRIKE:
            return (distance == 1 && _unit->getOwner() != targetUnit->getOwner());
        case ACTION_FIRE:
            return (distance > 0 && distance <= 2  && _unit->getOwner() != targetUnit->getOwner());
        case ACTION_BURN:
            return (distance > 1 && distance <= 3);
        case ACTION_GALE:
            return (distance == 1 && _unit->getOwner() != targetUnit->getOwner());
        case ACTION_HEAL:
            return (distance == 1 && _unit->getOwner() == targetUnit->getOwner());
		default:
			return false;
	}
}

std::vector<ActionState> Action::getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<UnitModel*>& units) {
    ActionState state;
    std::vector<ActionState> actionPoints;
    
    state.actionId = _id;
    state.cost = _cost;
    state.actionType = _type;
    state.active = (ap >= _cost);
    
    if (_targetType == TARGET_HEX) {
        for (std::map<int, HexState>::const_iterator it = hexes.begin(); it != hexes.end(); ++it) {
            if (this->isAvailableAtHex((it->second).pos)) {
                state.pos = (it->second).pos;
                actionPoints.push_back(state);
                //std::cout << "Action (hex) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    } else if (_targetType == TARGET_UNIT) {
        for (std::vector<UnitModel*>::const_iterator it = units.begin(); it != units.end(); ++it) {
            if (this->isAvailableToUnit(*it)) {
                state.pos = (*it)->getPosition();
                actionPoints.push_back(state);
                //std::cout << "Action (unit) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    }
    return actionPoints;
}


/*---------------------------------------------------------------*/

BattleAction* BattleAction::build(int actionId, UnitModel* unit) {    
    switch (actionId) {
		case ACTION_MOVE:
            return new BActionMove(actionId, unit);
		case ACTION_STRIKE:
            return 0;
		case ACTION_FIRE:
            return 0;
        case ACTION_BURN:
            return 0;
        case ACTION_GALE:
            return 0;
        case ACTION_HEAL:
            return 0;
            
		default:
            return 0;
    }
    
}

BattleAction::BattleAction(const std::string& name, int anId, UnitModel* unit, int cost, int targetType, int actionType) {
	_id = anId;	
	_unit = unit;
    _cost = cost;
    _targetType = targetType;
    _type = actionType;
    _name = name;
}

void BattleAction::doIt(const ActionState& statePoint) {
    
    if (_unit->spendAp(this->getCost())) {
        MessageView::add(_unit->getPosition(), _name);
        Sound *sound = Sound::instance();
        sound->play("strike");
        
        this->doAction(statePoint);
    }
}

int BattleAction::getCost() {
    return _cost;
}

bool BattleAction::isAvailableAtHex(const MPoint& hex) {
	return false;
}

bool BattleAction::isAvailableToUnit(UnitModel* targetUnit) {
	return false;
}

std::vector<ActionState> BattleAction::getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<UnitModel*>& units) {
    ActionState state;
    std::vector<ActionState> actionPoints;
    
    state.actionId = _id;
    state.cost = _cost;
    state.actionType = _type;
    state.active = (ap >= _cost);
    
    if (_targetType == TARGET_HEX) {
        for (std::map<int, HexState>::const_iterator it = hexes.begin(); it != hexes.end(); ++it) {
            if (this->isAvailableAtHex((it->second).pos)) {
                state.pos = (it->second).pos;
                actionPoints.push_back(state);
                //std::cout << "Action (hex) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    } else if (_targetType == TARGET_UNIT) {
        for (std::vector<UnitModel*>::const_iterator it = units.begin(); it != units.end(); ++it) {
            if (this->isAvailableToUnit(*it)) {
                state.pos = (*it)->getPosition();
                actionPoints.push_back(state);
                //std::cout << "Action (unit) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    }
    return actionPoints;
}

/*---------------------------------------------------------------*/

BActionMove::BActionMove(int anId, UnitModel* unit) : BattleAction("MOVE", anId, unit, 0, TARGET_HEX, ACTION_TYPE_MOVEMENT) {}

bool BActionMove::isAvailableAtHex(const MPoint& hex) {
    if (hexDistance(_unit->getPosition(), hex) == 1) {
        return (ModelManager::instance()->getUnitAtPos(hex) == 0);
    }
    return false;
}

void BActionMove::doAction(const ActionState& statePoint) {
    _unit->move(statePoint.pos);			
}


/*---------------------------------------------------------------*/

AdventureAction* AdventureAction::build(int actionId, PartyModel* party) {
    switch (actionId) {
        case ADV_ACTION_MOVE:
            return new AdvActionMove(actionId, party);
        case ADV_ACTION_BATTLE:
            return 0; //new AdvActionBattle(actionId, party);
		default:
			return 0;
	}
}

AdventureAction::AdventureAction(const std::string& name, int anId, PartyModel* party, int cost, int targetType, int actionType) {
	_id = anId;	
	_party = party;
    _cost = cost;
    _targetType = targetType;
    _type = actionType;
    _name = name;
}

int AdventureAction::getCost() {
    return _cost;
}

std::vector<ActionState> AdventureAction::getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<PartyModel*>& parties) {
    ActionState state;
    std::vector<ActionState> actionPoints;
    
    state.actionId = _id;
    state.cost = _cost;
    state.actionType = _type;
    state.active = true;
    
    if (_targetType == TARGET_HEX) {
        for (std::map<int, HexState>::const_iterator it = hexes.begin(); it != hexes.end(); ++it) {
            if (this->isAvailableAtHex((it->second).pos)) {
                state.pos = (it->second).pos;
                actionPoints.push_back(state);
                //std::cout << "Action (hex) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    } else if (_targetType == TARGET_PARTY) {
        for (std::vector<PartyModel*>::const_iterator it = parties.begin(); it != parties.end(); ++it) {
            if (this->isAvailableToParty(*it)) {
                state.pos = (*it)->getPosition();
                actionPoints.push_back(state);
                //std::cout << "Action (unit) " << _id << " at (" << state.pos.x << ", " << state.pos.y << ")" << std::endl;
            }
        }
    }
    return actionPoints;
}

bool AdventureAction::isAvailableAtHex(const MPoint& hex) {
    return false;
}

bool AdventureAction::isAvailableToParty(PartyModel* targetParty) {
    return false;
}

/*---------------------------------------------------------------*/

AdvActionMove::AdvActionMove(int anId, PartyModel* party) : AdventureAction("MOVE", anId, party, 0, TARGET_HEX, ACTION_TYPE_MOVEMENT) { }

bool AdvActionMove::isAvailableAtHex(const MPoint& hex) {
    int distance = hexDistance(_party->getPosition(), hex);
    return (distance == 1);
}

void AdvActionMove::doIt(const ActionState& statePoint) {
    
}

