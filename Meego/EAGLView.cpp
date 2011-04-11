#include "EAGLView.h"
#include "HexMap.h"
#include "TextureMap.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include <QDebug>

class EAGLView::PrivateData {
public:
	HexMap *hexMap;
	UnitModel *unit;
	UnitView *unitView;
	InputManager *input;
};

EAGLView::EAGLView(QWidget *parent)
:QGLWidget(parent)
{
	d = new PrivateData;
}

EAGLView::~EAGLView() {
	delete d->unit;
	delete d->unitView;
	delete d->hexMap;
	delete d->input;
	delete d;
	TextureCatalog::instance()->destroy();
}
	
void EAGLView::paintGL () {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glLoadIdentity();
	
	d->hexMap->draw();
	d->unitView->draw();

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
	
	TextureCatalog* catalog = TextureCatalog::instance();
	
	catalog->addAndLoad("units", loadTexture("texmap.png"), 2);
	catalog->addAndLoad("hexTiles", loadTexture("texmap_hex.png"), 2);
	
	d->hexMap = new HexMap(catalog->get("hexTiles"), 4, 4, 80.0f, 80.0f);
	
	d->unit = new UnitModel(0, 0);
	d->unitView = new UnitView(64.0f, 64.0f, catalog->get("units"), 0);
	d->unit->registerView(d->unitView);
	
	d->input = new InputManager();
}

GLuint EAGLView::loadTexture(const std::string &filename) {
	QString path = QString("/Users/micke/Documents/dev/hex-game/Resources/") + filename.c_str();
	return bindTexture(QPixmap(path), GL_TEXTURE_2D, GL_RGBA, QGLContext::NoBindOption);	
}