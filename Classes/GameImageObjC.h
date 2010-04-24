//
//  GameImage.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TextureMap.h"

@interface GameImage : NSObject {
	CGPoint position;
	CGSize size;
	TextureMap *texture;
	int texIndex;
}

@property (nonatomic, readwrite) CGPoint position;

- (id) initWithTextureMap: (TextureMap*)texMap withIndex: (int)index;
- (id) initWithSize: (CGSize)newSize andTexture: (TextureMap*)texMap withIndex: (int)index;
- (void) draw;

@end
