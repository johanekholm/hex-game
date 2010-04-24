//
//  TextureMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TextureMap.h"

TextureMap::~TextureMap() {
	glDeleteTextures(1, &_name);
}

TextureMap::TextureMap(const std::string& filename, int numDivs) {
	NSString *objCFilename;
	Texture2D *tex;
	
	_numSubdivisions = numDivs;
	
	objCFilename = [[NSString alloc] initWithCString: filename.c_str() encoding: NSASCIIStringEncoding]; //NSASCIIStringEncoding
	
	NSLog(@"filename: %@", objCFilename);
	
	tex = [[Texture2D alloc] initWithImage: [UIImage imageNamed: objCFilename]];
	_name = [tex name];
	[tex dealloc];
	
}

void TextureMap::bind() {
	glBindTexture(GL_TEXTURE_2D, _name);	
}

void TextureMap::getTriangleTexCoordsForSub(GLfloat target[12], int index) {
	GLfloat tx, ty, size;
	
	size = 1.0f / _numSubdivisions;
	tx = index % _numSubdivisions * size;
	ty = index / _numSubdivisions * size;
	
	
	// flipped
	target[0] = tx;
	target[1] = ty;
	target[2] = tx + size;
	target[3] = ty;
	target[4] = tx;
	target[5] = ty + size;
	target[6] = tx;
	target[7] = ty + size;
	target[8] = tx + size;
	target[9] = ty;	
	target[10] = tx + size;
	target[11] = ty + size;
}

/*
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
*/