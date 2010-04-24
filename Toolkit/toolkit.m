/*
 *  toolkit.c
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"

GPoint GPointMake(GLfloat x, GLfloat y) {
	GPoint point;
	point.x = x;
	point.y = y;
	
	return point;
}