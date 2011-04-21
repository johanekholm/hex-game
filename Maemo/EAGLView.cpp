#include "EAGLView.h"
#include "HexMap.h"
#include "TextureMap.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "CentralControl.h"

#include "SDL_image.h"

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

	videoInfo = SDL_GetVideoInfo( );

	d->videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	d->videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	d->videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	d->videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if ( videoInfo->hw_available ) {
		d->videoFlags |= SDL_HWSURFACE;
	} else {
		d->videoFlags |= SDL_SWSURFACE;
	}

	/* This checks if hardware blits can be done */
	if ( videoInfo->blit_hw ) {
		d->videoFlags |= SDL_HWACCEL;
	}

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* get a SDL surface */
	d->surface = SDL_SetVideoMode( 640, 480, 16, d->videoFlags );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glDisable(GL_DEPTH_TEST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	SDL_GL_SwapBuffers( );

}

int EAGLView::run() {
	SDL_Event event;

	while(1) {

		while ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_VIDEORESIZE:
					SDL_SetVideoMode( event.resize.w, event.resize.h, 16, d->videoFlags );
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
	std::string path = "/home/micke/Documents/dev/hex-game/Resources/";
	path.append(filename);
	SDL_Surface *surface = IMG_Load( path.c_str() );

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels );

	/* Linear Filtering */
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	/* Free up any memory we may have used */
	SDL_FreeSurface( surface );
	return texture;
}
