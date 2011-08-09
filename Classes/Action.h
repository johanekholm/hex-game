/*
 *  Action.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <vector>
#include <map>

#include "toolkit.h"

#define ACTION_MOVE 0
#define ACTION_STRIKE 1
#define ACTION_FIRE 2
#define ACTION_GALE 3
#define ACTION_BURN 4
#define ACTION_HEAL 5

#define TARGET_HEX 1
#define TARGET_UNIT 2

#define ACTION_TYPE_ATTACK      1
#define ACTION_TYPE_MOVEMENT    2
#define ACTION_TYPE_DEFENSE     3
#define ACTION_TYPE_BOOST       4



class UnitModel;
struct HexState;

struct ActionState {
    MPoint pos;
    int actionId;
    bool active;
    int cost;
    int actionType;
};

class Action {
	int _id;
    int _cost;
    int _targetType;
    int _type;
	UnitModel* _unit;
    std::string _name;
	
public:
	Action(int anId, UnitModel* unit);
	void doIt(const ActionState& statePoint);
    int getCost();
    bool isAvailableAtHex(const MPoint& hex);
    bool isAvailableToUnit(UnitModel* targetUnit);
    std::vector<ActionState> getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<UnitModel*>& units);
};

#endif