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


find_package (Log4Cxx REQUIRED)
if (Log4Cxx_FOUND)
	add_definitions( ${Log4Cxx_DEFINITIONS} )
	include_directories(${Log4Cxx_INCLUDE_DIR})
	link_libraries (${Log4Cxx_LIBRARIES})	
	endif (Log4Cxx_FOUND)	


include_directories(include ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIR})
#target_link_libraries(chimera ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} -lSDL2)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

add_library (Chimera STATIC
				src/Color.cpp
				src/Entity.cpp
				src/Node.cpp
				src/Light.cpp
				src/Object.cpp
				src/Camera.cpp
				src/TrackCam.cpp
				src/Physics.cpp
				src/PhysicsWorld.cpp				
				src/Image.cpp
				src/Material.cpp
				src/Draw.cpp
				src/Shapes.cpp
				src/Mesh.cpp
				src/Engine3D.cpp
				#src/Font.cpp
				#src/HUD.cpp
				src/Loader.cpp
				src/Timer.cpp 
				src/Video.cpp
				src/SceneMng.cpp
				src/GameClient.cpp
			)

INSTALL(FILES ${Chimera_BINARY_DIR}/libChimera.a DESTINATION lib)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
