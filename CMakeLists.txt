cmake_minimum_required(VERSION 3.6)

project(Othello)

set(OTHELLO_SOURCES
	src/main.cpp
	src/othello.cpp
	src/player.cpp
	src/server.cpp
	src/client.cpp
	src/ai.cpp
	src/board.cpp
)

set(OTHELLO_HEADERS
	src/main.h
	src/othello.h
	src/player.h
	src/server.h
	src/client.h
	src/ai.h
	src/board.h
	src/definet.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(OTHELLO_LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

if(WIN32)
	# SDL2 is assumed to be installed in libs/SDL2
	set(SDL2_INCLUDE_DIR "${OTHELLO_LIBS_DIR}/SDL2/include")
	
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
		set(SDL2_LIBRARY_DIR "${OTHELLO_LIBS_DIR}/SDL2/lib/x64")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32-bit
		set(SDL2_LIBRARY_DIR "${OTHELLO_LIBS_DIR}/SDL2/lib/x86")
	endif()
	
	set(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_DIR}/SDL2.lib")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(OpenGL REQUIRED)

# imgui
set(IMGUI_DIR "${OTHELLO_LIBS_DIR}/imgui")
set(IMGUI_SOURCES
	"${IMGUI_DIR}/imgui.cpp"
	"${IMGUI_DIR}/imgui_demo.cpp"
	"${IMGUI_DIR}/imgui_draw.cpp"
	"${IMGUI_DIR}/imgui_widgets.cpp"
	"${IMGUI_DIR}/examples/imgui_impl_opengl2.cpp"
	"${IMGUI_DIR}/examples/imgui_impl_sdl.cpp"
)
set(IMGUI_HEADERS
	"${IMGUI_DIR}/imconfig.h"
	"${IMGUI_DIR}/imgui.h"
	"${IMGUI_DIR}/imgui_internal.h"
	"${IMGUI_DIR}/imstb_rectpack.h"
	"${IMGUI_DIR}/imstb_textedit.h"
	"${IMGUI_DIR}/imstb_truetype.h"
)

include_directories(${IMGUI_DIR})
add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

set(LIBS ${SDL2_LIBRARY} imgui ${OPENGL_opengl_LIBRARY})
if(WIN32)
	list(APPEND LIBS opengl32)
endif()

add_executable(Othello ${OTHELLO_SOURCES} ${OTHELLO_HEADERS})
target_link_libraries(Othello ${LIBS})

if(WIN32)
	# copies the SDL2.dll to target directory
	add_custom_command(TARGET Othello POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_LIBRARY_DIR}/SDL2.dll" $<TARGET_FILE_DIR:Othello>)
endif()

if(WIN32)
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Othello")
endif()
