//
//  EAGLView.m
//  Tutorial1
//
//  Created by Michael Daley on 25/02/2009.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//



#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "InputManager.h"
#import "Texture2D.h"

#include "UnitModel.h"
#include "UnitView.h"

#include "CentralControl.h"
#include "toolkit.h"
#include "toolkit_ios.h"
#include "TextureCatalog.h"

#define USE_DEPTH_BUFFER 0

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;
- (void) updateScene:(float)delta;
- (void) renderScene;
- (GLuint) loadTexture:(NSString *)fileName;

@end


@implementation EAGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)dealloc {
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
	delete centralControl;

	TextureCatalog::destroy();
	
    [context release];  
    [super dealloc];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
    
    if ((self = [super initWithCoder:coder])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        animationInterval = 1.0 / 60.0;
		
		CGRect rect = [[UIScreen mainScreen] bounds];
		
		// Set up OpenGL projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrthof(0, rect.size.width, 0, rect.size.height, -1, 1);
		
		// flipped ortho-view
		glOrthof(0, rect.size.width, rect.size.height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);

		// Initialize OpenGL states
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		TextureCatalog* catalog = TextureCatalog::instance();
		
		catalog->addAndLoad("hexTiles", [self loadTexture:@"texmap_hex.png"], 2);
		catalog->addAndLoad("actions", [self loadTexture:@"actions.png"], 4);
		catalog->addAndLoad("icons", [self loadTexture:@"icons.png"], 4);
		catalog->addAndLoad("units", [self loadTexture:@"texmap.png"], 2);
		
		centralControl = CentralControl::instance();
    }
    return self;
}


- (void)mainGameLoop {
	CFTimeInterval		time;
	float				delta;
	time = CFAbsoluteTimeGetCurrent();
	delta = (time - lastTime);
	
	centralControl->update();
	
	[self updateScene:delta];
	[self renderScene];
	
	lastTime = time;
}


- (void)updateScene:(float)delta {

}


- (void)renderScene {
    
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	centralControl->draw();
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (void)layoutSubviews {
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self renderScene];
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (GLuint)loadTexture:(NSString *)fileName {
	Texture2D *tex = [[Texture2D alloc] initWithImage: [UIImage imageNamed:fileName]];
	GLuint texRef = [tex name];
	[tex dealloc];
	return texRef;
}


- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(mainGameLoop) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	centralControl->touchesBegan(touchPoint);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	centralControl->touchesMoved(touchPoint);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	centralControl->touchesEnded(touchPoint);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	centralControl->touchesCancelled(touchPoint);
}

@end


