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
#include "IObserver.h"
#include <vector>
#include <string>

class ShapeImage;
class GameImage;
class StringImage;
class MultiRowStringImage;
class UnitModel;
struct UnitState;

struct MenuChoice {
    int choiceId;
    std::string label;
	
	static MenuChoice makeChoice(int choiceId, const std::string& label);
};

class BaseMenuNodeVC;
class ParentMenuNodeVC;

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

class MenuAction {
public:
	virtual ~MenuAction() {}
	virtual void doIt() = 0;
	virtual bool isInputRequired() = 0;
    virtual std::vector<MenuChoice> getChoices() = 0;
	virtual void reportChoice(int choiceId) = 0;
	virtual void reset() = 0;
};

/*---------------------------------------------------------------*/

class ChoiceMenuVC : public MenuViewController {
    ControlCallback& _returnControl;
    
public:
	ChoiceMenuVC(ControlCallback& control, std::vector<MenuChoice> choices, bool addCancelButton=true);
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
    ParentMenuNodeVC* _parentNode;
    int _choiceId;
    StringImage* _label;
    ShapeImage* _button;
    
public:
	virtual ~BaseMenuNodeVC();
	BaseMenuNodeVC(MenuViewController* menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height);
	void draw(const GPoint& cameraPos);
	virtual void drawGUI(const GPoint& cameraPos);
    ParentMenuNodeVC* getParent();
	virtual bool handleEvent(const TouchEvent& event) = 0;
    virtual void setMenu(MenuViewController* menuVC);
    void setParent(ParentMenuNodeVC* parent);
};


/*---------------------------------------------------------------*/

class ParentMenuNodeVC : public BaseMenuNodeVC {
protected:
    std::vector<BaseMenuNodeVC*> _subNodes;

	void destroySubNodes();

public:
	virtual ~ParentMenuNodeVC();
	ParentMenuNodeVC(MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height, bool doAutoPosition);
	void registerAndAutoPositionSubNodes(bool doAutoPosition, GLfloat verticalSpacing);
	virtual void drawGUI(const GPoint& cameraPos);
	virtual void goUp();
	virtual bool handleEvent(const TouchEvent& event);
    virtual void reportChoice(int choiceId);
    virtual void setMenu(MenuViewController* menuVC);
};

/*---------------------------------------------------------------*/

class ActionMenuNodeVC : public ParentMenuNodeVC {
protected:
	MenuAction* _action;
	
	void buildSubNodes();
	
public:
	virtual ~ActionMenuNodeVC();
	ActionMenuNodeVC(MenuAction* action, MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height);
	virtual void goUp();
	virtual bool handleEvent(const TouchEvent& event);
    virtual void reportChoice(int choiceId);
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
private:
	MultiRowStringImage* _text;
public:
	~TextMenuNodeVC();
	TextMenuNodeVC(MenuViewController* _menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height);
	virtual void drawGUI(const GPoint& cameraPos);
	virtual bool handleEvent(const TouchEvent& event);
};

/*---------------------------------------------------------------*/

class UnitInfoMenuNodeVC : public BaseMenuNodeVC, public IObserver {
private:
	UnitModel* _unit;
	StringImage *_stringName, *_stringPower, *_stringSkill, *_stringDefense, *_stringAp;

	void buildStrings();

public:
	virtual ~UnitInfoMenuNodeVC();
	UnitInfoMenuNodeVC(MenuViewController* _menuVC, UnitModel* unit, const GPoint& pos);
	virtual void drawGUI(const GPoint& cameraPos);
	virtual bool handleEvent(const TouchEvent& event);
	void destroyed();
	void updateState();
	
};

/*---------------------------------------------------------------*/

#endif
