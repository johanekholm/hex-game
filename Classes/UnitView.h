/*
 *  UnitView.h
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef UNITVIEW_H
#define UNITVIEW_H

#include "GameImage.h"
#include "IObserver.h"
#include "ViewController.h"
#include "UnitModel.h"
#include "Action.h"
#include "BaseUnitViewController.h"

#define ACTION_ANGLE_INITIAL -PI/2.0f
#define ACTION_ANGLE_INCREMENT PI/3.0f
#define ACTION_RADIUS 64.0f

class ShapeImage;

class UnitViewController : public BaseUnitViewController {
	UnitModel* _unitModel;
    UnitState _state;
	GameImage* _unitImage;
    ShapeImage *_hpBar, *_hpBarSlot;
    ShapeImage *_apBar, *_apBarSlot;

    void updateBars();

public:
	~UnitViewController();
	UnitViewController(UnitModel* model, GLfloat width, GLfloat height, int index);
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
	void updateState();
};

/*class UnitView : public ViewController, public IObserver {
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
*/
#endif