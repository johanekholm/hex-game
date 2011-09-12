/*
 *  BaseUnitViewController.h
 *
 *  Created by Johan Ekholm.
 *  
 *
 */

#ifndef BASEUNITVIEWCONTROLLER_H
#define BASEUNITVIEWCONTROLLER_H

#include "IObserver.h"
#include "ViewController.h"
#include <vector>

class GameImage;
class ShapeImage;
struct ActionState;

struct ActionView {
    GPoint pos;
    int actionId;
    bool active;
    ActionState* statePoint;
};

class BaseUnitViewController : public ViewController, public IObserver {
protected:
    GameImage* _actionImage;
    ShapeImage *_actionMarker;
    ActionView* _selectedActionView;
    std::vector<ActionView> _actionPoints;

	void drawActions();	
    ActionState* getTouchedActionState(GPoint point);
    ActionView* getTouchedActionView(GPoint point);
	void updateActions(std::vector<ActionState>& actionStates);
	void updatePosition(const MPoint& pos);

public:
	virtual ~BaseUnitViewController();
	BaseUnitViewController(GLfloat width, GLfloat height, int index);
    void destroyed();
	void drawGUI();
    void setFocus(bool hasFocus);
};

#endif