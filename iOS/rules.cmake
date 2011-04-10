
LIST(APPEND SOURCES
	iOS/main.m
	iOS/EAGLView.mm
	iOS/OGLGameAppDelegate.mm
	iOS/main.m
	Toolkit/toolkit_ios.mm
)

LIST(APPEND HEADERS
	iOS/EAGLView.h
	iOS/OGLGameAppDelegate.h
	Toolkit/toolkit_ios.h
)

LIST(APPEND RESOURCES
	iOS/MainWindow.xib
)

LIST(APPEND LIBRARIES
	"-framework Foundation -framework UIKit -framework CoreGraphics -framework OpenGLES -framework QuartzCore"
)

SET(SDK_VERSION "iphoneos4.0" CACHE STRING "The sdk version")
SET(CMAKE_OSX_SYSROOT ${SDK_VERSION})
SET(CMAKE_OSX_ARCHITECTURES i386 armv6 armv7)
SET(CMAKE_CXX_FLAGS "-x objective-c++")
SET(APP_TYPE MACOSX_BUNDLE)
