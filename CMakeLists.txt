CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


SET( SOURCES
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
	INCLUDE( ${TARGET_PLATFORM}/rules.cmake NO_POLICY_SCOPE )
ENDIF(NOT TARGET_PLATFORM)

ADD_EXECUTABLE(HexGame MACOSX_BUNDLE
	${SOURCES}
	${HEADERS}
)

TARGET_LINK_LIBRARIES( HexGame ${LIBRARIES} )
