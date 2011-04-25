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
#include "IObservable.h"

#include <vector>

#define ACTION_ANGLE_INITIAL -PI/2.0f
#define ACTION_ANGLE_INCREMENT PI/3.0f
#define ACTION_RADIUS 64.0f

class UnitView : public ViewController, public IObservable {
	GPoint _pos;
	GLfloat _facing;
	UnitModel* _unitModel;
	std::vector<int> _actions;
	GameImage* _unitImage;
	GameImage* _actionImage;
	GameImage* _directionImage;

	void drawActions();	
	GPoint getActionPosition(int index);
	void updateActions(std::vector<int> actions);
	void updatePosition(const MPoint& pos, int direction);

public:
	~UnitView();
	UnitView(UnitModel* model, GLfloat aWidth, GLfloat aHeight, int index);
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
	int touchedAction(GPoint point);
	void update();
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
