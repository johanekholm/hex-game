/*
 *  geometry.mm
 *  igame
 *
 *  
 *  
 *
 */

#include "geometry.h"

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
