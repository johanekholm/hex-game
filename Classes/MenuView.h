/*
 *  MenuView.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef MENUVIEWCONTROLLER_H
#define MENUVIEWCONTROLLER_H

#include "ViewController.h"
#include "ControlCallback.h"
#include <vector>
#include <string>

class ShapeImage;
class GameImage;
class StringImage;

struct MenuChoice {
    int choiceId;
    std::string label;
};

class BaseMenuNodeVC;

/*---------------------------------------------------------------*/

class MenuViewController : public ViewController {
protected:
    BaseMenuNodeVC* _rootNode;
    BaseMenuNodeVC* _focus;
    ShapeImage* _background;

public:
	~MenuViewController();
	MenuViewController();
	MenuViewController(BaseMenuNodeVC* root);
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
    void goUp();
	bool handleEvent(const TouchEvent& event);
    virtual void reportChoice(int choiceId);
    void setFocus(BaseMenuNodeVC* focus);
};

/*---------------------------------------------------------------*/

class MenuActionCallback : public ControlCallback {
public:
	virtual ~MenuActionCallback() {}
	virtual bool isInputRequired() {return false;};
    virtual std::vector<MenuChoice> getChoices() = 0;
};

/*---------------------------------------------------------------*/

class ChoiceMenuVC : public MenuViewController {
    ControlCallback& _returnControl;
    
public:
	ChoiceMenuVC(ControlCallback& control, std::vector<MenuChoice> choices);
    virtual void reportChoice(int choiceId);    
};

/*---------------------------------------------------------------*/

class TextboxMenuVC : public MenuViewController {
    ControlCallback& _returnControl;
    
public:
	TextboxMenuVC(ControlCallback& control, const std::string& text, const std::string& buttonLabel);
    virtual void reportChoice(int choiceId);    
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
	void draw(const GPoint& cameraPos);
	virtual void drawGUI(const GPoint& cameraPos);
    BaseMenuNodeVC* getParent();
	virtual bool handleEvent(const TouchEvent& event) = 0;
    virtual void setMenu(MenuViewController* menuVC);
    void setParent(BaseMenuNodeVC* parent);
};


/*---------------------------------------------------------------*/

class ParentMenuNodeVC : public BaseMenuNodeVC {
protected:
    std::vector<BaseMenuNodeVC*> _subNodes;

public:
	virtual ~ParentMenuNodeVC();
	ParentMenuNodeVC(MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height);
	virtual void drawGUI(const GPoint& cameraPos);
	virtual bool handleEvent(const TouchEvent& event);
    virtual void setMenu(MenuViewController* menuVC);
};

/*---------------------------------------------------------------*/

class ActionMenuNodeVC : public ParentMenuNodeVC {
protected:
	MenuActionCallback& _action;
	
	void buildSubNodes();
	
public:
	virtual ~ActionMenuNodeVC();
	ActionMenuNodeVC(MenuActionCallback& action, MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height);
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

/*---------------------------------------------------------------*/

class TextMenuNodeVC : public BaseMenuNodeVC {
    
public:
	TextMenuNodeVC(MenuViewController* _menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height);
	virtual bool handleEvent(const TouchEvent& event);
};

#endif
