/*
 *  geometry.mm
 *  igame
 *
 *  
 *  
 *
 */

#include "geometry.h"
#include <algorithm>
#include <stdlib.h>
#include <map>
#include <iostream>

MPoint getHexVector(int direction, const MPoint& pos) {
	MPoint v;
	
	switch (direction) {
		case GEOM_DIR_E:
			v.x = 1;
			v.y = 0;
			break;

		case GEOM_DIR_W:
			v.x = -1;
			v.y = 0;
			break;

		case GEOM_DIR_NW:
			if (pos.y % 2 == 0) {
				v.y = -1;
				v.x = -1;
			} else {
				v.y = -1;
				v.x = 0;				
			}
			break;

		case GEOM_DIR_NE:
			if (pos.y % 2 == 0) {
				v.y = -1;
				v.x = 0;
			} else {
				v.y = -1;
				v.x = 1;				
			}
			break;

		case GEOM_DIR_SW:
			if (pos.y % 2 == 0) {
				v.y = 1;
				v.x = -1;
			} else {
				v.y = 1;
				v.x = 0;				
			}
			break;

		case GEOM_DIR_SE:
			if (pos.y % 2 == 0) {
				v.y = 1;
				v.x = 0;
			} else {
				v.y = 1;
				v.x = 1;				
			}
			break;
			
		default:
			break;
	}

	return v;
}

MPoint arrayToHex(const MPoint& arrayPos) {
    MPoint hexPos;
    
    hexPos.x = arrayPos.x - Floor2(arrayPos.y);
    hexPos.y = arrayPos.x + Ceil2(arrayPos.y);
        
    return hexPos;
}



int hexDistance(const MPoint& start, const MPoint& dest) {
    MPoint A, B;
    int dx, dy, dist;
    
    // calculate hexspace coordinates of A and B
    A.x = start.x - Floor2(start.y); 
    A.y = start.x + Ceil2(start.y);
    B.x = dest.x - Floor2(dest.y); 
    B.y = dest.x + Ceil2(dest.y);
    
    // calculate distance using hexcoords as per previous algorithm
    dx = B.x - A.x;
    dy = B.y - A.y;
    
    if (sign(dx) == sign(dy)) {
        dist = std::max(abs(dx),abs(dy));
    } else {
        dist = abs(dx) + abs(dy);
    }
    
    return dist;
}

int sightDirection(const MPoint& subject, const MPoint& object) {
    MPoint hexSubject, hexObject;
    int dx, dy;
    
    hexSubject = arrayToHex(subject);
    hexObject = arrayToHex(object);
    
    dx = hexSubject.x - hexObject.x;
    dy = hexSubject.y - hexObject.y;
        
    if (dx == 0) {
        if (dy > 0) {
            return GEOM_DIR_NW;
        } else {
            return GEOM_DIR_SE;
        }
    } else if (dy == 0) {
        if (dx > 0) {
            return GEOM_DIR_SW;
        } else {
            return GEOM_DIR_NE;
        }
    } else if (dx == dy) {
        if (dx > 0) {
            return GEOM_DIR_W;
        } else {
            return GEOM_DIR_E;
        }
    }
    
    return -1;
}

int directionTowards(const int& currentDirection, const MPoint& subject, const MPoint& object) {
    MPoint hexSubject, hexObject;
    int dx, dy;
    std::map<int, int> axis;
    std::map<int, int> angles;
    int angle;
    int min = 100;
    int minDir = 0; 
    int tiedDir = 0;

    hexSubject = arrayToHex(subject);
    hexObject = arrayToHex(object);
    
    dx = hexSubject.x - hexObject.x;
    dy = hexSubject.y - hexObject.y;

    // calculate how far off each direction is to object direction
    axis[(dx > 0) ? GEOM_DIR_SW : GEOM_DIR_NE] = abs(dy);
    axis[(dy > 0) ? GEOM_DIR_NW : GEOM_DIR_SE] = abs(dx);
    axis[(dx > 0) ? GEOM_DIR_W : GEOM_DIR_E] = abs(dx-dy);
    
    // select the direction that is least off, two if there is a tie
    // also calculate angle distance for each direction
    for (std::map<int, int>::iterator it = axis.begin(); it != axis.end(); ++it) {
        angle = abs(it->first - currentDirection);
        
        angles[it->first] = angle;
        
		if (it->second < min) {
            min = it->second;
            minDir = it->first;
            tiedDir = 0;
        } else if (it->second == min) {
            tiedDir = it->first;
        }
	}
    
    // return the best direction, or in case of tie the one that is nearest to subject's current direction
    if (tiedDir == 0) {
        return minDir;
    } else {
        if (angles[minDir] < angles[tiedDir]) {
            return minDir;
        } else {
            return tiedDir;
        }
    }
}

DistanceToHexSorter::DistanceToHexSorter(const MPoint& pos) {
	origin = pos;
}

bool DistanceToHexSorter::operator() (const MPoint& first, const MPoint& second) {
	return hexDistance(origin, first) < hexDistance(origin, second);
}
