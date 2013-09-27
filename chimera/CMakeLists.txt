PROJECT(Chimera)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)
find_package (LibXml2 REQUIRED)
if (LIBXML2_FOUND)
	add_definitions( ${LIBXML2_DEFINITIONS} )
	include_directories(${LIBXML2_INCLUDE_DIR})
	link_libraries (${LIBXML2_LIBRARIES})
	endif (LIBXML2_FOUND)
	


include_directories(include ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIR})
#target_link_libraries(chimera ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} -lSDL2)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

add_library (Chimera STATIC
				src/Entity.cpp
				src/Node.cpp
				src/Transform.cpp
				src/Color.cpp
				src/Light.cpp
				src/Camera.cpp
				src/Image.cpp
				src/Material.cpp
				src/Draw.cpp
				src/Shapes.cpp
				src/Physics.cpp
				src/PhysicsWorld.cpp
				src/Mesh.cpp
				#src/Font.cpp
				#src/HUD.cpp
				#src/Loader.cpp
				src/Object.cpp
				src/Timer.cpp 
				src/Video.cpp
				src/SceneMng.cpp
				src/GameClient.cpp
			)

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimera.a DESTINATION lib)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
