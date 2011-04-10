//
//  OGLGameAppDelegate.h
//  OGLGame
//
//  Created by Michael Daley on 14/03/2009.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface OGLGameAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
	UIView *uiView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UIView *uiView;
@property (nonatomic, retain) EAGLView *glView;

@end

