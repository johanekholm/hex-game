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


BattleAction* BattleAction::build(int actionId, UnitModel* unit) {    
    switch (actionId) {
		case ACTION_MOVE:
            return new BActionMove(actionId, unit);
		case ACTION_STRIKE:
            return new BActionStrike(actionId, unit);
		case ACTION_FIRE:
            return new BActionFire(actionId, unit);
        case ACTION_BURN:
            return new BActionBurn(actionId, unit);
        case ACTION_GALE:
            return 0;
        case ACTION_HEAL:
            return new BActionHeal(actionId, unit);
            
		default:
            return 0;
    }
    
}

BattleAction::BattleAction(int anId, UnitModel* unit, const std::string& name, int cost, int targetType, int actionType, const std::string& sound) {
	_id = anId;	
	_unit = unit;
    _cost = cost;
    _targetType = targetType;
    _type = actionType;
    _name = name;
    _sound = sound;
}

void BattleAction::doIt(const ActionState& statePoint) {
    
    if (_unit->spendAp(this->getCost())) {
        MessageView::add(_unit->getPosition(), _name);
        Sound *sound = Sound::instance();
        sound->play(_sound);
        
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

BActionMove::BActionMove(int anId, UnitModel* unit) : BattleAction(anId, unit, "MOVE", 1, TARGET_HEX, ACTION_TYPE_MOVEMENT, "") {}

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

BActionStrike::BActionStrike(int anId, UnitModel* unit) : BattleAction(anId, unit, "STRIKE", 2, TARGET_UNIT, ACTION_TYPE_ATTACK, "strike") {}

bool BActionStrike::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance == 1 && _unit->getOwner() != targetUnit->getOwner());
}

void BActionStrike::doAction(const ActionState& statePoint) {
    _unit->strike(statePoint.pos);
}

/*---------------------------------------------------------------*/

BActionFire::BActionFire(int anId, UnitModel* unit) : BattleAction(anId, unit, "FIRE", 2, TARGET_UNIT, ACTION_TYPE_ATTACK, "") {}

bool BActionFire::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance > 0 && distance <= 2  && _unit->getOwner() != targetUnit->getOwner());
}

void BActionFire::doAction(const ActionState& statePoint) {
    _unit->fire(statePoint.pos);
}

/*---------------------------------------------------------------*/

BActionHeal::BActionHeal(int anId, UnitModel* unit) : BattleAction(anId, unit, "HEAL", 2, TARGET_UNIT, ACTION_TYPE_DEFENSE, "heal") {}

bool BActionHeal::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance == 1 && _unit->getOwner() == targetUnit->getOwner());
}

void BActionHeal::doAction(const ActionState& statePoint) {
    UnitModel* target = ModelManager::instance()->getUnitAtPos(statePoint.pos);
    if (target != 0) {
        target->inflictDamage(-2);
    }
}

/*---------------------------------------------------------------*/

BActionBurn::BActionBurn(int anId, UnitModel* unit) : BattleAction(anId, unit, "BURN", 4, TARGET_UNIT, ACTION_TYPE_ATTACK, "fire") {}

bool BActionBurn::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance > 1 && distance <= 3  && _unit->getOwner() != targetUnit->getOwner());
}

void BActionBurn::doAction(const ActionState& statePoint) {
    UnitModel* target = ModelManager::instance()->getUnitAtPos(statePoint.pos);
    if (target != 0) {
        target->defend(_unit, 4, 4, ATTACK_TYPE_FIRE);    
    }
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

