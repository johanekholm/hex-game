#include "EAGLView.h"
#include "HexMap.h"
#include "TextureMap.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "CentralControl.h"
#include <QMouseEvent>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

class EAGLView::PrivateData {
public:
	HexMap *hexMap;
	UnitModel *unit;
	UnitView *unitView;
	InputManager *input;
	CentralControl* centralControl;
	QTimer timer;
	qint64 lastTime;
};

EAGLView::EAGLView(QWidget *parent)
:QGLWidget(parent)
{
	d = new PrivateData;
	d->timer.setInterval(1000.0 / 60.0);
	connect(&d->timer, SIGNAL(timeout()), this, SLOT(mainGameLoop()));
}

EAGLView::~EAGLView() {
	delete d->centralControl;
	delete d->unit;
	delete d->unitView;
	delete d->hexMap;
	delete d->input;
	delete d;
	TextureCatalog::instance()->destroy();
}

void EAGLView::mainGameLoop() {
	qint64		time;
	float				delta;
	time = QDateTime::currentMSecsSinceEpoch();
	delta = (time - d->lastTime);
	
	d->centralControl->update();
	
//	[self updateScene:delta];
	this->updateGL();

	d->lastTime = time;

}
	
void EAGLView::paintGL () {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glLoadIdentity();
	
	d->centralControl->draw();
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
	catalog->addAndLoad("actions", loadTexture("actions.png"), 4);
	catalog->addAndLoad("icons", loadTexture("icons.png"), 4);

	
	d->hexMap = new HexMap(catalog->get("hexTiles"), 4, 4, 80.0f, 80.0f);
	
	d->unit = new UnitModel(1, 1);
	d->unitView = new UnitView(64.0f, 64.0f, catalog->get("units"), 0);
	d->unit->registerView(d->unitView);
	
	d->input = new InputManager();
	
	d->centralControl = CentralControl::setup(d->input, d->unit, d->unitView, d->hexMap);
	d->timer.start();
}

void EAGLView::mouseMoveEvent ( QMouseEvent * event ) {
	d->input->touchesMoved(GPointMake(event->x(), event->y()));
}

void EAGLView::mousePressEvent ( QMouseEvent * event ) {
	d->input->touchesBegan(GPointMake(event->x(), event->y()));
}

void EAGLView::mouseReleaseEvent ( QMouseEvent * event ) {
	d->input->touchesEnded(GPointMake(event->x(), event->y()));
}

GLuint EAGLView::loadTexture(const std::string &filename) {
	QString path = QString(":/Resources/") + filename.c_str();
	return bindTexture(QPixmap(path), GL_TEXTURE_2D, GL_RGBA, QGLContext::NoBindOption);	
}
