//
//  InputState.h
//  OGLGame
//
//  Created by Johan Ekholm on 2010-04-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface InputState : NSObject
{
	bool isBeingTouched;
	CGPoint touchLocation;
}

@property (nonatomic, readwrite) bool isBeingTouched;
@property (nonatomic, readwrite) CGPoint touchLocation;

@end

