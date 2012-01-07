#include "EAGLView.h"
#include "TextureCatalog.h"
#include "ResourceLoader.h"
#include "CentralControl.h"
#include <QMouseEvent>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

class EAGLView::PrivateData {
public:
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
	this->setWindowTitle("HexGame");
	this->resize(320, 480);
}

EAGLView::~EAGLView() {
	delete d->centralControl;
	delete d;
	TextureCatalog::destroy();
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
	
    TextureCatalog::instance();

    ResourceLoader resourceLoader(this);
    resourceLoader.load();

	d->centralControl = CentralControl::instance();
	d->timer.start();
}

void EAGLView::mouseMoveEvent ( QMouseEvent * event ) {
	d->centralControl->touchesMoved(GPointMake(event->x(), event->y()));
}

void EAGLView::mousePressEvent ( QMouseEvent * event ) {
	d->centralControl->touchesBegan(GPointMake(event->x(), event->y()));
}

void EAGLView::mouseReleaseEvent ( QMouseEvent * event ) {
	d->centralControl->touchesEnded(GPointMake(event->x(), event->y()));
}
