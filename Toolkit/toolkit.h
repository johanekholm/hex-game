/*
 *  toolkit.h
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <OpenGLES/ES1/gl.h>

#ifndef TOOLKIT_H
#define TOOLKIT_H



struct GPoint {
	GLfloat x;
	GLfloat y;
	
	GPoint operator+(const GPoint& other) { 
		GPoint result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}
	
	void operator+=(const GPoint& other) { 
		this->x += other.x;
		this->y += other.y;
	}
	
};

typedef struct GPoint GPoint;


struct GPointInTime {
	GLfloat x;
	GLfloat	y;
	double time;
};
typedef struct GPointInTime GPointInTime;


struct MPoint {
	int x;
	int y;
	
	MPoint operator+(const MPoint& other) { 
		MPoint result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}
	
	void operator+=(const MPoint& other) { 
		this->x += other.x;
		this->y += other.y;
	}
	
};

typedef struct MPoint MPoint;

#ifdef __cplusplus
extern "C" {
#endif

GPoint GPointMake(GLfloat x, GLfloat y);
MPoint MPointMake(int x, int y);

#ifdef __cplusplus
}
#endif

#endif