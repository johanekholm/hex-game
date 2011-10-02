/*
 *  UnitView.h
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef UNITVIEWCONTROLLER_H
#define UNITVIEWCONTROLLER_H

#include "UnitModel.h"
#include "BaseUnitViewController.h"

class ShapeImage;
class GameImage;

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
	void draw(const GPoint& cameraPos);
	void drawGUI(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
	void updateState();
};

#endif