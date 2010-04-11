//
//  TextureMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Texture2D.h"

@interface TextureMap : Texture2D {
	int numSubdivisions;
}

- (id) initWithSubdivisions:(int)numDivs ofImage:(UIImage *)uiImage;
- (void) setNumSubdivisions:(int)numDivs;
- (void) getTexCoordsForSubWithIndex: (int)index  into: (GLfloat[8])array;

@end
