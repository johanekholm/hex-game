
LIST(APPEND SOURCES
	iOS/main.m
	iOS/EAGLView.mm
	iOS/OGLGameAppDelegate.mm
	iOS/ResourceLoader.mm
	iOS/main.m
	iOS/Texture2D.m
	iOS/Sound.mm
	Toolkit/toolkit_ios.mm
)

LIST(APPEND HEADERS
	iOS/EAGLView.h
	iOS/OGLGameAppDelegate.h
	iOS/ResourceLoader.h
	iOs/Texture2D.h
	Toolkit/toolkit_ios.h
)

LIST(APPEND RESOURCES
	iOS/MainWindow.xib
)

FOREACH(sound ${SOUNDS})
	GET_FILENAME_COMPONENT(soundName ${sound} NAME_WE)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/sounds/${soundName}.caf
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/sounds
			COMMAND afconvert -f caff -d LEI16 ${CMAKE_SOURCE_DIR}/${sound} ${CMAKE_BINARY_DIR}/sounds/${soundName}.caf
			DEPENDS ${sound}
			COMMENT "Encode ${sound}"
	)
	LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/sounds/${soundName}.caf)
	SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/sounds/${soundName}.caf PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDFOREACH()


LIST(APPEND LIBRARIES
	"-framework Foundation -framework UIKit -framework CoreGraphics -framework OpenGLES -framework QuartzCore -framework AVFoundation"
)

SET(SDK_VERSION "iphoneos4.1" CACHE STRING "The sdk version")
SET(CMAKE_OSX_SYSROOT ${SDK_VERSION})
SET(CMAKE_OSX_ARCHITECTURES i386 armv6 armv7)
SET(CMAKE_CXX_FLAGS "-x objective-c++")
SET(APP_TYPE MACOSX_BUNDLE)


FUNCTION(COMPILE target)
	SET_TARGET_PROPERTIES(${target} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/iOS/Info.plist)
ENDFUNCTION(COMPILE)
