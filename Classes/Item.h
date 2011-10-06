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
    std::string _name;
    
public:
	~Item();
	Item(int type, int count, std::string name, bool equipable);
    
    static Item* buildItem(int type, int count);
    std::string getDescription();
    int getCount();
    int getType();
    void increaseCount(int increase);
};

#endif
