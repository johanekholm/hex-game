/*
 *  toolkit.h
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TOOLKIT_H
#define TOOLKIT_H

#if defined(PLATFORM_IOS)
	#import <Foundation/Foundation.h>
	#import <OpenGLES/ES1/gl.h>
    #include <cstdio>
    #define DEBUGLOG(...) { printf(__VA_ARGS__); printf("\n"); }
#elif defined(PLATFORM_DESKTOP)
	#include <QGLContext>
    inline void DEBUGLOG(...) { } // TODO: Implement
#elif defined(PLATFORM_ANDROID)
	#include <GLES/gl.h>
	#include <android/log.h>
	#define DEBUGLOG(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#elif defined(PLATFORM_MAEMO)
	#include "SDL.h"
	#include <SDL_gles.h>
    inline void DEBUGLOG(...) { } // TODO: Implement
#endif

struct GPointInTime {
	GLfloat x;
	GLfloat	y;
	double time;
};
typedef struct GPointInTime GPointInTime;


struct GPoint {
	GLfloat x;
	GLfloat y;
	
	GPoint operator+(const GPoint& other) const { 
		GPoint result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	GPoint operator-(const GPoint& other) const { 
		GPoint result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}
    
	GPoint operator*(GLfloat scalar) { 
		GPoint result;
		result.x = this->x * scalar;
		result.y = this->y * scalar;
		return result;
	}
	
	void operator+=(const GPoint& other) { 
		this->x += other.x;
		this->y += other.y;
	}

	void operator-=(const GPoint& other) { 
		this->x -= other.x;
		this->y -= other.y;
	}

	void operator=(const GPointInTime& other) { 
		this->x = other.x;
		this->y = other.y;
	}
	
	void operator=(GLfloat scalar) {
		this->x = scalar;
		this->y = scalar;
	}
		
};

typedef struct GPoint GPoint;




struct MPoint {
	int x;
	int y;
	
	MPoint operator+(const MPoint& other) const { 
		MPoint result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	MPoint operator-(const MPoint& other) const { 
		MPoint result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}
	
	GPoint operator+(GLfloat scalar) { 
		GPoint result;
		result.x = this->x + scalar;
		result.y = this->y + scalar;
		return result;
	}

	GPoint operator*(GLfloat scalar) { 
		GPoint result;
		result.x = this->x * scalar;
		result.y = this->y * scalar;
		return result;
	}
	
	void operator=(int scalar) {
		this->x = scalar;
		this->y = scalar;
	}
	
	void operator+=(const MPoint& other) { 
		this->x += other.x;
		this->y += other.y;
	}

	bool operator==(const MPoint& other) { 
		return (this->x == other.x && this->y == other.y);
	}
	
};

typedef struct MPoint MPoint;

struct RGBA {
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
    
    void makeRed() {red=1.0f; green=0.0f; blue=0.0f; alpha=1.0f;}
    void makeGreen() {red=0.0f; green=1.0f; blue=0.0f; alpha=1.0f;}
    void makeBlue() {red=0.0f; green=0.0f; blue=1.0f; alpha=1.0f;}
    void makeYellow() {red=1.0f; green=1.0f; blue=0.0f; alpha=1.0f;}
    void makeWhite() {red=1.0f; green=1.0f; blue=1.0f; alpha=1.0f;}
    void makeBlack() {red=0.0f; green=0.0f; blue=0.0f; alpha=1.0f;}
    void makeGray(GLfloat i) {red=i; green=i; blue=i; alpha=1.0f;}
};

typedef struct RGBA RGBA;

#ifdef __cplusplus
extern "C" {
#endif

GPoint GPointMake(GLfloat x, GLfloat y);
GPoint convertToGPoint(const GPointInTime& pit);
MPoint MPointMake(int x, int y);
//bool PointWithin(GPoint point, GPoint pos, GPoint size);
bool PointWithin(const GPoint& point, const GPoint& pos, GLfloat size);
GPoint transformModelPositionToView(const MPoint& pos);
RGBA RGBAMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
RGBA RGBAMakeGray(GLfloat intensity);
	
#ifdef __cplusplus
}
#endif

#endif
