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
#include "MessageView.h"
#include "HexMapModel.h"
#include "StateManager.h"
#include "geometry.h"
#include "json.h"
#include <cstdlib>
#include <iostream>


std::map<std::string, UnitModelTemplate> UnitModelTemplate::_templates = UnitModelTemplate::initTemplates();

std::map<std::string, UnitModelTemplate> UnitModelTemplate::initTemplates() {
	std::map<std::string, UnitModelTemplate> templates;
	return templates;
}

UnitModelTemplate* UnitModelTemplate::getTemplate(const std::string& templateId) {
	if (_templates.find(templateId) != _templates.end()) {
        return &_templates[templateId];
    } else {
		return 0;
	}
}

void UnitModelTemplate::loadTemplatesFromJson() {
    Json::Value root;
	UnitModelTemplate unitTemplate;
    
	StateManager::loadStateFromFile(root, "UnitModelTemplates.jsn");
	
	for (Json::ValueIterator it = root["templates"].begin(); it != root["templates"].end(); it++) {
		//unitTemplate = new UnitModelTemplate();
		unitTemplate.deserialize(*it);
		_templates[unitTemplate.getTemplateId()] = unitTemplate;
    }
}

UnitModelTemplate::UnitModelTemplate() {
	_templateId = "";
	_name = "";
    _visualType = 0;
    _basePower = 0;
    _baseSkill = 0;
    _baseDefense = 0;
    _maxAp = 0;
    _maxHp = 0;
}

UnitModelTemplate::UnitModelTemplate(const std::string& templateId, const std::string& name, int visualType, int basePower, int baseSkill, int baseDefense, int maxAp, int maxHp, const std::vector<int>& actionIds) {
	_templateId = templateId;
	_name = name;
    _visualType = visualType;
    _basePower = basePower;
    _baseSkill = baseSkill;
    _baseDefense = baseDefense;
    _maxAp = maxAp;
    _maxHp = maxHp;
	_actionIds = actionIds;
}

void UnitModelTemplate::deserialize(Json::Value& root) {
    _templateId = root.get("type", "missing_id").asString();
	_name = root.get("name", "NONAME").asString();
    _basePower = root.get("power", 0).asInt();
    _baseSkill = root.get("skill", 0).asInt();
    _baseDefense = root.get("defense", 0).asInt();
    _maxAp = root.get("maxAp", 0).asInt();
    _maxHp = root.get("maxHp", 0).asInt();
    _visualType = root.get("visualType", 0).asInt();

	_actionIds.clear();
	
    // deserialize actions
    for (Json::ValueIterator it = root["actions"].begin(); it != root["actions"].end(); it++) {
        _actionIds.push_back((*it).asInt());
    }
}


std::vector<int> UnitModelTemplate::getActionIds() {
	return _actionIds;
}

std::string UnitModelTemplate::getName() {
	return _name;
}

int UnitModelTemplate::getStat(int stat) {
	switch (stat) {
		case StatNS::POWER:
			return _basePower;
		case StatNS::SKILL:
			return _baseSkill;
		case StatNS::DEFENSE:
			return _baseDefense;
		case StatNS::MAXHP:
			return _maxHp;
		case StatNS::MAXAP:
			return _maxAp;
		default:
			break;
	}
	return 0;
}

std::string UnitModelTemplate::getTemplateId() {
	return _templateId;
}

int UnitModelTemplate::getVisualType() {
	return _visualType;
}



UnitModel::~UnitModel() {
    this->updateObserversDestroyed();
}

UnitModel::UnitModel() {}

UnitModel::UnitModel(const std::string& templateId, const MPoint& pos, int owner) {
	std::vector<int> actionIds;
	_template = UnitModelTemplate::getTemplate(templateId);
    _owner = owner;
	_pos = pos;
    _hp = _template->getStat(StatNS::MAXHP);
    _ap = rand() % 10;
	
	actionIds = _template->getActionIds();
    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }	
}

Json::Value UnitModel::serialize() {
    Json::Value root;
    
    Json::Value& equip = root["equip"];
	
    root["type"] = _template->getTemplateId();
    root["id"] = _id;
	root["x"] = _pos.x;
    root["y"] = _pos.y;
    root["ap"] = _ap;
    root["hp"] = _hp;
    root["owner"] = _owner;
	
	// serialize items
	equip = this->serializeEquippedItems();

    return root;
}

void UnitModel::deserialize(Json::Value& root) {
	std::vector<int> actionIds;
	std::string templateId = root.get("type", "").asString();
	_template = UnitModelTemplate::getTemplate(templateId);
    _id = root.get("id", 0).asInt();
    _owner = root.get("owner", 0).asInt();
    _pos.x = root.get("x", 0.0f).asFloat();
    _pos.y = root.get("y", 0.0f).asFloat();
    _ap = root.get("ap", 0).asInt();
    _hp = root.get("hp", 0).asInt();

	actionIds = _template->getActionIds();
	for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }

	this->deserializeEquippedItems(root["equip"]);
}


BattleAction* UnitModel::addAction(int action) {
	_actions[action] = BattleAction::build(action, this); //new Action(action, this);
	return _actions[action];
}

void UnitModel::chooseTarget() {
    _target = ModelManager::instance()->getClosestTo(_pos);
}

int UnitModel::chooseMovementTarget(const std::vector<ActionState>& targets) {
    int distance, minDistance;
    int index = 0, bestAction = 0;
    
    for (std::vector<ActionState>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
        distance = ModelManager::instance()->getDistanceToClosestEnemy(2, (*it).pos);
        
        if (distance < minDistance) {
            minDistance = distance;
            bestAction = index;
        }
        index++;
    }
    
    return bestAction;
}

