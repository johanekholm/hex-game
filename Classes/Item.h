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
	const int HELMET	= 6;
	const int CHAIN_MAIL = 7;
};


class ItemTemplate {
protected:
	static std::map<int, ItemTemplate*> _templates;
	static std::map<std::string, std::map<int, ItemTemplate*> > _dropMap;
	static std::map<std::string, int> _dropWeightSums;

    int _type;
	int _cost;
	int _dropWeight;
    bool _equipable;
    std::string _name;
	int _hpBonus;
	int _powerBonus;
	int _skillBonus;
	int _defenseBonus;
	
public:
	static std::map<int, ItemTemplate*> initTemplates();
	static std::map<std::string, std::map<int, ItemTemplate*> > initDropMap();
	static std::map<std::string, int> initDropWeightSums();
	static ItemTemplate* getTemplate(int type);
	static ItemTemplate* generateDrop(int level);
	ItemTemplate();
	ItemTemplate(int type, std::string name, int dropWeight, int cost, int hp, int power, int skill, int defense);
	int getCost();
	int getDropWeight();
	std::string getName();
	int getStatBonus(int stat);
};

class Item {
    int _type;
    int _count;
	ItemTemplate* _template;
    
public:
	~Item();
	Item();
	Item(int type, int count);
    
    //static Item* buildItem(int type, int count);
	Json::Value serialize();
    void deserialize(Json::Value& root);

    bool decreaseCount(int decrease);
    std::string getDescription(bool includeCost=false);
    int getCount();
	int getStatBonus(int stat);
    int getType();
    void increaseCount(int increase);
};



class ItemHandler {
    std::map<int, Item*> _items;

public:
    virtual ~ItemHandler();
	Json::Value serializeItems();
    void deserializeItems(Json::Value& root);

    void addItem(Item* item);
    void addItems(const std::vector<Item*>& items);
    std::map<int, Item*> getItems();
    bool hasItem(int type, int count);
    bool removeItem(int type, int count);
};


class ItemEquipper {
protected:
    std::map<int, Item*> _equippedItems;
	
public:
    virtual ~ItemEquipper();
	Json::Value serializeEquippedItems();
    void deserializeEquippedItems(Json::Value& root);
	
    std::map<int, Item*> getEquippedItems();
    Item* getItemInSlot(int slot);
	int getStatBonus(int stat);
    Item* removeEquipment(int slot);
    Item* replaceEquipment(Item* item, int slot);
};

#endif
