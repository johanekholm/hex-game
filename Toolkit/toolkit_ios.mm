/*
 *  toolkit_iphone.c
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit_iphone.h"

GPoint GPointFromCGPoint(CGPoint cgPoint) {
	GPoint point;
	point.x = cgPoint.x;
	point.y = cgPoint.y;

	return point;
}