/*
 *  Item.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <string>

namespace ItemNS {
    const int SWORD     = 1;
    const int SHIELD    = 2;
    const int POTION    = 3;
    const int RING      = 4;
};


class Item {
    int _type;
    int _count;
    bool _equipable;
    
public:
	~Item();
	Item(int type, int count, bool equipable);
    
    static Item* buildItem(int type, int count);
};

#endif
