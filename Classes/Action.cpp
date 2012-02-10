/*
 *  Action.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Action.h"
#include "CentralControl.h"
#include "UnitModel.h"
#include "geometry.h"
#include "Item.h"
#include "ModelManager.h"
#include "HexMapModel.h"
#include "MenuView.h"
#include "MessageView.h"
#include "MapObject.h"
#include "SceneLoader.h"
#include "Sound.h"
#include "TransitionViewController.h"
#include "ViewControllerManager.h"
#include <iostream>
#include <string>

using namespace ActionNS;


BattleAction* BattleAction::build(int actionId, UnitModel* unit) {
    switch (actionId) {
		case BACTION_MOVE:
            return new BActionMove(actionId, unit);
		case BACTION_STRIKE:
            return new BActionStrike(actionId, unit);
		case BACTION_FIRE:
            return new BActionFire(actionId, unit);
        case BACTION_BURN:
            return new BActionBurn(actionId, unit);
        case BACTION_GALE:
            return 0;
        case BACTION_HEAL:
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
            }
        }
    } else if (_targetType == TARGET_UNIT) {
        for (std::vector<UnitModel*>::const_iterator it = units.begin(); it != units.end(); ++it) {
            if (this->isAvailableToUnit(*it)) {
                state.pos = (*it)->getPosition();
                actionPoints.push_back(state);
            }
        }
    }
    return actionPoints;
}

/*---------------------------------------------------------------*/


BActionMove::BActionMove(int anId, UnitModel* unit) : BattleAction(anId, unit, "MOVE", 10, TARGET_HEX, TYPE_MOVEMENT, "") {}

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

BActionStrike::BActionStrike(int anId, UnitModel* unit) : BattleAction(anId, unit, "STRIKE", 20, TARGET_UNIT, TYPE_ATTACK, "strike") {}

bool BActionStrike::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance == 1 && _unit->getOwner() != targetUnit->getOwner());
}

void BActionStrike::doAction(const ActionState& statePoint) {
    _unit->strike(statePoint.pos);
}

/*---------------------------------------------------------------*/

BActionFire::BActionFire(int anId, UnitModel* unit) : BattleAction(anId, unit, "FIRE", 20, TARGET_UNIT, TYPE_ATTACK, "") {}

bool BActionFire::isAvailableToUnit(UnitModel* targetUnit) {
    int distance = hexDistance(_unit->getPosition(), targetUnit->getPosition());
    
    return (distance > 0 && distance <= 2  && _unit->getOwner() != targetUnit->getOwner());
}

void BActionFire::doAction(const ActionState& statePoint) {
    _unit->fire(statePoint.pos);
}

/*---------------------------------------------------------------*/

BActionHeal::BActionHeal(int anId, UnitModel* unit) : BattleAction(anId, unit, "HEAL", 20, TARGET_UNIT, TYPE_DEFENSE, "heal") {}

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

BActionBurn::BActionBurn(int anId, UnitModel* unit) : BattleAction(anId, unit, "BURN", 40, TARGET_UNIT, TYPE_ATTACK, "fire") {}

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

AdventureAction* AdventureAction::build(int actionId, MapObject* object) {
    switch (actionId) {
        case AACTION_MOVE:
            return new AdvActionMove(actionId, object);
        case AACTION_FIGHT:
            return new AActionFight(actionId, object);
        case AACTION_SHOP:
            return new AActionShop(actionId, object);
        case AACTION_ENTERDUNGEON:
            return new AActionEnterDungeon(actionId, object);
        case AACTION_INVENTORY:
            return new AActionInventory(actionId, object);
        case AACTION_PARTYOPTIONS:
            return new AActionPartyOptions(actionId, object);
		default:
			return 0;
	}
}

AdventureAction::AdventureAction(const std::string& name, int anId, MapObject* object, int cost, int targetType, int actionType) {
	_id = anId;	
	_object = object;
    _cost = cost;
    _targetType = targetType;
    _type = actionType;
    _name = name;
}

