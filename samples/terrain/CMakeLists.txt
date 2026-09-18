PROJECT(terrain LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(Bullet REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")

#define os includes
set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/chimera/chimera_base/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_core/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_space/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_ecs/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_collada/include/
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_render/include/
				 ${CMAKE_BINARY_DIR}/configured_files/include
				 ${PUGIXML_DIR}
				 ${CMAKE_SOURCE_DIR}
				 ${SDL3_INCLUDE_DIR}
				 ${BULLET_INCLUDE_DIR})

set (SourceFiles src/Game.cpp
				 src/main.cpp)

#define os Lib's a serem usados
set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libchimera_render.a
				  ${CMAKE_SOURCE_DIR}/bin/libchimera_collada.a
                  ${CMAKE_SOURCE_DIR}/bin/libchimera_core.a
                  ${CMAKE_SOURCE_DIR}/bin/libchimera_base.a
				  ${SDL3_IBRARIES}
				  ${BULLET_LIBRARIES}
				  -lSDL3
				  -lSDL3_image
				  -lSDL3_ttf
				  -lpugixml
				  -lpthread)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

add_dependencies(${PROJECT_NAME} chimera_render)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
