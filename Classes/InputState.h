//
//  InputState.h
//  OGLGame
//
//  Created by Johan Ekholm on 2010-04-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

class InputState {
public:
	InputState();
	~InputState();
	
	bool touched();

	CGPoint touchLocation();
	
	void setTouchLocation(CGPoint touchLocation);
	void setTouched(bool touched);
	
private:
	bool _touched;
	CGPoint _touchLocation;
};
