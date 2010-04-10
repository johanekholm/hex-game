//
//  OGLGameAppDelegate.m
//  OGLGame
//
//  Created by Michael Daley on 14/03/2009.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "OGLGameAppDelegate.h"
#import "EAGLView.h"

@implementation OGLGameAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
