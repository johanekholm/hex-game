/*
 *  Item.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "Item.h"
#include "toolkit.h"
#include "json.h"
#include "UnitModel.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

std::map<int, ItemTemplate*> ItemTemplate::_templates = ItemTemplate::initTemplates();
std::map<std::string, std::map<int, ItemTemplate*> > ItemTemplate::_dropMap = ItemTemplate::initDropMap();
std::map<std::string, int> ItemTemplate::_dropWeightSums = ItemTemplate::initDropWeightSums();

std::map<int, ItemTemplate*> ItemTemplate::initTemplates() {
	using namespace ItemNS;
	std::map<int, ItemTemplate*> templates;
	
	templates[SILVER] =		new ItemTemplate(SILVER, "SILVER",			0, 1, 0, 0, 0, 0);
	templates[SWORD] =		new ItemTemplate(SWORD, "SWORD",			5, 10, 0, 0, 1, 0);
	templates[SHIELD] =		new ItemTemplate(SHIELD, "SHIELD",			5, 8, 0, 0, 0, 1);
	templates[HELMET] =		new ItemTemplate(HELMET, "HELMET",			10, 5, 0, 0, 0, 1);
	templates[RING] =		new ItemTemplate(RING, "RING",				5, 12, 1, 0, 0, 0);
	templates[CHAIN_MAIL] = new ItemTemplate(CHAIN_MAIL, "CHAIN MAIL",	5, 15, 0, 0, 0, 1);
	templates[POTION] =		new ItemTemplate(POTION, "POTION",			10, 6, 0, 0, 0, 0);

	return templates;
}

std::map<std::string, std::map<int, ItemTemplate*> > ItemTemplate::initDropMap() {
	std::map<std::string, std::map<int, ItemTemplate*> > dropMap;
	int sum = 0;
	
	for (std::map<int, ItemTemplate*>::iterator it = _templates.begin(); it != _templates.end(); ++it) {
		sum += it->second->getDropWeight();
		dropMap["basic"][sum] = it->second;
	}
	
	return dropMap;
}

std::map<std::string, int> ItemTemplate::initDropWeightSums() {
	std::map<std::string, int> dropWeightSums;
	
	dropWeightSums["basic"] = 0;
	dropWeightSums["advanced"] = 0;
	
	for (std::map<int, ItemTemplate*>::iterator it = _templates.begin(); it != _templates.end(); ++it) {
		dropWeightSums["basic"] += it->second->getDropWeight();
	}
	
	return dropWeightSums;
}

ItemTemplate* ItemTemplate::getTemplate(int type) {
	if (_templates.find(type) != _templates.end()) {
        return _templates[type];
    } else {
		return 0;
	}
}

ItemTemplate* ItemTemplate::generateDrop(int level) {
	std::string category = "basic";
	int random;
	
	random = rand() % _dropWeightSums[category];

	for (std::map<int, ItemTemplate*>::iterator it = _dropMap[category].begin(); it != _dropMap[category].end(); ++it) {
		if (random < it->first ) {
			DEBUGLOG("Generated 1 %s", it->second->getName().c_str());
			return it->second;
		}
	}
	
	return 0;
}


ItemTemplate::ItemTemplate() {
	_type = 0;
	_name = "";
	_hpBonus = 0;
	_powerBonus = 0;
	_skillBonus = 0;
	_defenseBonus = 0;	
}

ItemTemplate::ItemTemplate(int type, std::string name, int dropWeight, int cost, int hp, int power, int skill, int defense) {
	_type = type;
	_name = name;
	_dropWeight = dropWeight;
	_cost = cost;
	_hpBonus = hp;
	_powerBonus = power;
	_skillBonus = skill;
	_defenseBonus = defense;
}

int ItemTemplate::getCost() {
	return _cost;
}

int ItemTemplate::getDropWeight() {
	return _dropWeight;
}

std::string ItemTemplate::getName() {
	return _name;
}

int ItemTemplate::getStatBonus(int stat) {
	using namespace StatNS;
	
	switch (stat) {
		case POWER:
			return _powerBonus;
		case SKILL:
			return _skillBonus;
		case DEFENSE:
			return _defenseBonus;
		case MAXHP:
			return _hpBonus;
			
		default:
			return 0;
	}
}

int ItemTemplate::getType() {
	return _type;
}

/*---------------------------------------------------------------*/


Item::~Item() {

}

Item::Item() {
	_type = 0;
	_count = 0;
}

Item::Item(int type, int count) {
    _type = type;
    _count = count;
	_template = ItemTemplate::getTemplate(type);
}

Item::Item(ItemTemplate* itemTemplate, int count) {
	if (itemTemplate != 0) {
		_type = itemTemplate->getType();		
	}
    _count = count;
	_template = itemTemplate;
}

