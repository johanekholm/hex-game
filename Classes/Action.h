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
#include "MenuView.h"

namespace ActionNS {
    const int BACTION_MOVE =    0;
    const int BACTION_STRIKE =  1;
    const int BACTION_FIRE =    2;
    const int BACTION_GALE =    3;
    const int BACTION_BURN =    4;
    const int BACTION_HEAL =    5;
    
    const int AACTION_MOVE =     0;
    const int AACTION_FIGHT =    1;
    const int AACTION_SHOP =     2;
    
    const int TARGET_HEX =      1;
    const int TARGET_UNIT =     2;
    const int TARGET_PARTY =    3;
    const int TARGET_SELF =     4;
    
    const int TYPE_NEUTRAL     = 0;
    const int TYPE_ATTACK      = 1;
    const int TYPE_MOVEMENT    = 2;
    const int TYPE_DEFENSE     = 3;
    const int TYPE_BOOST       = 4;
    
};




class UnitModel;
struct HexState;

class PartyModel;
class MapObject;

struct ActionState {
    MPoint pos;
    int actionId;
    bool active;
    int cost;
    int actionType;
    int targetType;
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
	MapObject* _object;
    std::string _name;

    virtual bool isAvailable();
    virtual bool isAvailableAtHex(const MPoint& hex);
    virtual bool isAvailableToObject(MapObject* targetObject);

public:
    static AdventureAction* build(int anId, MapObject* object);
    AdventureAction(const std::string& name, int anId, MapObject* object, int cost, int targetType, int actionType);
	virtual void doIt(const ActionState& statePoint) = 0;
    int getCost();
    std::vector<ActionState> getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<MapObject*>& parties);
};

/*---------------------------------------------------------------*/

class AdvActionMove : public AdventureAction {
protected:
    virtual bool isAvailableAtHex(const MPoint& hex);
    
public:
    AdvActionMove(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
};

/*---------------------------------------------------------------*/

class AActionFight : public AdventureAction {
protected:
    virtual bool isAvailableAtHex(const MPoint& hex);
    
public:
    AActionFight(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
};

/*---------------------------------------------------------------*/

class AActionShop : public AdventureAction, public IChoiceCallback {
protected:
    virtual bool isAvailable();
    
public:
    AActionShop(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
    void reportChoice(int choiceId);
};

#endif