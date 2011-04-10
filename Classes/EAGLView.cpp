#include "EAGLView.h"
#include "HexMap.h"
#include "TextureMap.h"
#include "RobotModel.h"
#include "RobotView.h"
#include "InputManager.h"
#include <QDebug>

class EAGLView::PrivateData {
public:
	TextureMap *hexTexMap, *texMap, *boardTexMap;
	HexMap *hexMap;
	RobotModel* robot;
	RobotView* robotView;	
	GameImage* sprite;
	InputManager *input;
};

EAGLView::EAGLView(QWidget *parent)
:QGLWidget(parent)
{
	d = new PrivateData;
}

EAGLView::~EAGLView() {
	delete d->hexMap;
	delete d->hexTexMap;
	delete d->texMap;
	delete d->boardTexMap;
	delete d->robot;
	delete d->robotView;
	delete d->sprite;
	delete d->input;
	delete d;
}
	
void EAGLView::paintGL () {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glLoadIdentity();
	
	d->hexMap->draw();	
	d->sprite->draw();
	d->robotView->draw();
}

void EAGLView::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, this->width(), this->height(), 0, -1, 1);
	
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
	
	d->texMap = new TextureMap(loadTexture("texmap.png"), 2);
	d->boardTexMap = new TextureMap(loadTexture("floortilemap.png"), 4);
	
	d->robot = new RobotModel(3, 7);
	d->robotView = new RobotView(64.0f, 64.0f, d->texMap, 0);
	d->robot->registerView(d->robotView);
	d->robotView->setModel(d->robot);
	
	//board = new TileMap(10, 15, 32.0f, 32.0f, boardTexMap, &vData);
	d->input = new InputManager();
	
	d->sprite = new GameImage(64.0f, 64.0f, d->texMap, 1);
	d->sprite->setPosition(GPointMake(250.0f, 250.0f));
	
	d->hexTexMap = new TextureMap(loadTexture("texmap_hex.png"), 2);
	d->hexMap = new HexMap(d->hexTexMap, 4, 4, 80.0f, 80.0f);
}

GLuint EAGLView::loadTexture(const std::string &filename) {
	QString path = QString("/Users/micke/Documents/dev/hex-game/Resources/") + filename.c_str();
	return bindTexture(QPixmap(path), GL_TEXTURE_2D, GL_RGBA, QGLContext::NoBindOption);	
}