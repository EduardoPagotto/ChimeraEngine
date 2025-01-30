cmake_minimum_required(VERSION 3.28)

PROJECT(ChimeraBullet)

find_package(SDL2 REQUIRED)
find_package(Bullet REQUIRED)

#define objetos a compilar
set (BulletSource 	PhysicsControl.cpp
					Solid.cpp)

#define os includes
set (BulletInclude 	${CMAKE_SOURCE_DIR}/chimera
					${SDL2_INCLUDE_DIR}
					${BULLET_INCLUDE_DIR}
					/usr/local/include/) # Raiz do projeto

# Define o Target
add_library (ChimeraBullet STATIC ${BulletSource})

# Define os includes privados para este target
target_include_directories (ChimeraBullet PUBLIC ${BulletInclude})

add_dependencies(ChimeraBullet ChimeraCore
                               ChimeraSpace)
