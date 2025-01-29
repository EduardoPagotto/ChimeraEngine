PROJECT(isekai_zero VERSION 0.0.1 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set(include_dir ${CMAKE_SOURCE_DIR}/chimera
                ${CMAKE_CURRENT_SOURCE_DIR}/include
                ${CMAKE_BINARY_DIR}/configured_files/include)

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(libs_def ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
             ${CMAKE_SOURCE_DIR}/bin/libChimeraCoreGL.a
             ${CMAKE_SOURCE_DIR}/bin/libChimeraSpace.a
             ${OPENGL_LIBRARIES}
             -lGL
             -lSDL2
             -lGLEW
             -lpthread)

add_executable (${PROJECT_NAME} ${source_dir}/Game.cpp
                                ${source_dir}/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${include_dir})
target_link_libraries(${PROJECT_NAME} ${libs_def})

add_dependencies(${PROJECT_NAME} ChimeraCore ChimeraCoreGL ChimeraSpace)


# INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME} DESTINATION bin)
