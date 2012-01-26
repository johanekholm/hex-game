/*
 *  Item.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "Item.h"
#include "toolkit.h"
#include "json.h"
#include <iostream>
#include <sstream>

Item::~Item() {

}

Item::Item() {
	_type = 0;
	_name = "";
	_count = 0;
	_equipable = false;
}

Item::Item(int type, int count, std::string name, bool equipable) {
    _type = type;
    _name = name;
    _count = count;
    _equipable = equipable;
}

Item* Item::buildItem(int type, int count) {
    using namespace ItemNS;
    
    switch (type) {
        case SWORD:
            return new Item(type, count, "SWORD", true);
        case SHIELD:
            return new Item(type, count, "SHIELD", true);
        case POTION:
            return new Item(type, count, "POTION", false);
        case RING:
            return new Item(type, count, "RING", true);
        case SILVER:
            return new Item(type, count, "SILVER", true);

        default:
            break;
    }
    
    return 0;
}

Json::Value Item::serialize() {
    Json::Value root;
    root["type"] = _type;
    root["count"] = _count;
    root["name"] = _name;
    root["equip"] = _equipable;
	
    return root;
}

void Item::deserialize(Json::Value& root) {
    _type = root.get("type", 0).asInt();
    _count = root.get("count", 0).asInt();
    _name = root.get("name", 0).asString();
    _equipable = root.get("equip", 0).asBool();
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

std::string Item::getDescription() {
    std::stringstream stream;

    stream << _count << " " << _name;
    return stream.str();
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
    
    /*for (std::map<int, Item*>::iterator it = _items.begin(); it != _items.end(); ++it) {
        DEBUGLOG("An Item: %s", it->second->getDescription().c_str());
    }*/
}

std::map<int, Item*> ItemHandler::getItems() {
    return _items;
}

bool ItemHandler::hasItem(int type, int count) {
    if (_items.find(type) == _items.end()) {
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

