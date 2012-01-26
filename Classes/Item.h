/*
 *  Item.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef ITEM_H
#define ITEM_H

#include "json-forwards.h"
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
	Item();
	Item(int type, int count, std::string name, bool equipable);
    
    static Item* buildItem(int type, int count);
	Json::Value serialize();
    void deserialize(Json::Value& root);

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
	Json::Value serializeItems();
    void deserializeItems(Json::Value& root);

    void addItem(Item* item);
    std::map<int, Item*> getItems();
    bool hasItem(int type, int count);
    bool removeItem(int type, int count);
};

#endif
