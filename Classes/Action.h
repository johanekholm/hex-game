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
#include "ControlCallback.h"
#include "ControlBeanDirector.h"
#include "Item.h"

namespace ActionNS {
    const int BACTION_MOVE =    0;
    const int BACTION_STRIKE =  1;
    const int BACTION_FIRE =    2;
    const int BACTION_GALE =    3;
    const int BACTION_BURN =    4;
    const int BACTION_HEAL =    5;
    
    const int AACTION_MOVE          = 0;
    const int AACTION_FIGHT         = 1;
    const int AACTION_SHOP          = 6;
    const int AACTION_ENTERDUNGEON  = 7;
    const int AACTION_INVENTORY     = 8;
	const int AACTION_PARTYOPTIONS	= 9;
	const int AACTION_CITY			= 10;
    
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

class AActionInventory : public AdventureAction, public ControlCallback {
protected:
    virtual bool isAvailable();
    
public:
    AActionInventory(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
    void callbackNumber(int num);
};

/*---------------------------------------------------------------*/

class AActionShop : public AdventureAction, public ControlCallback {
protected:
    virtual bool isAvailable();
    
public:
    AActionShop(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
    void callbackNumber(int num);
};

/*---------------------------------------------------------------*/

class AActionEnterDungeon : public AdventureAction, public ControlCallback {
protected:
    virtual bool isAvailable();
    
public:
    AActionEnterDungeon(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
    void callbackVoid();
};

/*---------------------------------------------------------------*/
 
class AActionPartyOptions : public AdventureAction {
protected:
	virtual bool isAvailable();
 
public:
	AActionPartyOptions(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
};


/*---------------------------------------------------------------*/

class AActionCity : public AdventureAction {
protected:
	virtual bool isAvailable();
	
public:
	AActionCity(int anId, MapObject* object);
	virtual void doIt(const ActionState& statePoint);
};


/*---------------------------------------------------------------*/

class CallbackActionUseItemOnMap : public ControlCallback {
protected:
    int _item;
    MapObject* _object;
	
public:
    CallbackActionUseItemOnMap(MapObject* object, int item);
    void callbackNumber(int num);
};

/*---------------------------------------------------------------*/

class CallbackActionEquip : public MenuActionCallback {
protected:
    MapObject* _object;
	UnitModel* _unit;
    int _item;
	int _slot;
	
public:
    CallbackActionEquip(MapObject* object, UnitModel* unit);
	void callbackVoid();
    void callbackNumber(int num);
	bool isInputRequired();
	std::vector<MenuChoice> getChoices();
	void reset();
};

/*---------------------------------------------------------------*/

class CallbackActionRecruit : public MenuActionCallback {
protected:
    MapObject* _object;
    int _unit;
	
public:
    CallbackActionRecruit(MapObject* object);
	void callbackVoid();
    void callbackNumber(int num);
	bool isInputRequired();
	std::vector<MenuChoice> getChoices();
	void reset();
};


/*---------------------------------------------------------------*/

class CallbackActionShop : public MenuActionCallback {
protected:
    MapObject* _object;
    MapObject* _shop;
    int _item;
	
public:
    CallbackActionShop(MapObject* object, MapObject* shop);
	void callbackVoid();
    void callbackNumber(int num);
	bool isInputRequired();
	std::vector<MenuChoice> getChoices();
	void reset();
};


/*---------------------------------------------------------------*/

class GiveItemBean : public ControlBean, public ControlCallback {
protected:
	Item* _item;
	MapObject* _object;
	
public:
	~GiveItemBean();
	GiveItemBean(Item* item, MapObject* object);
	void start();
	void callbackVoid();
};

/*---------------------------------------------------------------*/

class FadeInBean : public ControlBean, public ControlCallback {
public:
	void start();
	void callbackVoid();
};

/*---------------------------------------------------------------*/

class FadeOutBean : public ControlBean, public ControlCallback {
public:
	void start();
	void callbackVoid();
};

/*---------------------------------------------------------------*/

class ReturnToAdventureSceneBean : public ControlBean {
public:
	void start();
};

/*---------------------------------------------------------------*/

class DungeonChoiceBean : public ControlBean, public ControlCallback {
protected:
	std::string _sceneId;
public:
	DungeonChoiceBean(const std::string& sceneId);
	void start();
	void callbackNumber(int num);
};

/*---------------------------------------------------------------*/

class NextDungeonSceneBean : public ControlBean {
protected:
	std::string _sceneId;
public:
	NextDungeonSceneBean(const std::string& sceneId);
	void start();
};

#endif