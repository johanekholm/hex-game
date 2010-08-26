//
//  TextureMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Texture2D.h"
#include <string>

class TextureMap {
	GLuint _name;
	int _numSubdivisions;

public:
	TextureMap(const std::string& filename, int numDivs);
	~TextureMap();
	void getTriangleTexCoordsForSub(GLfloat target[12], int index);
	void getHexTexCoordsForSub(GLfloat target[24], int index, GLfloat hexPointRatio);
	void bind();
};

/*@interface TextureMap : Texture2D {
	int numSubdivisions;
}

- (id) initWithSubdivisions:(int)numDivs ofImage:(UIImage *)uiImage;
- (void) setNumSubdivisions:(int)numDivs;
- (void) getTexCoordsForSubWithIndex: (int)index  into: (GLfloat[8])array;
- (void) getTriangleTexCoordsForSubWithIndex: (int)index into: (GLfloat[12])array;

@end
*/