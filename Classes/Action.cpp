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
#include "ObjectBuilder.h"
#include "SceneContext.h"
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
		if (ModelManager::instance()->getMap()->getHexValue(hex) != 6) {
			return (ModelManager::instance()->getUnitAtPos(hex) == 0);			
		}
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
		case AACTION_CITY:
			return new AActionCity(actionId, object);
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
	if (_object->getOwner() == FactionNS::PLAYER) {
		CentralControl::instance()->nextTurn();
		ViewControllerManager::instance()->centerCamera(_object->getPosition());
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
    //SceneLoader::instance()->loadBattleScene("battleMap1.txt", *_object, *target);
	SceneLoader::instance()->loadBattleScene("battleMap1.txt", _object, target);
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

    SceneLoader::instance()->returnFromMenu();
    CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
	std::vector<UnitModel*> units;

	switch (num) {
		case ItemNS::POTION:

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
            _object->addItem(new Item(num, 1));
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


CallbackActionUseItemOnMap::CallbackActionUseItemOnMap(MapObject* object, int item) {
	_object = object;
	_item = item;
}

void CallbackActionUseItemOnMap::callbackNumber(int num) {
	std::vector<UnitModel*> units = _object->getMembers();
	UnitModel* unit = 0;
	
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
				break;
				
			default:
				break;
		}
	}
}

/*---------------------------------------------------------------*/


AActionEnterDungeon::AActionEnterDungeon(int anId, MapObject* object) : AdventureAction("ENTERDUNGEON", anId, object, 0, TARGET_SELF, 0) { }

bool AActionEnterDungeon::isAvailable() {
    return (ModelManager::instance()->mapObjectExistAtPos(MapObjectCategory::DUNGEON, _object->getPosition()));
}

