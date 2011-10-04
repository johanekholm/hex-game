/*
 *  Item.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "Item.h"
#include "toolkit.h"
#include <string>
#include <iostream>

Item::~Item() {

}

Item::Item(int type, int count, bool equipable) {
    _type = type;
    _count = count;
    _equipable = equipable;
}

Item* Item::buildItem(int type, int count) {
    using namespace ItemNS;
    bool equipable;
    
    switch (type) {
        case SWORD:
            equipable = true; break;

        case SHIELD:
            equipable = true; break;

        case POTION:
            equipable = false; break;

        case RING:
            equipable = true; break;

        default:
            break;
    }
    
    return new Item(type, count, equipable);
}