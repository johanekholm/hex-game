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

#define ADV_ACTION_MOVE -1
#define ADV_ACTION_BATTLE -2

#define TARGET_HEX 1
#define TARGET_UNIT 2
#define TARGET_PARTY 3
#define TARGET_SELF 4



#define ACTION_TYPE_ATTACK      1
#define ACTION_TYPE_MOVEMENT    2
#define ACTION_TYPE_DEFENSE     3
#define ACTION_TYPE_BOOST       4



class UnitModel;
struct HexState;

class PartyModel;

struct ActionState {
    MPoint pos;
    int actionId;
    bool active;
    int cost;
    int actionType;
};

class BattleAction {
protected:
	int _id;
    int _cost;
    int _targetType;
    int _type;
	UnitModel* _unit;
    std::string _name;
    std::string _sound;

    virtual bool isAvailableAtHex(const MPoint& hex);
    virtual bool isAvailableToUnit(UnitModel* targetUnit);
    virtual void doAction(const ActionState& statePoint) = 0;

public:
    static BattleAction* build(int actionId, UnitModel* unit);
    BattleAction(int anId, UnitModel* unit, const std::string& name, int cost, int targetType, int actionType, const std::string& sound);
    void doIt(const ActionState& statePoint);
    int getCost();
    std::vector<ActionState> getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<UnitModel*>& units);
};

/*---------------------------------------------------------------*/

class BActionMove : public BattleAction {
protected:
    virtual bool isAvailableAtHex(const MPoint& hex);
    virtual void doAction(const ActionState& statePoint);
public:
    BActionMove(int anId, UnitModel* unit);
};

/*---------------------------------------------------------------*/

class BActionStrike : public BattleAction {
protected:
    virtual bool isAvailableToUnit(UnitModel* targetUnit);
    virtual void doAction(const ActionState& statePoint);
public:
    BActionStrike(int anId, UnitModel* unit);
};

/*---------------------------------------------------------------*/

class BActionFire : public BattleAction {
protected:
    virtual bool isAvailableToUnit(UnitModel* targetUnit);
    virtual void doAction(const ActionState& statePoint);
public:
    BActionFire(int anId, UnitModel* unit);
};

/*---------------------------------------------------------------*/

class BActionHeal : public BattleAction {
protected:
    virtual bool isAvailableToUnit(UnitModel* targetUnit);
    virtual void doAction(const ActionState& statePoint);
public:
    BActionHeal(int anId, UnitModel* unit);
};

/*---------------------------------------------------------------*/

class BActionBurn : public BattleAction {
protected:
    virtual bool isAvailableToUnit(UnitModel* targetUnit);
    virtual void doAction(const ActionState& statePoint);
public:
    BActionBurn(int anId, UnitModel* unit);
};

/*---------------------------------------------------------------*/

class AdventureAction {
protected:
	int _id;
    int _cost;
    int _targetType;
    int _type;
	PartyModel* _party;
    std::string _name;

    virtual bool isAvailableAtHex(const MPoint& hex);
    virtual bool isAvailableToParty(PartyModel* targetParty);

public:
    static AdventureAction* build(int anId, PartyModel* party);
    AdventureAction(const std::string& name, int anId, PartyModel* party, int cost, int targetType, int actionType);
	virtual void doIt(const ActionState& statePoint) = 0;
    int getCost();
    std::vector<ActionState> getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<PartyModel*>& parties);
};

/*---------------------------------------------------------------*/

class AdvActionMove : public AdventureAction {
protected:
    virtual bool isAvailableAtHex(const MPoint& hex);
    
public:
    AdvActionMove(int anId, PartyModel* party);
	virtual void doIt(const ActionState& statePoint);
};

/*---------------------------------------------------------------*/

#endif