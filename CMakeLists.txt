CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT( HexGame )

SET(PRODUCT_NAME HexGame)

SET( SOURCES
	Classes/Action.cpp
	Classes/CentralControl.cpp
	Classes/GameImage.cpp
	Classes/geometry.cpp
	Classes/HexMap.cpp
	Classes/InputManager.cpp
	Classes/RobotModel.cpp
	Classes/RobotView.cpp
	Classes/TextureCatalog.cpp
	Classes/TextureMap.cpp
	Classes/UnitModel.cpp
	Classes/UnitView.cpp
	Toolkit/toolkit.cpp
)

SET( HEADERS
	Classes/Action.h
	Classes/CentralControl.h
	Classes/GameImage.h
	Classes/geometry.h
	Classes/HexMap.h
	Classes/InputManager.h
	Classes/IRobotView.h
	Classes/RobotModel.h
	Classes/RobotView.h
	Classes/TextureCatalog.h
	Classes/TextureMap.h
	Classes/UnitModel.h
	Classes/UnitView.h
	Toolkit/toolkit.h
)

SET(RESOURCES
	Resources/floortilemap.png
	Resources/texmap.png
	Resources/texmap_hex.png
	Resources/icons.png
	Resources/actions.png
)


INCLUDE_DIRECTORIES( Toolkit )
INCLUDE_DIRECTORIES( Classes )

SET(TARGET_PLATFORM "" CACHE STRING "Sets the target platform. Valid values are 'iOS', 'Android' or 'Meego'")
IF (NOT TARGET_PLATFORM)
	MESSAGE(FATAL_ERROR "Please set the target platform with -DTARGET_PLATFORM='platform'")
ELSE (NOT TARGET_PLATFORM)
	STRING(TOUPPER ${TARGET_PLATFORM} UPLATFORM)
	ADD_DEFINITIONS( -DPLATFORM_${UPLATFORM} )
	INCLUDE( ${TARGET_PLATFORM}/rules.cmake NO_POLICY_SCOPE )
	IF (COMMAND CONFIGURE)
		CONFIGURE()
	ENDIF(COMMAND CONFIGURE)
ENDIF(NOT TARGET_PLATFORM)

IF (${TARGET_PLATFORM} MATCHES "Android")
	ADD_LIBRARY(HexGame SHARED
		${SOURCES}
		${HEADERS}
		${RESOURCES}
	)
ELSE (${TARGET_PLATFORM} MATCHES "Android")
	ADD_EXECUTABLE(HexGame MACOSX_BUNDLE
		${SOURCES}
		${HEADERS}
		${RESOURCES}
	)
ENDIF (${TARGET_PLATFORM} MATCHES "Android")	

SET_SOURCE_FILES_PROPERTIES(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

IF (COMMAND COMPILE)
	COMPILE(HexGame)
ENDIF(COMMAND COMPILE)

TARGET_LINK_LIBRARIES( HexGame ${LIBRARIES} )