int AdventureAction::getCost() {
    return _cost;
}

std::vector<ActionState> AdventureAction::getActionPoints(int ap, const std::map<int, HexState>& hexes, const std::vector<MapObject*>& parties) {
    ActionState state;
    std::vector<ActionState> actionPoints;
    
    state.actionId = _id;
    state.cost = _cost;
    state.actionType = _type;
    state.active = true;
    state.targetType = _targetType;
    
    if (_targetType == TARGET_HEX) {
        for (std::map<int, HexState>::const_iterator it = hexes.begin(); it != hexes.end(); ++it) {
            if (this->isAvailableAtHex((it->second).pos)) {
                state.pos = (it->second).pos;
                actionPoints.push_back(state);
            }
        }
    } else if (_targetType == TARGET_PARTY) {
        for (std::vector<MapObject*>::const_iterator it = parties.begin(); it != parties.end(); ++it) {
            if (this->isAvailableToObject(*it)) {
                state.pos = (*it)->getPosition();
                actionPoints.push_back(state);
            }
        }
    } else if (_targetType == TARGET_SELF) {
        if (this->isAvailable()) {
            state.pos = _object->getPosition();
            actionPoints.push_back(state);
        }
    }
    return actionPoints;
}

bool AdventureAction::isAvailable() {
    return false;
}

bool AdventureAction::isAvailableAtHex(const MPoint& hex) {
    return false;
}

bool AdventureAction::isAvailableToObject(MapObject* targetObject) {
    return false;
}

/*---------------------------------------------------------------*/

AdvActionMove::AdvActionMove(int anId, MapObject* object) : AdventureAction("MOVE", anId, object, 0, TARGET_HEX, TYPE_MOVEMENT) { }

bool AdvActionMove::isAvailableAtHex(const MPoint& hex) {
    int distance = hexDistance(_object->getPosition(), hex);
    return (distance == 1 && _object->canMoveTo(hex) && !ModelManager::instance()->mapObjectExistAtPos(MapObjectCategory::PARTY, hex));
}

void AdvActionMove::doIt(const ActionState& statePoint) {
    _object->move(statePoint.pos);
	
	// advance to next turn if player party moves
	if (_object->getOwner() == 1) {
		CentralControl::instance()->nextTurn();
		//ViewControllerManager::instance()->centerCamera(_object->getPosition());
	}
}

/*---------------------------------------------------------------*/

AActionFight::AActionFight(int anId, MapObject* object) : AdventureAction("FIGHT", anId, object, 0, TARGET_HEX, TYPE_ATTACK) { }

bool AActionFight::isAvailableAtHex(const MPoint& hex) {
    int distance = hexDistance(_object->getPosition(), hex);
    MapObject* target = ModelManager::instance()->getMapObjectAtPos(hex);
    return (distance == 1 && target != 0 && target->matchesCategory(MapObjectCategory::PARTY));
}

void AActionFight::doIt(const ActionState& statePoint) {
    MapObject* target = ModelManager::instance()->getMapObjectAtPos(statePoint.pos);
    _object->move(statePoint.pos);
    //SceneLoader::instance()->loadBattleScene("battleMap1.jsn", *_object, *target);
	SceneLoader::instance()->loadBattleScene("battleMap1.jsn", _object, target);
}

/*---------------------------------------------------------------*/

AActionInventory::AActionInventory(int anId, MapObject* object) : AdventureAction("INVENTORY", anId, object, 0, TARGET_SELF, 0) { }

bool AActionInventory::isAvailable() {
    return true;
}

