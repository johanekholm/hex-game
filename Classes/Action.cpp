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
#include <string>
#include "ModelManager.h"
#include "HexMapModel.h"
#include <iostream>

//using namespace std;

Action::Action(int anId, UnitModel* unit) {
	_id = anId;	
	_unit = unit;
    
    switch (_id) {
		case ACTION_MOVE:
            _cost = 2;
            _targetType = TARGET_HEX;
            break;
		case ACTION_ROTATE_LEFT:
            _cost = 1;
            _targetType = 0;
            break;
		case ACTION_ROTATE_RIGHT:
            _cost = 1;
            _targetType = 0;
            break;
		case ACTION_STRIKE:
            _cost = 2;
            _targetType = TARGET_UNIT;
            break;
		case ACTION_FIRE:
            _cost = 2;
            _targetType = TARGET_UNIT;
            break;
			
		default:
			_cost = 0;
	}

}

void Action::doIt(const ActionState& statePoint) {
    std::cout << "Action of id " << _id << std::endl;

    if (_unit->spendAp(this->getCost())) {
        switch (_id) {
            case ACTION_MOVE:
                _unit->move(statePoint.pos);			
                break;
            case ACTION_ROTATE_LEFT:
                _unit->rotate(1);
                break;
            case ACTION_ROTATE_RIGHT:
                _unit->rotate(-1);			
                break;
            case ACTION_STRIKE:
                _unit->strike(statePoint.pos);
                break;
            case ACTION_FIRE:
                _unit->fire(statePoint.pos);
                break;
                
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
            //return (distance == 1 && ModelManager::instance()->getUnitAtPos(hex) == 0);
		case ACTION_ROTATE_LEFT:
            return false;
		case ACTION_ROTATE_RIGHT:
            return false;
		/*case ACTION_STRIKE:
            return (distance == 1 && ModelManager::instance()->getUnitAtPos(hex) != 0);
		case ACTION_FIRE:
            return (distance > 0 && distance <= 2 && ModelManager::instance()->getUnitAtPos(hex) != 0);
		*/	
		default:
			return false;
	}
}

bool Action::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
	switch (_id) {
        case ACTION_STRIKE:
            return (distance == 1);
        case ACTION_FIRE:
            return (distance > 0 && distance <= 2);
		default:
			return false;
	}
}

std::vector<ActionState> Action::getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<UnitModel*>& units) {
    ActionState state;
    std::vector<ActionState> actionPoints;
    
    state.actionId = _id;
    state.cost = _cost;
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
