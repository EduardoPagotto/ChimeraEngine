#include "LibraryPhysicModels.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"
#include "Group.h"
#include "Solid.h"
#include "LibraryPhysicsMaterials.h"
#include "Mesh.h"
#include "LibraryGeometrys.h"

namespace ChimeraLoaders {

LibraryPhysicModels::LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryPhysicModels::~LibraryPhysicModels() {
}

Chimera::Group *LibraryPhysicModels::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_physics_models");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("physics_model");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    Chimera::Group *pGroup = new Chimera::Group(nullptr, l_id);

                    tinyxml2::XMLElement* l_nRigid = l_nNodeBase->FirstChildElement ("rigid_body");
                    while(l_nRigid != nullptr) {

                        const char* l_nNameRb = l_nRigid->Attribute("name");

                        Chimera::Solid *pPhysic = new Chimera::Solid(nullptr, std::string(l_nNameRb));


                        tinyxml2::XMLElement* l_nTec = l_nRigid->FirstChildElement ("technique_common");
                        if (l_nTec != nullptr) {

                            //Massa
                            tinyxml2::XMLElement* l_nMass = l_nTec->FirstChildElement ( "mass" );
                            if ( l_nMass != nullptr ) {
                                const char* l_mass = l_nMass->GetText();
                                pPhysic->setMass ( atof ( l_mass ) );
                            }

                            // // Dinamico
                            // tinyxml2::XMLElement* l_nDy = l_nTec->FirstChildElement ( "dynamic" );
                            // if ( l_nDy != nullptr ) {
                            //     //const char* l_mass = l_nDy->GetText();
                            //     //_pPhysic->setMass ( atof ( l_mass ) );
                            // }

                            // instance_physics_material
                            tinyxml2::XMLElement* l_npm = l_nTec->FirstChildElement ( "instance_physics_material" );
                            if ( l_npm != nullptr ) {
                                const char* l_url = l_npm->Attribute("url");

                                LibraryPhysicsMaterials lib(root, l_url);
                                Chimera::PhysicMaterial * pPm = lib.target();

                                pPhysic->setRestitution( pPm->getRestitution() );
                                pPhysic->setFrictionDynamic( pPm->getFrictionDynamic() );
                                pPhysic->setFrictionStatic( pPm->getFrictionStatic() );

                            }

                            tinyxml2::XMLElement* l_nShape = l_nTec->FirstChildElement ( "shape" );
                            if ( l_nShape != nullptr ) {

                                loadColladaShape(root, l_nShape, pPhysic);

                                //const char* l_mass = l_nShape->GetText();
                                //_pPhysic->setMass ( atof ( l_mass ) );
                            }


                            pGroup->addChild(pPhysic);
                        } 

                        l_nRigid = l_nRigid->NextSiblingElement();  
                    }

                    return pGroup;
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "physics_model" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_physics_models" ) );
    }
}

void LibraryPhysicModels::loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Chimera::Solid *_pPhysic) {

    if ( _nShape != nullptr ) {
        _nShape = _nShape->FirstChildElement();
        const char *l_tipoShape = _nShape->Value();

        if ( strcmp ( l_tipoShape, "sphere" ) == 0 ) {

            tinyxml2::XMLElement* l_nEsfera = _nShape->FirstChildElement();
            const char *l_raio = l_nEsfera->GetText();

            std::vector<float> l_arrayValores;
            Chimera::loadArrayBtScalar ( l_raio, l_arrayValores );
            if ( l_arrayValores.size() == 1 ) {
                _pPhysic->setShapeSphere ( l_arrayValores[0] );
            } else if ( l_arrayValores.size() == 3 ) {
                _pPhysic->setShapeSphere ( l_arrayValores[0] );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "plane" ) == 0 ) {

            //setShapeBox(btVector3(2.0, 2.0, 2.0));

            tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
            const char *l_size = l_nBox->GetText();

            std::vector<float> l_arrayValores;
            Chimera::loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                _pPhysic->setShapePlane ( glm::vec3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ), l_arrayValores[0] );
            } else if ( l_arrayValores.size() == 4 ) {
                _pPhysic->setShapePlane ( glm::vec3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ), l_arrayValores[3] );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "box" ) == 0 ) {

            tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
            const char *l_size = l_nBox->GetText();

            std::vector<float> l_arrayValores;
            Chimera::loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                _pPhysic->setShapeBox ( glm::vec3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ) );
            } else if ( l_arrayValores.size() == 3 ) {
                _pPhysic->setShapeBox ( glm::vec3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ) );
            } else {

            }

        } else if ( strcmp ( l_tipoShape, "cylinder" ) == 0 ) {

            tinyxml2::XMLElement* l_nCyl = _nShape->FirstChildElement();
            const char *l_size = l_nCyl->GetText();

            std::vector<float> l_arrayValores;
            Chimera::loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                _pPhysic->setShapeCilinder ( glm::vec3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ) );
            } else if ( l_arrayValores.size() == 3 ) {
                _pPhysic->setShapeCilinder ( glm::vec3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ) );
            }
        } else if ( strcmp ( l_tipoShape, "mesh" ) == 0 ) {

            // TODO: implementar triMesh de colisao abaixo
            // tinyxml2::XMLElement* l_nMesh = _nShape->FirstChildElement(); //instance_geometry
            // if ( l_nMesh != nullptr ) {
            //     const char *l_mesh = l_nMesh->Attribute ( "url" );
            //     if ( l_mesh != nullptr ) {

            //         LibraryGeometrys lib(_root, l_mesh);
            //         auto pDrawTriMesh = lib.target();

            //         if ( pDrawTriMesh != nullptr ) {

            //             // FIXME: ERRADO!!!! verificar porque trava, Usar o VBO como dado desta vez 
            //             btTriangleIndexVertexArray *indexVertexArray = new btTriangleIndexVertexArray (
            //                 pDrawTriMesh->vertexIndex.size(),       //num triangles
            //                 (int*)&pDrawTriMesh->vertexIndex[0],    //lista de indice
            //                 3 * sizeof ( int ),                     //tamanho do indice por elemento
            //                 pDrawTriMesh->vertexList.size(),        //num Vertices
            //                 (float*)&pDrawTriMesh->vertexList[0],   //vList.ptrVal(),       //lista de vertice
            //                 3 * sizeof ( float )                    //tamanho do vertice por elemento
            //             );

            //             // btTriangleIndexVertexArray *indexVertexArray = new btTriangleIndexVertexArray (
            //             //     pDrawTriMesh->vIndex.getSize() / 3,  //num triangles
            //             //     pDrawTriMesh->vIndex.ptrVal(),		//lista de indice
            //             //     3 * sizeof ( int ),					//tamanho do indice por elemento
            //             //     pDrawTriMesh->vList.getSize() / 3,	//num Vertices
            //             //     pDrawTriMesh->vList.ptrVal(),		//lista de vertice
            //             //     3 * sizeof ( float )				    //tamanho do vertice por elemento
            //             // );

            //             _pPhysic->setIndexVertexArray ( indexVertexArray );
            //         }
            //     }
            // }
        }
    }
}
}