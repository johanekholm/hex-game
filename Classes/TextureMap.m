//
//  TextureMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TextureMap.h"


@implementation TextureMap

- (id) initWithSubdivisions:(int)numDivs ofImage:(UIImage *)uiImage {
	self = [super initWithImage:uiImage];
	numSubdivisions = numDivs;
	return self;
}

- (void) setNumSubdivisions:(int)numDivs {
	numSubdivisions = numDivs;
}

- (void) getTexCoordsForSubWithIndex: (int)index into: (GLfloat[8])array {
	GLfloat tx, ty, size;
	
	size = 1.0f / numSubdivisions;
	tx = index % numSubdivisions * size;
	ty = index / numSubdivisions * size;
	
	//NSLog(@"size: %f, tx: %f, ty: %f", size, tx, ty);
	
	/*GLfloat		coordinates[] = {	tx,	ty + size,
									tx + size,	ty + size,
									tx,	ty,
									tx + size,	ty };*/
	
/*	array[0] = tx;
	array[1] = ty + size;
	array[2] = tx + size;
	array[3] = ty + size;
	array[4] = tx;
	array[5] = ty;
	array[6] = tx + size;
	array[7] = ty;*/

	// flipped
	array[0] = tx;
	array[1] = ty;
	array[2] = tx + size;
	array[3] = ty;
	array[4] = tx;
	array[5] = ty + size;
	array[6] = tx + size;
	array[7] = ty + size;
	
	
}

- (void) getTriangleTexCoordsForSubWithIndex: (int)index into: (GLfloat[12])array {
	GLfloat tx, ty, size;
	
	size = 1.0f / numSubdivisions;
	tx = index % numSubdivisions * size;
	ty = index / numSubdivisions * size;
	
	
	// flipped
	array[0] = tx;
	array[1] = ty;
	array[2] = tx + size;
	array[3] = ty;
	array[4] = tx;
	array[5] = ty + size;
	array[6] = tx;
	array[7] = ty + size;
	array[8] = tx + size;
	array[9] = ty;	
	array[10] = tx + size;
	array[11] = ty + size;
	
	
}

@end
