PROJECT(models3 LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(Bullet REQUIRED)
find_package(PUGIXML REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

message(STATUS "Compiling: ${PROJECT_NAME}")
message(STATUS "Base: ${CMAKE_SOURCE_DIR}")
message(STATUS "Current: ${CMAKE_CURRENT_SOURCE_DIR}")

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
				 ${OPENGL_INCLUDE_DIR}
				 ${SDL3_INCLUDE_DIR}
				 ${BULLET_INCLUDE_DIR})

set (SourceFiles src/Game.cpp
				 src/main.cpp)

#ordem é importante!!!
set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libchimera_render.a
				  ${CMAKE_SOURCE_DIR}/bin/libchimera_collada.a
                  ${CMAKE_SOURCE_DIR}/bin/libchimera_core.a
                  ${CMAKE_SOURCE_DIR}/bin/libchimera_base.a
				  ${SDL3_LIBRARY}
				  ${BULLET_LIBRARIES}
				  -lSDL3
				  -lSDL3_image
				  -lSDL3_ttf
				  -lpugixml
				  -lpthread)

if (OVR_SET_TO_USE)
	message(STATUS "OVR Incluido no Projeto")
	set(IncludeDirs ${IncludeDirs} /usr/local/ovr_sdk_linux_0.4.4/LibOVR)
    set(LIBRARY_DEPS ${LIBRARY_DEPS} /usr/local/ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64/libovr.a)
endif(OVR_SET_TO_USE)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

add_dependencies(${PROJECT_NAME} chimera_render)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
