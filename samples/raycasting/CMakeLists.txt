PROJECT(raycasting LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(PUGIXML REQUIRED)

set (IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include
				 ${PUGIXML_DIR}
				 ${CMAKE_SOURCE_DIR}
				 ${SDL2_INCLUDE_DIR})

set (SourceFiles src/raycasting.cpp
				 src/Game.cpp
				 src/main.cpp)

set (LIBRARY_DEPS ${SDL2_LIBRARY}
				  -lSDL2
				  -lpugixml
				  -lpthread)

add_executable (${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

target_include_directories (${PROJECT_NAME} PRIVATE ${IncludeDirs})

add_dependencies(${PROJECT_NAME} ChimeraCollada)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME} DESTINATION bin)