void AActionEnterDungeon::doIt(const ActionState& statePoint) {
	_sceneId = ModelManager::instance()->getMapObjectAtPos(_object->getPosition(), MapObjectCategory::DUNGEON)->getText("sceneId");
    SceneLoader::instance()->switchToTransition(new FadeOutTransition(*this, true));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void AActionEnterDungeon::callbackVoid() {
	SceneLoader::instance()->loadDungeonScene(_sceneId, _object);
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
	MenuAction* equip;
	int counter = 1;

    units = _object->getMembers();

	unitNodes.push_back(new BackButtonMenuNodeVC(0, "BACK", GPointMake(0.0f, 0.0f), 120.0f, 32.0f));

	for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
		equip = new MenuActionEquip(_object, *it);
		actionNodes.clear();
		actionNodes.push_back(new BackButtonMenuNodeVC(0, "BACK", GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
		actionNodes.push_back(new ActionMenuNodeVC(equip, 0, "EQUIP", empty, GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
		actionNodes.push_back(new UnitInfoMenuNodeVC(0, *it, GPointMake(0.0f, 0.0f)));
		
		unitNodes.push_back(new ParentMenuNodeVC(0, (*it)->getDescription(), actionNodes, GPointMake(0.0f, 0.0f), 120.0f, 32.0f, true));
		counter++;
	}
	
	
    rootNode = new ParentMenuNodeVC(0, "ROOT", unitNodes, GPointMake(0.0f, 0.0f), 80.0f, 32.0f, true);
    
    SceneLoader::instance()->switchToMenu(new MenuViewController(rootNode));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

/*---------------------------------------------------------------*/

AActionCity::AActionCity(int anId, MapObject* object) : AdventureAction("CITY", anId, object, 0, TARGET_SELF, 0) { }

bool AActionCity::isAvailable() {
    return (ModelManager::instance()->mapObjectExistAtPos(MapObjectCategory::CITY, _object->getPosition()));
}

void AActionCity::doIt(const ActionState& statePoint) {
	BaseMenuNodeVC* rootNode;
	std::vector<BaseMenuNodeVC*> actionNodes, empty;
	MenuAction* shopAction = 0;
	MenuAction* recruitAction = 0;
	MenuAction* chatAction = 0;
	MapObject* city = ModelManager::instance()->getMapObjectAtPos(_object->getPosition(), MapObjectCategory::CITY);
	
	recruitAction = new MenuActionRecruit(_object);
	shopAction = new MenuActionShop(_object, city);
	chatAction = new MenuActionChat(_object, city);
	
	actionNodes.clear();
	actionNodes.push_back(new BackButtonMenuNodeVC(0, "BACK", GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
	actionNodes.push_back(new ActionMenuNodeVC(recruitAction, 0, "RECRUIT", empty, GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
	actionNodes.push_back(new ActionMenuNodeVC(shopAction, 0, "SHOP", empty, GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
	actionNodes.push_back(new ActionMenuNodeVC(chatAction, 0, "CHAT", empty, GPointMake(0.0f, 0.0f), 120.0f, 32.0f));
		
	rootNode = new ParentMenuNodeVC(0, "ROOT", actionNodes, GPointMake(0.0f, 0.0f), 120.0f, 32.0f, true);
	    
    SceneLoader::instance()->switchToMenu(new MenuViewController(rootNode));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

/*---------------------------------------------------------------*/

MenuActionEquip::MenuActionEquip(MapObject* object, UnitModel* unit) {
	_object = object;
	_unit = unit;
	_slot = 0;
	_item = 0;
}

void MenuActionEquip::doIt() {
	
	if (_object->removeItem(_item, 1)) {
		// add new item to equipment and move old equipment to inventory
		_object->addItem(_unit->replaceEquipment(new Item(_item, 1), _slot));
	}

	SceneLoader::instance()->returnFromMenu();
}

void MenuActionEquip::reportChoice(int choiceId) {
	if (_slot == 0) {
		_slot = choiceId;		
	} else {
		_item = choiceId;
	}
}

bool MenuActionEquip::isInputRequired() {
	return (_slot == 0 || _item == 0);
}

std::vector<MenuChoice> MenuActionEquip::getChoices() {
	MenuChoice node;
    std::vector<MenuChoice> choices;
	std::map<int, Item*> items;
	Item* item;
	std::string itemDescription;

    if (_slot == 0) {
		item = _unit->getItemInSlot(1); node.choiceId = 1; 
		node.label.assign("RIGHT HAND: ").append((item == 0) ? "EMPTY":item->getDescription()); choices.push_back(node);
		
		item = _unit->getItemInSlot(2); node.choiceId = 2; 
		node.label.assign("LEFT HAND: ").append((item == 0) ? "EMPTY":item->getDescription()); choices.push_back(node);
		
		item = _unit->getItemInSlot(3); node.choiceId = 3; 
		node.label.assign("HEAD: ").append((item == 0) ? "EMPTY":item->getDescription()); choices.push_back(node);
		
		item = _unit->getItemInSlot(4); node.choiceId = 4; 
		node.label.assign("BODY: ").append((item == 0) ? "EMPTY":item->getDescription()); choices.push_back(node);
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

void MenuActionEquip::reset() {
	_slot = 0;
	_item = 0;
}




/*---------------------------------------------------------------*/

MenuActionRecruit::MenuActionRecruit(MapObject* object) {
	_object = object;
	_unit = 0;
}

void MenuActionRecruit::doIt() {
	std::string unitClass;
	int cost = 0;
	
	switch (_unit) {
		case 1:
			unitClass = "archer"; cost = 8;
			break;
		case 2:
			unitClass = "soldier"; cost = 8;
			break;
		case 3:
			unitClass = "channeler"; cost = 10;
			break;
			
		default:
			break;
	}

	if (_object->removeItem(ItemNS::SILVER, cost)) {
		_object->addMember(ObjectBuilder::produceUnit(unitClass, _object->getOwner(), MPointMake(1,1)));
	}
	
	SceneLoader::instance()->returnFromMenu();
}

void MenuActionRecruit::reportChoice(int choiceId) {
	if (_unit == 0) {
		_unit = choiceId;		
	}
}

bool MenuActionRecruit::isInputRequired() {
	return (_unit == 0);
}

std::vector<MenuChoice> MenuActionRecruit::getChoices() {
	MenuChoice node;
    std::vector<MenuChoice> choices;
    
    node.choiceId = 1; node.label = "ARCHER 8S";	choices.push_back(node);
    node.choiceId = 2; node.label = "SOLDIER 8S";	choices.push_back(node);
    node.choiceId = 3; node.label = "MAGE 10S";		choices.push_back(node);

	return choices;		
}

void MenuActionRecruit::reset() {
	_unit = 0;
}


/*---------------------------------------------------------------*/

MenuActionShop::MenuActionShop(MapObject* object, MapObject* shop) {
	_object = object;
	_shop = shop;
	_item = 0;
}

void MenuActionShop::doIt() {
	std::string unitClass;
	ItemTemplate* itemTemplate = ItemTemplate::getTemplate(_item);
	
	if (itemTemplate != 0 && _object->removeItem(ItemNS::SILVER, itemTemplate->getCost())) {
		_shop->addItem(new Item(ItemNS::SILVER, itemTemplate->getCost()));
		_shop->removeItem(_item, 1);
		_object->addItem(new Item(_item, 1));
	}
	
	SceneLoader::instance()->returnFromMenu();
}

void MenuActionShop::reportChoice(int choiceId) {
	if (_item == 0) {
		_item = choiceId;		
	}
}

bool MenuActionShop::isInputRequired() {
	return (_item == 0);
}

std::vector<MenuChoice> MenuActionShop::getChoices() {
	MenuChoice node;
    std::vector<MenuChoice> choices;
    std::map<int, Item*> items;
    
	if (_shop != 0) {
		items = _shop->getItems();
		
		for (std::map<int, Item*>::iterator it = items.begin(); it != items.end(); ++it) {
			node.choiceId = it->second->getType(); 
			node.label = it->second->getDescription(true);
			choices.push_back(node);
		}		
	}
		
	return choices;		
}

void MenuActionShop::reset() {
	_item = 0;
}

/*---------------------------------------------------------------*/

MenuActionChat::MenuActionChat(MapObject* object, MapObject* village) {
	_object = object;
	_village = village;
	_speaker = 0;
}

void MenuActionChat::doIt() {
	//std::string dialogue;
	std::stringstream dialogue; 
	int id = 0;
	std::map<std::string,std::string> texts = _village->getTexts();

	for (std::map<std::string, std::string>::iterator it = texts.begin(); it != texts.end(); ++it) {
		if (++id == _speaker) {
			dialogue << it->first << ": " << it->second;
			break;
		}
	}		
	
	SceneLoader::instance()->returnFromMenu();
	SceneLoader::instance()->switchToMenu(new TextboxMenuVC(*this, dialogue.str(), "OK"));
}

void MenuActionChat::reportChoice(int choiceId) {
	_speaker = choiceId;		
}

bool MenuActionChat::isInputRequired() {
	return (_speaker == 0);
}

std::vector<MenuChoice> MenuActionChat::getChoices() {
	MenuChoice node;
    std::vector<MenuChoice> choices;
    
	std::map<std::string,std::string> texts = _village->getTexts();
	int id = 0;
	
	for (std::map<std::string, std::string>::iterator it = texts.begin(); it != texts.end(); ++it) {
		node.choiceId = ++id;
		node.label = it->first;
		choices.push_back(node);
	}		
	
	return choices;		
}

void MenuActionChat::reset() {
	_speaker = 0;
}

void MenuActionChat::callbackVoid() {
	SceneLoader::instance()->returnFromMenu();
}


/*---------------------------------------------------------------*/

GiveItemBean::~GiveItemBean() {
	_items.clear();
}

GiveItemBean::GiveItemBean(Item* item, MapObject* object) {
	_items.push_back(item);
	_object = object;
}

GiveItemBean::GiveItemBean(const std::vector<Item*>& items, MapObject* object) {
	_items = items;
	_object = object;
}

void GiveItemBean::start() {
	std::string text = "YOU GOT ";
	for (std::vector<Item*>::iterator it = _items.begin(); it != _items.end(); it++) {
		_object->addItem(*it);
		text += (*it)->getDescription();
		text += ", ";
	}
	SceneLoader::instance()->switchToMenu(new TextboxMenuVC(*this, text, "OK"));
	_items.clear();
}

void GiveItemBean::callbackVoid() {
	SceneLoader::instance()->returnFromMenu();
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

void FadeInBean::start() {
	SceneLoader::instance()->switchToTransition(new FadeOutTransition(*this, false));
}

void FadeInBean::callbackVoid() {
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

void FadeOutBean::start() {
	SceneLoader::instance()->switchToTransition(new FadeOutTransition(*this, true));
}

void FadeOutBean::callbackVoid() {
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

void ReturnToAdventureSceneBean::start() {
	SceneLoader::instance()->returnToAdventureScene();
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

DungeonChoiceBean::DungeonChoiceBean(const std::string& sceneId) : ControlBean() {
	_sceneId = sceneId;
}

void DungeonChoiceBean::start() {
	std::vector<MenuChoice> choices;
	choices.push_back(MenuChoice::makeChoice(2, "EXIT DUNGEON"));
	choices.push_back(MenuChoice::makeChoice(1, "CONTINUE"));
    
    SceneLoader::instance()->switchToMenu(new ChoiceMenuVC(*this, choices, false));
}

void DungeonChoiceBean::callbackNumber(int num) {
	ControlBeanDirector* director = ControlBeanDirector::instance();
	SceneLoader::instance()->returnFromMenu();

	switch (num) {
		case 1:
			director->addBean(new FadeOutBean());
			director->addBean(new NextDungeonSceneBean(_sceneId));
			director->addBean(new FadeInBean());
			break;
			
		case 2:
			director->addBean(new FadeOutBean());
			director->addBean(new ReturnToAdventureSceneBean());
			director->addBean(new FadeInBean());
			break;
			
		default:
			break;
	}

	_director->beanDidFinish(this);	
}

/*---------------------------------------------------------------*/

NextDungeonSceneBean::NextDungeonSceneBean(const std::string& sceneId) : ControlBean() {
	_sceneId = sceneId;
}

void NextDungeonSceneBean::start() {
	SceneLoader::instance()->loadNextDungeonScene(_sceneId, 0);
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

void GetLootBean::start() {
	ItemHandler* stack = SceneContext::instance()->getItemStack();
	MapObject* player = ModelManager::instance()->getFirstMapObjectWithOwner(FactionNS::PLAYER);
	_director->addBean(new GiveItemBean(stack->getItemsAsVector(), player));
	stack->removeAllItems();
	_director->beanDidFinish(this);
}

/*---------------------------------------------------------------*/

