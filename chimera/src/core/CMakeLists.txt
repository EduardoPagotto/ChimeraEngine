cmake_minimum_required(VERSION 3.10.2)

PROJECT(ChimeraCore)

find_package(SDL2 REQUIRED)

# define objetos a compilar
set (SourcesCore CanvasFB.cpp
				 utils.cpp
				 Timer.cpp
				 Keyboard.cpp
				 JoystickState.cpp
				 JoystickManager.cpp
				 MouseDevice.cpp
				 StateStack.cpp
				 Engine.cpp)

# define os includes
set (IncludeCore ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR})

# Define o Target
add_library (ChimeraCore STATIC ${SourcesCore})

# Define os includes 
target_include_directories (ChimeraCore PUBLIC ${IncludeCore})

# Lista os Includes
# get_property(dirs DIRECTORY ${IncludeCore}/chimera/core PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
# 	message(STATUS "Include dir='${dir}'")
# endforeach()

INSTALL(FILES ${ChimeraCore_BINARY_DIR}/libChimeraCore.a DESTINATION lib)
INSTALL(DIRECTORY ${IncludeCore}/chimera/core DESTINATION ${CMAKE_INSTALL_PREFIX}/include/chimera FILES_MATCHING PATTERN "*.h*")
