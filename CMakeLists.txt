cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE( Qt4 REQUIRED )
SET(QT_USE_QTOPENGL TRUE)
INCLUDE( ${QT_USE_FILE} )

FIND_PACKAGE(OpenGL REQUIRED)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


SET( SOURCES
	Classes/EAGLView.cpp
	Classes/GameImage.mm
#	Classes/GameImageObjC.m
	Classes/HexMap.mm
	Classes/InputManager.mm
#	Classes/OGLGameAppDelegate.mm
	Classes/RobotModel.mm
	Classes/RobotView.mm
#	Classes/Texture2D.m
	Classes/TextureMap.mm
#	Classes/TileMap.mm
#	Classes/UnitModel.mm
	Toolkit/toolkit.mm
	main.cpp
)

SET( HEADERS
	Classes/GameImage.h
#	Classes/GameImageObjC.h
	Classes/HexMap.h
	Classes/InputManager.h
	Classes/IRobotView.h
#	Classes/OGLGameAppDelegate.h
	Classes/RobotModel.h
	Classes/RobotView.h
	Classes/Texture2D.h
	Classes/TextureMap.h
#	Classes/TileMap.h
#	Classes/UnitModel.h
	Toolkit/toolkit.h
)

SET( MOC_HEADERS
	Classes/EAGLView.h
)

INCLUDE_DIRECTORIES( Toolkit )

QT4_WRAP_CPP(  MOC_SOURCES  ${MOC_HEADERS} )
QT4_AUTOMOC (  ${SOURCES} )

SET( LIBRARIES
	${QT_LIBRARIES}
	${OPENGL_gl_LIBRARY}
)

ADD_EXECUTABLE(HexGame MACOSX_BUNDLE
	${SOURCES}
	${HEADERS}
	${MOC_HEADERS}
	${MOC_SOURCES}
)

TARGET_LINK_LIBRARIES( HexGame ${LIBRARIES} )
