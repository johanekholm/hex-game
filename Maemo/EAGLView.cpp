#include "EAGLView.h"
#include "HexMap.h"
#include "TextureMap.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "CentralControl.h"

#include "SDL_image.h"
#include "toolkit.h"

#include <stdio.h>

class EAGLView::PrivateData {
public:
	HexMap *hexMap;
	UnitModel *unit;
	UnitView *unitView;
	InputManager *input;
	CentralControl* centralControl;
	SDL_Surface *surface;
	int videoFlags;
};

EAGLView::EAGLView()
{
	d = new PrivateData;
	
	/* this holds some info about our display */
	const SDL_VideoInfo *videoInfo;

	SDL_Init( SDL_INIT_VIDEO );

	d->surface = SDL_SetVideoMode(0, 0, 16, SDL_SWSURFACE | SDL_FULLSCREEN);

	SDL_ShowCursor(0);

	SDL_WM_SetCaption("HexGame", "HexGame");

	SDL_GLES_Init(SDL_GLES_VERSION_1_1);

	SDL_GLES_Context *context = SDL_GLES_CreateContext();
	SDL_GLES_MakeCurrent(context);

	/* Enable smooth shading */
	glShadeModel( GL_SMOOTH );

	/* Set the background black */
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );

	/* Really Nice Perspective Calculations */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	/* Use of vertex arrays is mandatory in GLES */
	glEnableClientState(GL_VERTEX_ARRAY);

	resizeWindow( 800, 480 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	TextureCatalog* catalog = TextureCatalog::instance();

	catalog->addAndLoad("units", loadTexture("texmap.png"), 2);
	catalog->addAndLoad("hexTiles", loadTexture("texmap_hex.png"), 2);
	catalog->addAndLoad("actions", loadTexture("actions.png"), 4);
	catalog->addAndLoad("icons", loadTexture("icons.png"), 4);

	d->hexMap = new HexMap(catalog->get("hexTiles"), 4, 4, 80.0f, 80.0f);

	d->unit = new UnitModel(1, 1);
	d->unitView = new UnitView(64.0f, 64.0f, catalog->get("units"), 0);
	d->unit->registerView(d->unitView);

	d->input = new InputManager();

	d->centralControl = CentralControl::setup(d->input, d->unit, d->unitView, d->hexMap);
}

EAGLView::~EAGLView() {
	delete d;
}

void EAGLView::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glLoadIdentity();

	d->centralControl->draw();

	/* Draw it to the screen */
	SDL_GLES_SwapBuffers( );
}

int EAGLView::resizeWindow( int width, int height ) {
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if ( height == 0 ) {
		height = 1;
	}

	ratio = ( GLfloat )width / ( GLfloat )height;

	/* Setup our viewport. */
	glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, width, height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);

	/* Reset The View */
	glLoadIdentity( );

	return true;
}

int EAGLView::run() {
	SDL_Event event;

	while(1) {

		while ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_VIDEORESIZE:
					SDL_SetVideoMode( event.resize.w, event.resize.h, 16, d->videoFlags );
					break;
				case SDL_MOUSEMOTION:
					d->input->touchesMoved(GPointMake(event.motion.x, event.motion.y));
					break;
				case SDL_MOUSEBUTTONDOWN:
					d->input->touchesBegan(GPointMake(event.button.x, event.button.y));
					break;
				case SDL_MOUSEBUTTONUP:
					d->input->touchesEnded(GPointMake(event.button.x, event.button.y));
					break;
				case SDL_QUIT:
					return 0;
// 				default:
// 					printf("default %i\n", event.type);
			}
		}
		draw();
		usleep(1000);
	}

	return 0;
}

GLuint EAGLView::loadTexture(const std::string &filename) {
	GLuint texture;
	SDL_Surface *surface = IMG_Load( filename.c_str() );

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	if (surface->format->BytesPerPixel == 4) {
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
	} else {
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels );
	}

	/* Linear Filtering */
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	/* Free up any memory we may have used */
	SDL_FreeSurface( surface );
	return texture;
}
