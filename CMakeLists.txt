cmake_minimum_required(VERSION 3.10)
project(gish
	VERSION "1.53"
	HOMEPAGE_URL "https://github.com/ScrelliCopter/gish"
	LANGUAGES C)

option(DEBUG_MODE "Enable special debug features, even in release builds" OFF)
option(DEMO "Build demo version executable" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(SOURCES
	src/audio/audio.c
	src/audio/audio.h

	src/game/ai.c
	src/game/ai.h
	src/game/animation.c
	src/game/animation.h
	src/game/audio.c
	src/game/audio.h
	src/game/block.c
	src/game/block.h
	src/game/boss.c
	src/game/boss.h
	src/game/config.c
	src/game/config.h
	src/game/credits.c
	src/game/credits.h
	src/game/custom.c
	src/game/custom.h
	src/game/damage.c
	src/game/damage.h
	src/game/editor.c
	src/game/editor.h
	src/game/english.h
	src/game/game.c
	src/game/game.h
	src/game/gamemenu.c
	src/game/gamemenu.h
	src/game/glext.c
	src/game/glext.h
	src/game/high.c
	src/game/high.h
	src/game/level.c
	src/game/level.h
	src/game/lighting.c
	src/game/lighting.h
	src/game/logic.c
	src/game/logic.h
	src/game/mainmenu.c
	src/game/mainmenu.h
	src/game/mappack.c
	src/game/mappack.h
	src/game/music.c
	src/game/music.h
	src/game/object.c
	src/game/object.h
	src/game/objedit.c
	src/game/objedit.h
	src/game/objfunc.c
	src/game/objfunc.h
	src/game/options.c
	src/game/options.h
	src/game/physics.c
	src/game/physics.h
	src/game/player.c
	src/game/player.h
	src/game/prerender.c
	src/game/prerender.h
	src/game/random.c
	src/game/random.h
	src/game/record.c
	src/game/record.h
	src/game/render.c
	src/game/render.h
	src/game/replay.c
	src/game/replay.h
	src/game/ropeedit.c
	src/game/ropeedit.h
	src/game/setup.c
	src/game/setup.h
	src/game/socket.c
	src/game/socket.h
	src/game/sprite.c
	src/game/sprite.h
	src/game/texture.c
	src/game/texture.h
	src/game/utils.c
	src/game/utils.h
	src/game/vsmode.c
	src/game/vsmode.h

	src/input/joystick.c
	src/input/joystick.h
	src/input/keyboard.c
	src/input/keyboard.h
	src/input/mouse.c
	src/input/mouse.h

	src/math/intersec.c
	src/math/intersec.h
	src/math/math.h
	src/math/vector.c
	src/math/vector.h

	src/menu/menu.c
	src/menu/menu.h

	src/physics/bond.c
	src/physics/bond.h
	src/physics/object.c
	src/physics/object.h
	src/physics/particle.c
	src/physics/particle.h

	src/sdl/event.c
	src/sdl/event.h
	src/sdl/file.c
	src/sdl/file.h
	src/sdl/platform.c
	src/sdl/platform.h
	src/sdl/globbing.c
	src/sdl/globbing.h
	src/sdl/physfsrwops.c
	src/sdl/physfsrwops.h
    src/sdl/string.c
    src/sdl/string.h
	src/sdl/video.c
	src/sdl/video.h

	src/video/glext.h
	src/video/glfunc.c
	src/video/glfunc.h
	src/video/text.c
	src/video/text.h
	src/video/texture.c
	src/video/texture.h

	${CMAKE_CURRENT_BINARY_DIR}/src/config.h
	src/main.c)

configure_file(src/config.h.in src/config.h)


find_package(PkgConfig REQUIRED)
#TODO: proper find module for SDL2
# Quick & dirty implementation of a find module because no mac-friendly one exists
# and relying on package managers for absolutely everything is some lunix bullshit
if (APPLE)
	include(FindPackageHandleStandardArgs)

	find_path(SDL2_INCLUDE_DIR
		NAMES SDL.h
		HINTS
			ENV SDL2DIR
		PATHS
			~/Library/Frameworks
			/Library/Frameworks
		PATH_SUFFIXES SDL2)
	find_library(SDL2_LIBRARY
		NAMES SDL2 libSDL2
		HINTS
			ENV SDL2DIR
		PATHS
			~/Library/Frameworks
			/Library/Frameworks
		PATH_SUFFIXES lib)

	find_package_handle_standard_args(SDL2
		FOUND_VAR SDL2_FOUND
		REQUIRED_VARS
			SDL2_LIBRARY
			SDL2_INCLUDE_DIR
		VERSION_VAR SDL2_VERSION)
	if (SDL2_FOUND AND NOT TARGET SDL2::SDL2)
		if ("${SDL2_LIBRARY}" MATCHES "\\.framework$")
			add_library(SDL2::SDL2 INTERFACE IMPORTED)
			set_target_properties(SDL2::SDL2 PROPERTIES
				INTERFACE_LINK_LIBRARIES      "${SDL2_LIBRARY}"
				INTERFACE_COMPILE_OPTIONS     "${SDL2_DEFINITIONS}"
				INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}")
		else()
			add_library(SDL2::SDL2 UNKNOWN IMPORTED)
			set_target_properties(SDL2::SDL2 PROPERTIES
				IMPORTED_LOCATION             "${SDL2_LIBRARY}"
				INTERFACE_COMPILE_OPTIONS     "${SDL2_DEFINITIONS}"
				INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}")
		endif()
	endif()
endif()
if (NOT SDL2_FOUND)
	find_package(SDL2 REQUIRED)
endif()
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
pkg_check_modules(PHYSFS physfs)
if (NOT PHYSFS_FOUND)
	find_package(PhysFS REQUIRED)
endif()
find_package(Vorbis)

add_executable(gish ${SOURCES})
target_compile_definitions(gish PRIVATE $<$<OR:$<BOOL:${DEBUG_MODE}>,$<CONFIG:Debug>>:DEBUG>)
target_include_directories(gish PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/src
	${PHYSFS_INCLUDE_DIR} ${PhysFS_INCLUDE_DIRS}
	${OPENAL_INCLUDE_DIR})
target_link_libraries(gish
	SDL2::SDL2
	${PHYSFS_LIBRARY}
	${OPENAL_LIBRARY}
	OpenGL::GL
	Vorbis::vorbisfile
	m)