void AActionInventory::doIt(const ActionState& statePoint) {
    MenuChoice menuItem;
    std::vector<MenuChoice> choices;
    std::map<int, Item*> items;
    
    items = _object->getItems();
    
	for (std::map<int, Item*>::iterator it = items.begin(); it != items.end(); ++it) {
        menuItem.choiceId = it->second->getType();
        menuItem.label = it->second->getDescription();
        choices.push_back(menuItem);
	}
    
    SceneLoader::instance()->switchToMenu(new ChoiceMenuVC(*this, choices));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void AActionInventory::callbackNumber(int num) {
    MenuChoice menuItem;
    std::vector<MenuChoice> choices;
    DEBUGLOG("Chose item %i", num);
    SceneLoader::instance()->returnFromMenu();
    CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
	std::vector<UnitModel*> units;

	switch (num) {
		case ItemNS::POTION:
			DEBUGLOG("Do potion");
			units = _object->getMembers();
			
			for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
				menuItem.choiceId = (*it)->getId();
				menuItem.label = "UNIT";
				choices.push_back(menuItem);
			}

			SceneLoader::instance()->switchToMenu(new ChoiceMenuVC(*(new CallbackActionUseItemOnMap(_object, num)), choices));
			CentralControl::instance()->switchMode(ControlMode::MENU);
			
			break;
			
		default:
			break;
	}
}

/*---------------------------------------------------------------*/

AActionShop::AActionShop(int anId, MapObject* object) : AdventureAction("SHOP", anId, object, 0, TARGET_SELF, 0) { }

bool AActionShop::isAvailable() {
    return (ModelManager::instance()->mapObjectExistAtPos(MapObjectCategory::CITY, _object->getPosition()));
}

void AActionShop::doIt(const ActionState& statePoint) {
    MenuChoice item;
    std::vector<MenuChoice> choices;
    
    item.choiceId = ItemNS::SWORD; item.label = "SWORD 10S";
    choices.push_back(item);
    item.choiceId = ItemNS::SHIELD; item.label = "SHIELD 7S";
    choices.push_back(item);
    item.choiceId = ItemNS::POTION; item.label = "POTION 6S";
    choices.push_back(item);
    item.choiceId = ItemNS::RING; item.label = "RING 8S";
    choices.push_back(item);
    
    SceneLoader::instance()->switchToMenu(new ChoiceMenuVC(*this, choices));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void AActionShop::callbackNumber(int num) {
    
    if (num != -1) {
        if (_object->removeItem(ItemNS::SILVER, 6)) {
            _object->addItem(Item::buildItem(num, 1));
            SceneLoader::instance()->returnFromMenu();
            CentralControl::instance()->switchMode(ControlMode::ADVENTURE);                
        } else {
            DEBUGLOG("Not enough silver");
        }
    } else {
        SceneLoader::instance()->returnFromMenu();
        CentralControl::instance()->switchMode(ControlMode::ADVENTURE);                
    }
}

/*---------------------------------------------------------------*/


AActionEnterDungeon::AActionEnterDungeon(int anId, MapObject* object) : AdventureAction("ENTERDUNGEON", anId, object, 0, TARGET_SELF, 0) { }

bool AActionEnterDungeon::isAvailable() {
    return (ModelManager::instance()->mapObjectExistAtPos(MapObjectCategory::DUNGEON, _object->getPosition()));
}

void AActionEnterDungeon::doIt(const ActionState& statePoint) {
    SceneLoader::instance()->switchToTransition(new FadeOutTransition(*this));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void AActionEnterDungeon::callbackVoid() {
	SceneLoader::instance()->loadDungeonScene("dungeon1.jsn", _object);
}

/*---------------------------------------------------------------*/

AActionPartyOptions::AActionPartyOptions(int anId, MapObject* object) : AdventureAction("PARTY", anId, object, 0, TARGET_SELF, 0) { }

bool AActionPartyOptions::isAvailable() {
    return true;
}

void AActionPartyOptions::doIt(const ActionState& statePoint) {
	BaseMenuNodeVC* rootNode;
	std::vector<BaseMenuNodeVC*> unitNodes, actionNodes, empty;
	std::vector<UnitModel*> units;
	MenuActionCallback* equip;
	int counter = 1;

    units = _object->getMembers();
	
	for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
		equip = new CallbackActionEquip(_object, *it);
		actionNodes.clear();
		actionNodes.push_back(new ActionMenuNodeVC(equip, 0, "EQUIP", empty, GPointMake(160.0f, 360.0f), 120.0f, 32.0f));
		actionNodes.push_back(new BackButtonMenuNodeVC(0, "BACK", GPointMake(160.0f, 400.0f), 120.0f, 32.0f));
		
		unitNodes.push_back(new ParentMenuNodeVC(0, (*it)->getDescription(), actionNodes, GPointMake(160.0f, 400.0f - counter * 40.0f), 120.0f, 32.0f));
		counter++;
	}
	
	unitNodes.push_back(new BackButtonMenuNodeVC(0, "BACK", GPointMake(160.0f, 400.0f), 120.0f, 32.0f));
	
    rootNode = new ParentMenuNodeVC(0, "ROOT", unitNodes, GPointMake(0.0f, 0.0f), 80.0f, 32.0f);
	DEBUGLOG("Party options");
    
    SceneLoader::instance()->switchToMenu(new MenuViewController(rootNode));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}


/*---------------------------------------------------------------*/


CallbackActionUseItemOnMap::CallbackActionUseItemOnMap(MapObject* object, int item) {
	_object = object;
	_item = item;
}

void CallbackActionUseItemOnMap::callbackNumber(int num) {
	std::vector<UnitModel*> units = _object->getMembers();
	UnitModel* unit = 0;
	DEBUGLOG("Callback do item");

	SceneLoader::instance()->returnFromMenu();
    CentralControl::instance()->switchMode(ControlMode::ADVENTURE);

	for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); it++) {
		if ((*it)->getId() == num) {
			unit = *it;
			break;
		}
	}
	
	if (unit != 0) {
		switch (_item) {
			case ItemNS::POTION:
				unit->inflictDamage(-3);
				_object->removeItem(ItemNS::POTION, 1);
				DEBUGLOG("Healing potion");
				break;
				
			default:
				break;
		}
	}
}

