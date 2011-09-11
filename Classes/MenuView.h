/*
 *  MenuView.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef MENUVIEWCONTROLLER_H
#define MENUVIEWCONTROLLER_H

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
    int _choiceId;
    StringImage* _label;
    ShapeImage* _button;
    
public:
    ~MenuItemView();
    MenuItemView(const char& label, int choiceId, const GPoint& pos);
    int getChoiceId();
    void draw();
    void drawGUI();
	bool handleEvent(const TouchEvent& event);
};

/*
 class IChoiseCallback {
 public:
 void reportChoise(int choiseId) = 0;
 };
 */ 

class BaseMenuNodeVC;

/*---------------------------------------------------------------*/

class MenuViewController : public ViewController {
	AdventureAction* _action;
    BaseMenuNodeVC* _rootNode;
    BaseMenuNodeVC* _focus;
    ShapeImage* _background;

public:
	~MenuViewController();
	MenuViewController();
	MenuViewController(AdventureAction* action, std::vector<MenuChoice> choices);
	void draw();
	void drawGUI();
    void goUp();
	bool handleEvent(const TouchEvent& event);
    void reportChoice(int choiceId);
    void setFocus(BaseMenuNodeVC* focus);
};

/*---------------------------------------------------------------*/

class BaseMenuNodeVC : public ViewController {
protected:
    MenuViewController* _menuVC;
    BaseMenuNodeVC* _parentNode;
    int _choiceId;
    StringImage* _label;
    ShapeImage* _button;
    
public:
	virtual ~BaseMenuNodeVC();
	BaseMenuNodeVC(MenuViewController* menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height);
	void draw();
	virtual void drawGUI();
    BaseMenuNodeVC* getParent();
	virtual bool handleEvent(const TouchEvent& event) = 0;
    void setMenu(MenuViewController* menuVC);
    void setParent(BaseMenuNodeVC* parent);
};


/*---------------------------------------------------------------*/

class ParentMenuNodeVC : public BaseMenuNodeVC {
    std::vector<BaseMenuNodeVC*> _subNodes;

public:
	virtual ~ParentMenuNodeVC();
	ParentMenuNodeVC(MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height);
	virtual void drawGUI();
	virtual bool handleEvent(const TouchEvent& event);
};

/*---------------------------------------------------------------*/

class LeafMenuNodeVC : public BaseMenuNodeVC {
    
public:
	virtual ~LeafMenuNodeVC();
	LeafMenuNodeVC(MenuViewController* _menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height);
	virtual bool handleEvent(const TouchEvent& event);
};

/*---------------------------------------------------------------*/

class BackButtonMenuNodeVC : public BaseMenuNodeVC {
    
public:
	BackButtonMenuNodeVC(MenuViewController* _menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height);
	virtual bool handleEvent(const TouchEvent& event);
};

#endif
