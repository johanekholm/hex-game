/*
 *  MenuView.h
 *
 *  Created by Johan Ekholm
 *
 */

#include "ViewController.h"
#include "Action.h"
#include <vector>

class ShapeImage;
class GameImage;
class StringImage;

struct MenuChoice {
    int choiceId;
    std::string label;
};

class MenuItemView : public ViewController {
    GPoint _pos;
    int _choiseId;
    StringImage* _label;
    ShapeImage* _button;
    
public:
    ~MenuItemView();
    MenuItemView(const char& label, int choiseId, const GPoint& pos);
    int getChoiseId();
    void draw();
    void drawGUI();
	bool handleEvent(const TouchEvent& event);
};

class MenuView : public ViewController {
	AdventureAction* _action;
	std::vector<MenuItemView*> _menuItems;
    ShapeImage* _background;

	void drawMenuItems();	
    MenuItemView* getTouchedMenuItem(const GPoint& point);

public:
	~MenuView();
	MenuView(AdventureAction* action, std::vector<MenuChoice> choices);
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
};

