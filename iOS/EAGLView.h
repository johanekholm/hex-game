//
//  EAGLView.h
//  Tutorial1
//
//  Created by Michael Daley on 25/02/2009.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//


//#include <QGLWidget>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "Texture2D.h"
#import "TextureMap.h"
#import "GameImage.h"
#import	"TileMap.h"

class HexMap;
class UnitModel;
class UnitView;
class InputManager;
class CentralControl;

/*
 This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
 The view content is basically an EAGL surface you render your OpenGL scene into.
 Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
 */
@interface EAGLView : UIView {
/*
	Q_OBJECT
public:
	EAGLView(QWidget *parent = 0);
	~EAGLView();
	
protected:
	void paintGL ();
	void initializeGL ();
	
	GLuint loadTexture(const std::string &filename);
private:
	class PrivateData;
	PrivateData *d;
*/
 
@private
    /* The pixel dimensions of the backbuffer */
    GLint backingWidth;
    GLint backingHeight;
    
    EAGLContext *context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;
    
    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;
    
    NSTimer *animationTimer;
    NSTimeInterval animationInterval;
	
	CFTimeInterval lastTime;

	UnitModel* unit;
	UnitView* unitView;
	HexMap *hexMap;
	
	InputManager *input;
	CentralControl* centralControl;
	
};

@property NSTimeInterval animationInterval;

- (void)startAnimation;
- (void)stopAnimation;
- (void)renderScene;
//- (void)loadTexture:(NSString *)name intoLocation:(GLuint)location;

@end
