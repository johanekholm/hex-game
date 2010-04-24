/*
 *  toolkit.h
 *  igame
 *
 *  Created by Magnus Ahlberg on 2010-04-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <OpenGLES/ES1/gl.h>

struct GPoint {
	GLfloat x;
	GLfloat y;
};
typedef struct GPoint GPoint;

#ifdef __cplusplus
extern "C" {
#endif

GPoint GPointMake(float x, float y);

#ifdef __cplusplus
}
#endif