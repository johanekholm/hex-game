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

GPoint convertToGPoint(const GPointInTime& pit) {
	GPoint point;
	point.x = pit.x;
	point.y = pit.y;
	
	return point;
}

MPoint MPointMake(int x, int y) {
	MPoint point;
	point.x = x;
	point.y = y;
	
	return point;
}

/*bool PointWithin(GPoint point, GPoint pos, GPoint size) {
	return (point.x >= pos.x - size.x/2.0f && point.x <= pos.x + size.x/2.0f && point.y >= pos.y - size.y/2.0f && point.y <= pos.y + size.y/2.0f);
}*/

bool PointWithin(const GPoint& point, const GPoint& pos, GLfloat size) {
	return (point.x >= pos.x - size/2.0f && point.x <= pos.x + size/2.0f && point.y >= pos.y - size/2.0f && point.y <= pos.y + size/2.0f);
}


GPoint transformModelPositionToView(const MPoint& pos) {
	GPoint viewPos;
    viewPos.x = 64.0f + (GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f;
	viewPos.y = 64.0f + (GLfloat)pos.y * 50.0f;
    return viewPos;
}

RGBA RGBAMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    RGBA color;
    color.red = red;
    color.green = green;
    color.blue = blue;
    color.alpha = alpha;
    return color;
}

RGBA RGBAMakeGray(GLfloat intensity) {
    RGBA color;
    color.red = intensity;
    color.green = intensity;
    color.blue = intensity;
    color.alpha = 1.0f;
    return color;    
}