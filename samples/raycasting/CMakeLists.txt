PROJECT(raycasting LANGUAGES CXX)

find_package(SDL3 REQUIRED)

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
				 ${CMAKE_SOURCE_DIR}/chimera/chimera_base/include/
				 ${CMAKE_BINARY_DIR}/configured_files/include)

set (SourceFiles src/raycasting.cpp
				 src/Game.cpp
				 src/main.cpp)

set (LIBRARY_DEPS ${CMAKE_SOURCE_DIR}/bin/libchimera_base.a
                  -lSDL3
				  -lpthread)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

add_dependencies(${PROJECT_NAME} chimera_base clean-coverage)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