void UnitModel::defend(UnitModel* attacker, int power, int skill, int attackType) {
    int skillDiff, damage, roll;
    
    damage = 0;
    skillDiff = skill - this->getStat(StatNS::DEFENSE);
    
    for (int i=0; i < power; i++) {
        roll = rand() % 8 + 1;
        
        if (roll + skillDiff >= 5 || roll == 8) {
            damage++;
        }
    }
    
    //attacker->reportHits(damage);
    this->inflictDamage(damage);
}

void UnitModel::doAction(const ActionState& statePoint) {
	if (_actions.find(statePoint.actionId) != _actions.end()) {
		_actions[statePoint.actionId]->doIt(statePoint);
	}
}

void UnitModel::doAI() {
    bool hasOffensiveAction = false;
    bool hasMovement = false;
    std::vector<ActionState> actionPoints, offensives, movements;
        
    if (_owner == 2) {
        actionPoints = this->getActions();
        
        for (std::vector<ActionState>::iterator it = actionPoints.begin(); it != actionPoints.end(); ++it) {
            switch ((*it).actionType) {
                case ActionNS::TYPE_ATTACK:
                    hasOffensiveAction = true;
                    if ((*it).active) {
                        offensives.push_back(*it);                        
                    }
                    break;
                    
                case ActionNS::TYPE_MOVEMENT:
                    hasMovement = true;
                    movements.push_back(*it);
                    break;
                    
                default:
                    break;
            }
        }
        
        if (hasOffensiveAction) {
            // choose offensive action randomly
            if (offensives.size() > 0) {
                this->doAction(offensives.at(rand() % offensives.size()));
            } else {
                // unit has to wait until offensive action becomes active, do nothing this tick
            }
        } else {
            // move closer to one unit
            if (movements.size() > 0) {
                this->doAction(movements.at(this->chooseMovementTarget(movements)));
            }
        }
    }
}

void UnitModel::fire(const MPoint& targetPos) {
    UnitModel* target;
        
    target = ModelManager::instance()->getUnitAtPos(targetPos);
    
    if (target != 0) {
        target->defend(this, this->getStat(StatNS::POWER), this->getStat(StatNS::SKILL), ATTACK_TYPE_PIERCE);    
    }
}

std::vector<ActionState> UnitModel::getActions() {
	std::vector<ActionState> actionPoints, temp;
    std::map<int, HexState> hexes;
    std::vector<UnitModel*> units;
    std::vector<MPoint> targets;
    //ActionState a;
    
    
    hexes = ModelManager::instance()->getMap()->getAllHexes();
	units = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, BattleAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(_ap, hexes, units);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

std::string UnitModel::getDescription() {
    std::stringstream stream;
	
    stream << _template->getName() << " " << _hp << "/" << this->getStat(StatNS::MAXHP);
    return stream.str();
}


int UnitModel::getId() {
	return _id;
}

int UnitModel::getOwner() {
	return _owner;
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}


int UnitModel::getStat(int stat) {
	return _template->getStat(stat) + this->getStatBonus(stat);
}

UnitState UnitModel::getState() {
    UnitState state;
    
    state.pos = _pos;
    state.ap = _ap;
    state.hp = _hp;
    state.maxAp = this->getStat(StatNS::MAXAP);
    state.maxHp = this->getStat(StatNS::MAXHP);
    state.actions = this->getActions();
    
    return state;
}

int UnitModel::getVisualType() {
    return _template->getVisualType();
}

void UnitModel::increaseAp(int points) {
	int maxAp = this->getStat(StatNS::MAXAP);
	_ap += points;
	
	if (_ap > maxAp) {
		_ap = maxAp;
	}
}

void UnitModel::inflictDamage(int damage) {
    std::stringstream ss;
    int maxHp = _template->getStat(StatNS::MAXHP);

    if (damage > 0) {
        ss << damage;
        MessageView::add(_pos, ss.str(), 1.0f, 0.0f, 0.0f, 1.0f);
    } else if (damage == 0) {
        MessageView::add(_pos, "MISS");
    } else if (damage < 0) {
        ss << -damage;
        MessageView::add(_pos, ss.str(), 0.0f, 1.0f, 0.0f, 1.0f);
    }
    
	_hp -= damage;
    
    if (_hp <= 0) {
        _hp = 0;
        ModelManager::instance()->deleteUnit(_id);
        return;
    } else if (_hp > maxHp) {
        _hp = maxHp;
    }
    this->updateObservers();
}

void UnitModel::move(const MPoint& targetPos) {
    if (ModelManager::instance()->getUnitAtPos(targetPos) == 0) {
        _pos = targetPos;        
    }
	
	this->updateObservers();
}

void UnitModel::setId(int unitId) {
    _id = unitId;
}

void UnitModel::setPosition(const MPoint& pos) {
	_pos = pos;
}

bool UnitModel::spendAp(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
        this->updateObservers();
		return true;
	} else {
		return false;
	}
}

void UnitModel::strike(const MPoint& targetPos) {
    UnitModel* target;
    
    target = ModelManager::instance()->getUnitAtPos(targetPos);
    if (target != 0) {
        target->defend(this, this->getStat(StatNS::POWER), this->getStat(StatNS::SKILL), ATTACK_TYPE_SLICE);    
    }
}

void UnitModel::tick() {
    if (this->_ap < this->getStat(StatNS::MAXAP)) {
        this->_ap += 1;
    }
	this->doAI();
	this->updateObservers();
	
}


