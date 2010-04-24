//
//  InputState.h
//  OGLGame
//
//  Created by Johan Ekholm on 2010-04-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "toolkit.h";

class InputState {
public:
	InputState();
	~InputState();
	
	bool touched();

	GPoint touchLocation();
	
	void setTouchLocation(GPoint touchLocation);
	void setTouched(bool touched);
	
private:
	bool _touched;
	GPoint _touchLocation;
};
