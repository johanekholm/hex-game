SET(PYTHON_FOR_ANDROID_DIR "" CACHE DIR "The dir with python for Android")
INCLUDE_DIRECTORIES( ${PYTHON_FOR_ANDROID_DIR}/include/python2.6 )

LIST(APPEND SOURCES
	Android/AssetLoader.cpp
	Android/GameGLView.cpp
	Android/OpenGLRenderer.cpp
	Android/ResourceLoader.cpp
	Android/Sound.cpp
)

LIST(APPEND HEADERS
	Android/AssetLoader.h
	Android/ResourceLoader.h
)

LIST(APPEND LIBRARIES
	android
	EGL
	GLESv1_CM
	log
	OpenSLES
	${PYTHON_FOR_ANDROID_DIR}/lib/libpython2.6.so
)

SET(JAVASOURCES
	Android/HexGame.java
	Android/OpenGLRenderer.java
	Android/GameGLView.java
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

FOREACH(image ${RESOURCES})
	GET_FILENAME_COMPONENT(imageName ${image} NAME)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/apk/assets/${imageName}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${image} ${CMAKE_BINARY_DIR}/apk/assets/${imageName}
			DEPENDS ${image}
			COMMENT "Copying ${image}"
	)
	LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/apk/assets/${imageName})
ENDFOREACH()

FOREACH(sound ${SOUNDS})
	GET_FILENAME_COMPONENT(soundName ${sound} NAME)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/apk/assets/${soundName}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${sound} ${CMAKE_BINARY_DIR}/apk/assets/${soundName}
			DEPENDS ${sound}
			COMMENT "Copying ${sound}"
	)
	LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/apk/assets/${soundName})
ENDFOREACH()

FOREACH(script ${SCRIPTS})
	GET_FILENAME_COMPONENT(scriptName ${script} NAME)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/apk/assets/${scriptName}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${script} ${CMAKE_BINARY_DIR}/apk/assets/${scriptName}
			DEPENDS ${script}
			COMMENT "Copying ${script}"
	)
	LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/apk/assets/${scriptName})
ENDFOREACH()

FOREACH(javaSource ${JAVASOURCES})
	GET_FILENAME_COMPONENT(sourceName ${javaSource} NAME)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/apk/src/com/hexgame/game/${sourceName}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${javaSource} ${CMAKE_BINARY_DIR}/apk/src/com/hexgame/game/${sourceName}
			DEPENDS ${javaSource}
			COMMENT "Copying ${javaSource}"
	)
	LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/apk/src/com/hexgame/game/${sourceName})
ENDFOREACH()

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/apk/libs/${ARM_TARGET}/libpython2.6.so
  COMMAND ${CMAKE_COMMAND} -E copy ${PYTHON_FOR_ANDROID_DIR}/lib/libpython2.6.so ${CMAKE_BINARY_DIR}/apk/libs/${ARM_TARGET}/libpython2.6.so
  DEPENDS ${PYTHON_FOR_ANDROID_DIR}/lib/libpython2.6.so
  COMMENT "Copying python2.6.so"
)
LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/apk/libs/${ARM_TARGET}/libpython2.6.so)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Android/AndroidManifest.xml ${CMAKE_BINARY_DIR}/apk/AndroidManifest.xml)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Android/strings.xml ${CMAKE_BINARY_DIR}/apk/res/values/strings.xml)

FUNCTION(COMPILE target)
	ADD_CUSTOM_COMMAND(
			    TARGET ${target}
			    POST_BUILD
			    COMMAND android update project -p ${CMAKE_BINARY_DIR}/apk -t 2
			    COMMAND cd ${CMAKE_BINARY_DIR}/apk/ && ant debug
			)
ENDFUNCTION()
