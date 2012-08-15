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

- (void)applicationWillTerminate:(UIApplication *)application {
	NSLog(@"applicationWillTerminate");
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
	// Start centralcontrol here
	NSLog(@"applicationDidFinishLaunching");
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	NSLog(@"applicationDidEnterBackground");
	// save state here
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	NSLog(@"applicationWillEnterForeground");
	
}


- (void)applicationWillResignActive:(UIApplication *)application {
	NSLog(@"applicationWillResignActive");
	// turn of OpenGL here
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	NSLog(@"applicationDidBecomeActive");
	glView.animationInterval = 1.0 / 60.0;
	// start OpenGL
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
