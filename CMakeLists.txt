cmake_minimum_required(VERSION 3.12)
project(gish C)
find_package(PkgConfig REQUIRED)

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
	src/sdl/globbing.c
	src/sdl/globbing.h
	src/sdl/physfsrwops.c
	src/sdl/physfsrwops.h
	src/sdl/platform.h
	src/sdl/video.c
	src/sdl/video.h

	src/video/glext.h
	src/video/glfunc.c
	src/video/glfunc.h
	src/video/text.c
	src/video/text.h
	src/video/texture.c
	src/video/texture.h

	src/config.h
	src/main.c)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
pkg_check_modules(PHYSFS physfs)
if (NOT PHYSFS_FOUND)
	find_package(PhysFS REQUIRED)
endif()
pkg_check_modules(VORBISFILE REQUIRED vorbisfile)

add_executable(gish ${SOURCES})
target_include_directories(gish PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${SDL2_INCLUDE_DIRS}
	${PHYSFS_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${OPENAL_INCLUDE_DIRS}
	${VORBISFILE_INCLUDE_DIRS})
target_link_directories(gish PRIVATE
	${SDL2_LIBRARY_DIRS}
	${PHYSFS_LIBRARY_DIRS}
	${OPENAL_LIBRARY_DIRS}
	${VORBISFILE_LIBRARY_DIRS})
target_link_libraries(gish
	${SDL2_LIBRARIES}
	${PHYSFS_LIBRARY}
	${OPENAL_LIBRARY}
	${OPENGL_LIBRARIES}
	${VORBISFILE_LIBRARIES}
	m)
