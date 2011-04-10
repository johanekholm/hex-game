FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTOPENGL TRUE)
INCLUDE( ${QT_USE_FILE} )

FIND_PACKAGE(OpenGL REQUIRED)

LIST(APPEND SOURCES
	Meego/main.cpp
	Meego/EAGLView.cpp
)

LIST(APPEND HEADERS
)

SET( MOC_HEADERS
	Meego/EAGLView.h
)

QT4_WRAP_CPP(  MOC_SOURCES  ${MOC_HEADERS} )
QT4_AUTOMOC (  ${SOURCES} )

LIST(APPEND LIBRARIES
	${QT_LIBRARIES}
	${OPENGL_gl_LIBRARY}
)

LIST(APPEND HEADERS ${MOC_HEADERS})
LIST(APPEND HEADERS ${MOC_SOURCES})
