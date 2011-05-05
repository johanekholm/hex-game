LIST(APPEND SOURCES
	Android/main.cpp
	Android/android_native_app_glue.c
)

LIST(APPEND HEADERS
	Android/android_native_app_glue.h
)

LIST(APPEND LIBRARIES
	android
	EGL
	GLESv1_CM
	log	
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
