//
//  GameImage.mm
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GameImage.h"
#import "TextureMap.h"

GameImage::GameImage(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index) {
	width = aWidth;
	height = aHeight;
	texture = tex;
	texIndex = index;
}

void GameImage::setPosition(GPoint point) {
	position = point;
}

void GameImage::draw() {
	this->drawAt(position);
}

void GameImage::drawAt(const GPoint &pos) {
	GLfloat	texCoords[12];
	GLfloat	vertices[] = {	-width/2.0f, -height/2.0f, 0.0f,
		width/2.0f, -height/2.0f, 0.0f,
		-width/2.0f, height/2.0f, 0.0f,
		-width/2.0f, height/2.0f, 0.0f,
		width/2.0f, -height/2.0f, 0.0f,		
		width/2.0f, height/2.0f, 0.0f };
	
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	
	texture->getTriangleTexCoordsForSub(texCoords, texIndex);
	texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
}

/*@implementation GameImage

@synthesize position;

- (id) initWithTextureMap: (TextureMap *)texMap withIndex: (int)index {
	self = [super init];
	if (self != nil) {
		texture = [texMap retain];
		texIndex = index;
		size = CGSizeMake(32.0f, 32.0f);
		position = CGPointMake(0.0f, 0.0f);
	}
	return self;
}

- (id) initWithSize: (CGSize)newSize andTexture: (TextureMap *)texMap withIndex: (int)index {
	self = [super init];
	if (self != nil) {
		size = CGSizeMake(newSize.width, newSize.height);
		position = CGPointMake(size.width/2.0f, size.height/2.0f);
		
		texture = [texMap retain];
		texIndex = index;
	}
	return self;
}

- (void) draw {
	//GLfloat	texCoords[8];
	GLfloat	texCoords[12];
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
*/