cmake_minimum_required(VERSION 3.29)
project(jadeite LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(APP_SOURCE_DIR "${PROJECT_SRC_DIR}src")
set(DEPENDENCIES_DIR "${PROJECT_SRC_DIR}dependencies")
set(LUA_DIR "${DEPENDENCIES_DIR}/lua")
set(SOL_DIR "${DEPENDENCIES_DIR}/sol2/include")
set(BOX2D_SRC_DIR "${DEPENDENCIES_DIR}/box2d_2_4_0/src")

add_library(
	lua STATIC
	${LUA_DIR}/onelua.c
)

add_library(
	box2d STATIC
	${BOX2D_SRC_DIR}/onebox2d.cpp
)

target_include_directories(
	box2d PUBLIC
	"${DEPENDENCIES_DIR}/box2d_2_4_0/include"
	"${DEPENDENCIES_DIR}/box2d_2_4_0/src"
)

target_compile_definitions(lua PRIVATE MAKE_LIB)
target_compile_definitions(box2d PRIVATE MAKE_LIB)

add_compile_definitions(SOL_ALL_SAFETIES_ON=1)

# Jadite executable
file(GLOB_RECURSE MY_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
	${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

# Add to a target
add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_include_directories(
	${PROJECT_NAME}
	PRIVATE 
	${APP_SOURCE_DIR}
	${LUA_DIR}
	${SOL_DIR}
	${DEPENDENCIES_DIR}/glm
	${DEPENDENCIES_DIR}/entt
	${DEPENDENCIES_DIR}/stb
	${DEPENDENCIES_DIR}/box2d_2_4_0/include
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	lua
	box2d
)

if (EMSCRIPTEN)
target_compile_options(
		${PROJECT_NAME} 
		PRIVATE
        -O3
        -Wall
        -sUSE_SDL=2
		-sUSE_OGG=1
		-sUSE_VORBIS=1
		-sUSE_SDL_MIXER=2
		-sUSE_MPG123=1
		-sSDL2_MIXER_FORMATS=["wav","ogg","mp3"]
		-sUSE_SDL_IMAGE=2
		-sSDL2_IMAGE_FORMATS=[png]
    )

    target_link_options(
		${PROJECT_NAME} 
		PRIVATE
        -sUSE_SDL=2
		-sUSE_OGG=1
		-sUSE_VORBIS=1
		-sUSE_SDL_MIXER=2
		-sUSE_MPG123=1
		-sSDL2_MIXER_FORMATS=["wav","ogg","mp3"]
		-sUSE_SDL_IMAGE=2
		-sSDL2_IMAGE_FORMATS=[png]
		-sFULL_ES3=1
        -sALLOW_MEMORY_GROWTH=1
		-sFORCE_FILESYSTEM=1
    )

endif()

