/*
 *  geometry.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "toolkit.h"

#define GEOM_DIR_NW 1
#define GEOM_DIR_NE 2
#define GEOM_DIR_E 3
#define GEOM_DIR_SE 4
#define GEOM_DIR_SW 5
#define GEOM_DIR_W 6

// macro to compute integer floor/ceiling when divide by two
#define Floor2(T) (((T) >= 0) ? (T>>1) : (((T)-1)/2))
#define Ceil2(X) (((X) >= 0) ? (((X)+1)>>1) : ((X)/2))

// macro to compute sign of integer
#define sign(x) ((x > 0) - (x < 0))

#define PI 3.14159265

MPoint getHexVector(int direction, const MPoint& pos);
int hexDistance(const MPoint& start, const MPoint& dest);
MPoint arrayToHex(const MPoint& arrayPos);
int sightDirection(const MPoint& subject, const MPoint& object);
int turnTowards(const int& currentDirection, const MPoint& subject, const MPoint& object);

#endif
