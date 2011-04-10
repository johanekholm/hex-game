/*
 *  UnitView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameImage.h"
#include "IUnitView.h"

//#include <vector>


class UnitView : public GameImage, public IUnitView {
	GPoint _pos;
	std::vector<int> _actions;
	GameImage* _actionImage;
	
public:
	UnitView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	void draw();
	void drawActions();
	void updatePosition(const MPoint& pos);
	void updateActions(std::vector<int> actions);
	bool wasTouched(GPoint point);
};