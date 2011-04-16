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
#include "UnitModel.h"
#include "UnitView.h"

#include "toolkit_iphone.h"
#include "HexMap.h"
#include "TextureCatalog.h"
#include "CentralControl.h"

#define USE_DEPTH_BUFFER 0

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;
- (void) updateScene:(float)delta;
- (void) renderScene;

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
	delete input;
	
	delete unit;
	delete unitView;
	delete hexMap;
	
	TextureCatalog::instance()->destroy();
		
	
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
		
		
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		// Enable blending
		//glEnable(GL_BLEND);
		
				
		/*int boardData[150] = {	7,3,3,3,7,3,3,3,3,8,
								6,0,0,0,0,0,0,5,0,4,
								2,0,0,0,0,0,0,0,0,8,
								2,0,0,7,0,0,0,0,0,4,
								2,0,0,0,0,0,0,0,8,4,
								2,5,0,0,0,0,0,0,0,4,
								2,0,0,0,0,0,6,0,0,4,
								2,0,0,0,0,0,0,0,0,4,
								2,0,0,0,0,0,0,0,0,4,
								2,0,0,0,0,0,0,0,0,4,
								2,0,5,0,0,0,0,0,0,8,
								2,0,0,0,0,0,0,7,0,4,
								7,0,0,6,0,0,0,0,0,4,
								2,0,0,0,0,0,0,0,8,4,
								6,1,1,1,1,1,6,1,1,5};
		
		vector<int> vData(150);
		vData.assign(boardData, boardData + 150);
		
		//board = new TileMap(10, 15, 32.0f, 32.0f, boardTexMap, &vData);
		*/
		
		TextureCatalog* catalog = TextureCatalog::instance();
		
		catalog->addAndLoad("units", "texmap.png", 2);
		catalog->addAndLoad("hexTiles", "texmap_hex.png", 2);
		catalog->addAndLoad("actions", "actions.png", 4);
		
		hexMap = new HexMap(catalog->get("hexTiles"), 4, 4, 80.0f, 80.0f);
		
		unit = new UnitModel(1, 1);
		unitView = new UnitView(64.0f, 64.0f, catalog->get("units"), 0);
		unit->registerView(unitView);
				
		input = new InputManager();
		
		centralControl = CentralControl::setup(input, unit, unitView, hexMap);
		
    }
    return self;
}


- (void)mainGameLoop {
	CFTimeInterval		time;
	float				delta;
	time = CFAbsoluteTimeGetCurrent();
	delta = (time - lastTime);

	
	centralControl->update();
	
/*	int action = 0;
	TouchEvent event;
	// TO-DO: change to member in control class
	static UnitView* _selectedUnit = 0;
	
	if (input->hasEvent()) {

		event = input->popEvent();
		
		switch (1) {
		case 1:
			if (event.type == 1) {
				if (unitView->wasTouched(event.point)) {
					_selectedUnit = unitView;
					//this->switchToMode("focus");
					NSLog(@"unit selected");
				}
			}
				
			if (event.type == 3) {
				if ((action = unitView->touchedAction(event.point)) > -1) {
					NSLog(@"action: %i", action);
					
					unit->doAction(action);
				}
			}
			break;
						
		case 2:
						
			break;
						
		}
	}*/
						
/*	if (input->wasClicked()) {
		if ((action = unitView->touchedAction(input->clickPoint())) > -1) {
			NSLog(@"action: %i", action);

			unit->doAction(action);
			//unit->move(1);
		}
	}
*/	
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
	
/*	hexMap->draw();
	unitView->draw();
	unitView->drawActions();
*/	
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
	input->touchesBegan(touchPoint);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	input->touchesMoved(touchPoint);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	input->touchesEnded(touchPoint);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	NSLog(@"Cancelled, crashing!");
	GPoint touchPoint = GPointFromCGPoint([[touches anyObject] locationInView:self]);
	input->touchesCancelled(touchPoint);
}




@end


