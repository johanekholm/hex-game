/*
 *  Item.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <map>
#include <string>

namespace ItemNS {
    const int SWORD     = 1;
    const int SHIELD    = 2;
    const int POTION    = 3;
    const int RING      = 4;
    const int SILVER    = 5;
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
    bool decreaseCount(int decrease);
    std::string getDescription();
    int getCount();
    int getType();
    void increaseCount(int increase);
};



class ItemHandler {
    std::map<int, Item*> _items;

public:
    ~ItemHandler();
    void addItem(Item* item);
    std::map<int, Item*> getItems();
    bool hasItem(int type, int count);
    bool removeItem(int type, int count);
};

#endif
