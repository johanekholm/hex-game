PROJECT( HexGame )

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

SET(PRODUCT_NAME HexGame)

SET( SOURCES
	Classes/AbstractResourceLoader.cpp
	Classes/Action.cpp
    Classes/BaseUnitViewController.cpp
	Classes/CentralControl.cpp
    Classes/EventManager.cpp
	Classes/GameImage.cpp
	Classes/geometry.cpp
	Classes/HexMap.cpp
    Classes/HexMapModel.cpp
	Classes/InputManager.cpp
    Classes/Item.cpp
    Classes/jsoncpp.cpp
    Classes/MapObject.cpp
    Classes/MapObjectView.cpp
    Classes/MenuView.cpp
    Classes/MessageView.cpp
    Classes/ModelManager.cpp
    Classes/Observable.cpp
    Classes/SceneLoader.cpp
    Classes/ScriptManager.cpp
    Classes/ShapeImage.cpp
	Classes/Sound.cpp
    Classes/StringImage.cpp
	Classes/TextureCatalog.cpp
	Classes/TextureMap.cpp
    Classes/TransitionViewController.cpp
    Classes/UnitFactory.cpp
	Classes/UnitModel.cpp
	Classes/UnitView.cpp
	Classes/ViewControllerManager.cpp
	Classes/ViewController.cpp
	Toolkit/toolkit.cpp
)

SET( HEADERS
	Classes/AbstractResourceLoader.h
	Classes/Action.h
    Classes/BaseUnitViewController.h
	Classes/CentralControl.h
    Classes/ControlCallback.h
    Classes/EventManager.h
    Classes/GameImage.h
	Classes/geometry.h
	Classes/HexMap.h
    Classes/HexMapModel.h
	Classes/InputManager.h
    Classes/Item.h
    Classes/IObserver.h
    Classes/json.h
    Classes/json-forwards.h
    Classes/MapObject.h
    Classes/MapObjectView.h
    Classes/MenuView.h
    Classes/MessageView.h
    Classes/ModelManager.h
    Classes/SceneLoader.h
    Classes/ScriptManager.h
    Classes/ShapeImage.h
    Classes/Sound.h
    Classes/StringImage.h
	Classes/TextureCatalog.h
	Classes/TextureMap.h
    Classes/TransitionViewController.h
    Classes/UnitFactory.h
	Classes/UnitModel.h
	Classes/UnitView.h
	Classes/ViewControllerManager.h
	Classes/ViewController.h
	Classes/Observable.h
	Toolkit/toolkit.h
)

SET(RESOURCES
	Resources/actions.png
	Resources/floortilemap.png
    Resources/font_1.png
    Resources/game_objects.png
	Resources/texmap.png
	Resources/texmap_hex.png
	Resources/icons.png
)

SET(SOUNDS
	Resources/fireball1.mp3
	Resources/slash1.mp3
    Resources/grotta.mp3
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

ADD_SUBDIRECTORY( lua )

IF (${TARGET_PLATFORM} MATCHES "Android")
	ADD_LIBRARY(HexGame SHARED
		${SOURCES}
		${HEADERS}
		${RESOURCES}
		${SOUNDS}
	)
ELSE ()
	ADD_EXECUTABLE(HexGame MACOSX_BUNDLE
		${SOURCES}
		${HEADERS}
		${RESOURCES}
		${SOUNDS}
	)
ENDIF ()	

SET_SOURCE_FILES_PROPERTIES(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

IF (COMMAND COMPILE)
	COMPILE(HexGame)
ENDIF(COMMAND COMPILE)

TARGET_LINK_LIBRARIES( HexGame ${LIBRARIES} lua )
