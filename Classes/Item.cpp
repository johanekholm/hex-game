/*
 *  Item.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "Item.h"
#include "toolkit.h"
#include <iostream>
#include <sstream>

Item::~Item() {

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

void ItemHandler::addItem(Item* item) {
    if (item != 0) {
        if (_items.find(item->getType()) == _items.end()) {
            _items[item->getType()] = item;
        } else {
            _items[item->getType()]->increaseCount(item->getCount());
        }
    }
    
    for (std::map<int, Item*>::iterator it = _items.begin(); it != _items.end(); ++it) {
        DEBUGLOG("An Item: %s", it->second->getDescription().c_str());
    }
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

