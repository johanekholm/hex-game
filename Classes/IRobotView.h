/*
 *  IRobotView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef IROBOTVIEW_H
#define IROBOTVIEW_H

class IRobotView {
public:
	virtual ~IRobotView() {}
	virtual void updatePosition(const GPoint& pos) = 0;
};

#endif