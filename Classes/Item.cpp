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

        default:
            break;
    }
    
    return 0;
}

std::string Item::getDescription() {
    std::stringstream stream;

    stream << _count << " " << _name;
    return stream.str();
}

int Item::getType() {
    return _type;
}