Json::Value Item::serialize() {
    Json::Value root;
    root["type"] = _type;
    root["count"] = _count;
	
    return root;
}

void Item::deserialize(Json::Value& root) {
    _type = root.get("type", 0).asInt();
    _count = root.get("count", 0).asInt();
	_template = ItemTemplate::getTemplate(_type);
}


bool Item::decreaseCount(int decrease) {
    if (_count >= decrease) {
        _count -= decrease;
        return true;
    } else {
        return false;
    }
}

int Item::getCount() {
    return _count;
}

std::string Item::getDescription(bool includeCost) {
    std::stringstream stream;

    stream << _count << " " << _template->getName();

    if (includeCost) {
		stream << " " << _template->getCost() << "S";
	}
	
	return stream.str();
}

int Item::getStatBonus(int stat) {
	return _template->getStatBonus(stat);
}

int Item::getType() {
    return _type;
}

void Item::increaseCount(int increase) {
    _count += increase;
}

/*---------------------------------------------------------------*/

ItemHandler::~ItemHandler() {
    for (std::map<int, Item*>::iterator it = _items.begin(); it != _items.end(); ++it) {
        delete it->second;
    }
    _items.clear();
}

Json::Value ItemHandler::serializeItems() {
	Json::Value root;

    for (std::map<int, Item*>::iterator it = _items.begin(); it != _items.end(); ++it) {
        root.append(it->second->serialize());
    }
	
	return root;
}

void ItemHandler::deserializeItems(Json::Value& root) {
	Item* item;
	
    for (Json::ValueIterator it = root.begin(); it != root.end(); it++) {
		item = new Item();
		item->deserialize(*it);
        this->addItem(item);
    }
}


void ItemHandler::addItem(Item* item) {
    if (item != 0) {
        if (_items.find(item->getType()) == _items.end()) {
            _items[item->getType()] = item;
        } else {
            _items[item->getType()]->increaseCount(item->getCount());
        }
    }
}

void ItemHandler::addItems(const std::vector<Item*>& items) {
	for (std::vector<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
		this->addItem(*it);
	}
}

std::map<int, Item*> ItemHandler::getItems() {
    return _items;
}

std::vector<Item*> ItemHandler::getItemsAsVector() {
	std::vector<Item*> itemVector;
	for (std::map<int, Item*>::iterator it = _items.begin(); it != _items.end(); ++it) {
		itemVector.push_back(it->second);
	}
	return itemVector;
}

bool ItemHandler::hasItem(int type, int count) {
    if (_items.find(type) != _items.end()) {
        return (_items[type]->getCount() >= count);
    }
    return false;
}

bool ItemHandler::removeItem(int type, int count) {
    if (_items.find(type) != _items.end()) {
        if (_items[type]->decreaseCount(count)) {
            if (_items[type]->getCount() <= 0) {
                _items.erase(type);
            }
            return true;
        }
    }
    return false;
}

void ItemHandler::removeAllItems() {
    _items.clear();
}


/*---------------------------------------------------------------*/

ItemEquipper::~ItemEquipper() {
    for (std::map<int, Item*>::iterator it = _equippedItems.begin(); it != _equippedItems.end(); ++it) {
        delete it->second;
    }
    _equippedItems.clear();
}

Json::Value ItemEquipper::serializeEquippedItems() {
	Json::Value root;
	
    for (std::map<int, Item*>::iterator it = _equippedItems.begin(); it != _equippedItems.end(); ++it) {
        root[it->first] = it->second->serialize();
    }
	
	return root;
}

void ItemEquipper::deserializeEquippedItems(Json::Value& root) {
	Item* item;
	
    for (Json::ValueIterator it = root.begin(); it != root.end(); it++) {
		item = new Item();
		item->deserialize(*it);
        _equippedItems[it.key().asInt()] = item;
    }
}

Item* ItemEquipper::replaceEquipment(Item* item, int slot) {
	Item* oldItem = 0;
	
    if (_equippedItems.find(slot) != _equippedItems.end()) {
        oldItem = _equippedItems[slot];
    }
	
	_equippedItems[slot] = item;
	return oldItem;
}

std::map<int, Item*> ItemEquipper::getEquippedItems() {
	return _equippedItems;
}

Item* ItemEquipper::getItemInSlot(int slot) {
    if (_equippedItems.find(slot) != _equippedItems.end()) {
        return _equippedItems[slot];
    } else {
		return 0;
	}
}

int ItemEquipper::getStatBonus(int stat) {
	int bonus = 0;
	
	for (std::map<int, Item*>::iterator it = _equippedItems.begin(); it != _equippedItems.end(); ++it) {
        bonus += (it->second == 0) ? 0 : it->second->getStatBonus(stat);
    }
	return bonus;
}

Item* ItemEquipper::removeEquipment(int slot){
	return this->replaceEquipment(0, slot);
}

