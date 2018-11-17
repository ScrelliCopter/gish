cmake_minimum_required(VERSION 3.12)
project(gish C)

include(FindPkgConfig)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
pkg_check_modules(VORBISFILE vorbisfile REQUIRED)

add_definitions(${SDL_CFLAGS})
add_definitions(${OPENGL_CFLAGS})
add_definitions(${OPENAL_CFLAGS})

include_directories(${SDL_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${OPENAL_INCLUDE_DIRS})
include_directories(src)

set(SOURCES
	src/main.c)

add_executable(gish ${SOURCES})
target_link_libraries(gish
	${SDL_LIBRARY}
	${OPENAL_LIBRARY}
	${OPENGL_LIBRARIES}
	${VORBISFILE_LIBRARIES}
	m)
