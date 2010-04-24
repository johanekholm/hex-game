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
#import "InputState.h"

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
    
	delete input;
	
	//[board release];
	//[sprite release];
	
	delete sprite;
	
	[player release];
	//[texMap release];
	delete texMap;
	
	
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
		
		// init images
		player = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"hero1.png"]];
		
		//texMap = [[TextureMap alloc] initWithSubdivisions:2 ofImage:[UIImage imageNamed:@"texmap.png"]];
		
		//texMap = [[TextureMap alloc] initWithImage:[UIImage imageNamed:@"texmap.png"]];
		//[texMap setNumSubdivisions:2];
		
		texMap = new TextureMap("texmap.png", 2);
		
		
		//sprite = [[GameImage alloc] initWithSize: CGSizeMake(64.0f, 64.0f) andTexture:texMap withIndex:1];
		
		//board = [[TileMap alloc] initWithMapWidth:2 andMapHeight:2 withTileSize:CGSizeMake(64.0f, 64.0f) andTexture:texMap];
		input = new InputManager();
		
		sprite = new GameImage(64.0f, 64.0f, texMap, 1);
		sprite->setPosition(GPointMake(250.0f, 250.0f));
    }
    return self;
}


- (void)mainGameLoop {
	CFTimeInterval		time;
	float				delta;
	time = CFAbsoluteTimeGetCurrent();
	delta = (time - lastTime);
	
	input->update();
	
	if (input->isButtonPressed()) {
		sprite->setPosition(input->currentState()->touchLocation());

		//xPos = input.currentState.touchLocation.x;
		//yPos = 480 - input.currentState.touchLocation.y;
		//yPos = input.currentState.touchLocation.y;
	}
	
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

	
	/*GLfloat		coordinates[] = {	0.0f,	1.0f,
									1.0f,	1.0f,
									0.0f,	0.0f,
									1.0f,	0.0f };

	GLfloat		vertices[] = {	-32.0f, -32.0f, 0.0f,
								32.0f, -32.0f, 0.0f,
								-32.0f, 32.0f, 0.0f,
								32.0f, 32.0f, 0.0f };
	
	GLfloat	texCoords[8];
	
	[texMap getTexCoordsForSubWithIndex:0 into:texCoords];
	
	glLoadIdentity();
	//glTranslatef(xPos, yPos, 0.0f);
	glTranslatef(32.0f, 32.0f, 0.0f);

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	//[player bind];
	[texMap bind];
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	*/
	
	//[board draw];
	//[sprite draw];
	
	sprite->draw();
	
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
	input->touchesBegan(touches, event, self, animationTimer);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	input->touchesMoved(touches, event, self, animationTimer);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	input->touchesEnded(touches, event, self, animationTimer);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	input->touchesCancelled(touches, event, self, animationTimer);
}



@end


// Kan-va-bra-att-komma-ihåg-kod:
 
 /*
 output = [[Texture2D alloc] initWithString:@"This is my output" dimensions:CGSizeMake(256.0f, 16.0f) alignment:UITextAlignmentLeft fontName:@"Courier" fontSize:12.0f];
 output2 = [[Texture2D alloc] initWithString:@"You touched me!" dimensions:CGSizeMake(256.0f, 16.0f) alignment:UITextAlignmentLeft fontName:@"Courier" fontSize:12.0f];
 */


 
 /*
 GLuint  texture[1];      // Storage For One Texture ( NEW ) 
 glGenTextures(1, &texture[0]);

 glBindTexture(GL_TEXTURE_2D, texture[0]);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
 NSString *path = [[NSBundle mainBundle] pathForResource:@"hero1" ofType:@"pvr"];
 NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
 // Instead of glTexImage2D, we have to use glCompressedTexImage2D
 //glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, 64.0, 64.0, 0, (64.0 * 64.0) / 2, [texData bytes]);
 glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, 64.0, 64.0, 0, [texData length], [texData bytes]);
 [texData release];
 
 
 
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
 */


