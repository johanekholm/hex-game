CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT( HexGame )

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

INCLUDE_DIRECTORIES( Toolkit )
INCLUDE_DIRECTORIES( Classes )

SET(TARGET_PLATFORM "" CACHE STRING "Sets the target platform. Valid values are 'iOS' or 'Meego'")
IF (NOT TARGET_PLATFORM)
	MESSAGE(FATAL_ERROR "Please set the target platform with -DTARGET_PLATFORM='platform'")
ELSE (NOT TARGET_PLATFORM)
	STRING(TOUPPER ${TARGET_PLATFORM} UPLATFORM)
	ADD_DEFINITIONS( -DPLATFORM_${UPLATFORM} )
	INCLUDE( ${TARGET_PLATFORM}/rules.cmake NO_POLICY_SCOPE )
ENDIF(NOT TARGET_PLATFORM)

ADD_EXECUTABLE(HexGame MACOSX_BUNDLE
	${SOURCES}
	${HEADERS}
)

TARGET_LINK_LIBRARIES( HexGame ${LIBRARIES} )
