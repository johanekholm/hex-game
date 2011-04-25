
LIST(APPEND SOURCES
	iOS/main.m
	iOS/EAGLView.mm
	iOS/OGLGameAppDelegate.mm
	iOS/main.m
	iOS/Texture2D.m
	Toolkit/toolkit_ios.mm
)

LIST(APPEND HEADERS
	iOS/EAGLView.h
	iOS/OGLGameAppDelegate.h
	iOs/Texture2D.h
	Toolkit/toolkit_ios.h
)

LIST(APPEND RESOURCES
	iOS/MainWindow.xib
)

LIST(APPEND LIBRARIES
	"-framework Foundation -framework UIKit -framework CoreGraphics -framework OpenGLES -framework QuartzCore"
)

SET(SDK_VERSION "iphoneos4.1" CACHE STRING "The sdk version")
SET(CMAKE_OSX_SYSROOT ${SDK_VERSION})
SET(CMAKE_OSX_ARCHITECTURES i386 armv6 armv7)
SET(CMAKE_CXX_FLAGS "-x objective-c++")
SET(APP_TYPE MACOSX_BUNDLE)


FUNCTION(COMPILE target)
	SET_TARGET_PROPERTIES(${target} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/iOS/Info.plist)
ENDFUNCTION(COMPILE)
