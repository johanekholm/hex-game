/*
 *  IUnitView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef IUNITVIEW_H
#define IUNITVIEW_H

#include <vector>

class IUnitView {
public:
	virtual ~IUnitView() {}
	virtual void updatePosition(const MPoint& pos, int direction) = 0;
	virtual	void updateActions(std::vector<int> actions) = 0;
};

#endif
