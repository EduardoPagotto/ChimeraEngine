PROJECT(isekai_zero VERSION 0.0.1 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set(include_dir ${CMAKE_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/include
                ${CMAKE_BINARY_DIR}/configured_files/include
                ${PUGIXML_DIR}
                ${CMAKE_SOURCE_DIR}
                ${OPENGL_INCLUDE_DIR}
                ${SDL2_INCLUDE_DIR}
                ${BULLET_INCLUDE_DIR})

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(libs_def ${CMAKE_SOURCE_DIR}/bin/libChimeraRender.a
             ${CMAKE_SOURCE_DIR}/bin/libChimeraCore.a
             ${OPENGL_LIBRARIES}
             ${SDL2_LIBRARY}
             ${BULLET_LIBRARIES}
             ${GLEW_LIBRARIES}
             -lSDL2
             -lSDL2_image
             -lSDL2_ttf
             -lpugixml
             -lpthread)

add_executable (${PROJECT_NAME} ${source_dir}/Game.cpp
                                ${source_dir}/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${include_dir})
target_link_libraries(${PROJECT_NAME} ${libs_def})

add_dependencies(${PROJECT_NAME} ChimeraCore ChimeraRender)


# INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME} DESTINATION bin)
