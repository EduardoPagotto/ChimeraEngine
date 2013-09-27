cmake_minimum_required(VERSION 2.8)

PROJECT(appTest)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)


include_directories(include ../chimera/include/ ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIR})

link_libraries(${Chimera_BINARY_DIR}/libChimera.a)
link_libraries(${OPENGL_LIBRARIES})
link_libraries(${SDL2_LIBRARIES})
link_libraries(${BULLET_LIBRARIES})
link_libraries(-lSDL2)

add_executable(appTest
        src/Game.cpp
        src/appTest.cpp)

add_dependencies(appTest chimera)

INSTALL(FILES ${appTest_BINARY_DIR}/appTest DESTINATION bin)
