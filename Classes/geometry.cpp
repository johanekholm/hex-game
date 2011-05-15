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
