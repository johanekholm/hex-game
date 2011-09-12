/*
 *  UnitView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameImage.h"
//#include "IUnitView.h"
#include "IObserver.h"
#include "ViewController.h"
#include "UnitModel.h"
#include "Action.h"

#include <vector>

#define ACTION_ANGLE_INITIAL -PI/2.0f
#define ACTION_ANGLE_INCREMENT PI/3.0f
#define ACTION_RADIUS 64.0f

class ShapeImage;

struct ActionView {
    GPoint pos;
    int actionId;
    bool active;
    ActionState* statePoint;
};

class UnitView : public ViewController, public IObserver {
	UnitModel* _unitModel;
    UnitState _state;
	std::vector<ActionView> _actionPoints;
	GameImage* _unitImage;
	GameImage* _actionImage;
	GameImage* _directionImage;
    ShapeImage *_hpBar, *_hpBarSlot;
    ShapeImage *_apBar, *_apBarSlot;
    ShapeImage *_actionMarker;
    ActionView* _selectedActionView;


	void drawActions();	
	GPoint getActionPosition(int index);
    ActionState* getTouchedActionState(GPoint point);
    ActionView* getTouchedActionView(GPoint point);
	void updateActions();
    void updateBars();
	void updatePosition(const MPoint& pos);

public:
	~UnitView();
	UnitView(UnitModel* model, GLfloat width, GLfloat height, int index);
    void destroyed();
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
    void setFocus(bool hasFocus);
	void updateState();
};

