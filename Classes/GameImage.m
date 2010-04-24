//
//  GameImage.m
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GameImage.h"


@implementation GameImage

@synthesize position;

- (id) initWithTextureMap: (TextureMap *)texMap withIndex: (int)index {
	self = [super init];
	if (self != nil) {
		texture = [texMap retain];
		texIndex = index;
		size = CGSizeMake(32.0f, 32.0f);
		position = GPointMake(0.0f, 0.0f);
	}
	return self;
}

- (id) initWithSize: (CGSize)newSize andTexture: (TextureMap *)texMap withIndex: (int)index {
	self = [super init];
	if (self != nil) {
		size = CGSizeMake(newSize.width, newSize.height);
		position = GPointMake(size.width/2.0f, size.height/2.0f);
		
		texture = [texMap retain];
		texIndex = index;
	}
	return self;
}

- (void) draw {
	//GLfloat	texCoords[8];
	GLfloat	texCoords[12];
/*	GLfloat	vertices[] = {	-size.width/2.0f, -size.height/2.0f, 0.0f,
							size.width/2.0f, -size.height/2.0f, 0.0f,
							-size.width/2.0f, size.height/2.0f, 0.0f,
							size.width/2.0f, size.height/2.0f, 0.0f };
*/
	GLfloat	vertices[] = {	-size.width/2.0f, -size.height/2.0f, 0.0f,
		size.width/2.0f, -size.height/2.0f, 0.0f,
		-size.width/2.0f, size.height/2.0f, 0.0f,
		-size.width/2.0f, size.height/2.0f, 0.0f,
		size.width/2.0f, -size.height/2.0f, 0.0f,		
		size.width/2.0f, size.height/2.0f, 0.0f };
	
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0f);

	
	//[texture getTexCoordsForSubWithIndex:texIndex into:texCoords];	
	[texture getTriangleTexCoordsForSubWithIndex:texIndex into:texCoords];	
	[texture bind];
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
}

- (void) dealloc {
	[texture release];
	[super dealloc];
}

@end
