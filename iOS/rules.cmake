
LIST(APPEND SOURCES
)

LIST(APPEND HEADERS
)

LIST(APPEND LIBRARIES
	"-framework Foundation -framework UIKit -framework CoreGraphics"
)

SET(SDK_VERSION "iphoneos4.0" CACHE STRING "The sdk version")
SET(CMAKE_OSX_SYSROOT ${SDK_VERSION})
SET(CMAKE_OSX_ARCHITECTURES i386 armv6 armv7)
SET(CMAKE_CXX_FLAGS "-x objective-c++")
SET(APP_TYPE MACOSX_BUNDLE)