/*---------------------------------------------------------------*/

CallbackActionEquip::CallbackActionEquip(MapObject* object, UnitModel* unit) {
	_object = object;
	_unit = unit;
	_slot = 0;
	_item = 0;
}

void CallbackActionEquip::callbackVoid() {
	DEBUGLOG("Equip that");
	
	if (_object->removeItem(_item, 1)) {
		_object->addItem(_unit->replaceEquipment(Item::buildItem(_item, 1), _slot));
	}
	
	SceneLoader::instance()->returnFromMenu();
}

void CallbackActionEquip::callbackNumber(int num) {
	if (_slot == 0) {
		_slot = num;		
	} else {
		_item = num;
	}
	DEBUGLOG("Slot is: %i, Item is: %i", _slot, _item);
}

bool CallbackActionEquip::isInputRequired() {
	return (_slot == 0 || _item == 0);
}

std::vector<MenuChoice> CallbackActionEquip::getChoices() {
	MenuChoice node;
    std::vector<MenuChoice> choices;
	std::map<int, Item*> items;

    if (_slot == 0) {
		node.choiceId = 1; node.label = "RIGHT HAND"; choices.push_back(node);
		node.choiceId = 2; node.label = "LEFT HAND"; choices.push_back(node);
		node.choiceId = 3; node.label = "HEAD"; choices.push_back(node);
		node.choiceId = 4; node.label = "BODY"; choices.push_back(node);
	} else {		
		items = _object->getItems();
		
		for (std::map<int, Item*>::iterator it = items.begin(); it != items.end(); ++it) {
			node.choiceId = it->second->getType();
			node.label = it->second->getDescription();
			choices.push_back(node);
		}
	}
	return choices;		
}

void CallbackActionEquip::reset() {
	_slot = 0;
	_item = 0;
}

