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

#include <vector>

#define ACTION_ANGLE_INITIAL -PI/2.0f
#define ACTION_ANGLE_INCREMENT PI/3.0f
#define ACTION_RADIUS 64.0f

struct ActionView {
    GPoint pos;
    int actionId;
    bool active;
};

class UnitView : public ViewController, public IObserver {
	GLfloat _facing;
	UnitModel* _unitModel;
    UnitState _state;
	std::vector<ActionView> _actionPoints;
	GameImage* _unitImage;
	GameImage* _actionImage;
	GameImage* _directionImage;

	void drawActions();	
	GPoint getActionPosition(int index);
	void updateActions();
	void updatePosition(const MPoint& pos, int direction);
    void drawHpBar();
    void drawApBar();


public:
	~UnitView();
	UnitView(UnitModel* model, GLfloat width, GLfloat height, int index);
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
	int touchedAction(GPoint point);
	void update();
    void destroyed();
};

/*class UnitView : public GameImage, public IUnitView {
	GPoint _pos;
	GLfloat _facing;
	std::vector<int> _actions;
	GameImage* _actionImage;
	GameImage* _directionImage;
	
	GPoint getActionPosition(int index);
public:
	~UnitView();
	UnitView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	void draw();
	void drawActions();
	void updatePosition(const MPoint& pos, int direction);
	void updateActions(std::vector<int> actions);
	bool wasTouched(GPoint point);
	int touchedAction(GPoint point);
};*/
