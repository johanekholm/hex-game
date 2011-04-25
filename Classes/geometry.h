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

#define PI 3.14159265

MPoint getHexVector(int direction, const MPoint& pos);

#endif
