
#include <QtGui>
#include "EAGLView.h"

int main(int argc, char *argv[])
{
	QApplication app( argc, argv );

	EAGLView l;
	l.show();
	
	return app.exec();
}
