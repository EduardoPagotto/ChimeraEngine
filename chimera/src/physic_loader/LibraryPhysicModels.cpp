#include "LibraryPhysicModels.hpp"
#include "LibraryPhysicsMaterials.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/physic/Solid.hpp"

namespace ChimeraLoaders {

LibraryPhysicModels::LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url,
                                         Chimera::PhysicsControl* _pWorld)
    : Library(_root, _url) {
    pListNodes = Chimera::Singleton<ListNodes>::getRefSingleton();
    pWorld = _pWorld;
}

LibraryPhysicModels::~LibraryPhysicModels() { Chimera::Singleton<ListNodes>::releaseRefSingleton(); }

void LibraryPhysicModels::target(std::map<std::string, Chimera::Solid*>& _mapSolids) {

    tinyxml2::XMLElement* l_nPhyModel =
        root->FirstChildElement("library_physics_models")->FirstChildElement("physics_model");
    for (l_nPhyModel; l_nPhyModel; l_nPhyModel = l_nPhyModel->NextSiblingElement()) {

        std::string l_id = l_nPhyModel->Attribute("id");
        if (url.compare(l_id) == 0) {

            tinyxml2::XMLElement* l_nRigid = l_nPhyModel->FirstChildElement("rigid_body");
            for (l_nRigid; l_nRigid; l_nRigid = l_nRigid->NextSiblingElement()) {

                std::string l_nNameRb = l_nRigid->Attribute("name");

                Chimera::Solid* pPhysic = new Chimera::Solid(pWorld); //(nullptr, l_nNameRb, pWorld);

                tinyxml2::XMLElement* l_nMass =
                    l_nRigid->FirstChildElement("technique_common")->FirstChildElement("mass");
                if (l_nMass != nullptr) {
                    const char* l_mass = l_nMass->GetText();
                    pPhysic->setMass(atof(l_mass));
                }

                tinyxml2::XMLElement* l_npm =
                    l_nRigid->FirstChildElement("technique_common")->FirstChildElement("instance_physics_material");
                if (l_npm != nullptr) {
                    const char* l_url = l_npm->Attribute("url");

                    LibraryPhysicsMaterials lib(root, l_url);
                    Chimera::PhysicMaterial* pPm = lib.target();

                    pPhysic->setRestitution(pPm->getRestitution());
                    pPhysic->setFrictionDynamic(pPm->getFrictionDynamic());
                    pPhysic->setFrictionStatic(pPm->getFrictionStatic());
                }

                tinyxml2::XMLElement* l_nShape =
                    l_nRigid->FirstChildElement("technique_common")->FirstChildElement("shape");
                if (l_nShape != nullptr) {
                    loadColladaShape(root, l_nShape, pPhysic);
                    // const char* l_mass = l_nShape->GetText();
                    //_pPhysic->setMass ( atof ( l_mass ) );
                }
                _mapSolids[l_nNameRb] = pPhysic;
            }
            return;
        }
    }

    throw Chimera::Exception("Physics model nao encontrado: " + url);
}

void LibraryPhysicModels::loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape,
                                           Chimera::Solid* _pPhysic) {

    _nShape = _nShape->FirstChildElement();
    const char* l_tipoShape = _nShape->Value();

    if (strcmp(l_tipoShape, "sphere") == 0) {

        tinyxml2::XMLElement* l_nEsfera = _nShape->FirstChildElement();
        const char* l_raio = l_nEsfera->GetText();

        std::vector<float> l_arrayValores;
        loadArrayBtScalar(l_raio, l_arrayValores);
        if (l_arrayValores.size() == 1) {
            _pPhysic->setShapeSphere(l_arrayValores[0]);
        } else if (l_arrayValores.size() == 3) {
            _pPhysic->setShapeSphere(l_arrayValores[0]);
        } else {
            throw Chimera::Exception("Shape Sphere Array valores invalidos: " + url);
        }

    } else if (strcmp(l_tipoShape, "plane") == 0) {

        // setShapeBox(btVector3(2.0, 2.0, 2.0));

        tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
        const char* l_size = l_nBox->GetText();

        std::vector<float> l_arrayValores;
        loadArrayBtScalar(l_size, l_arrayValores);

        if (l_arrayValores.size() == 1) {
            _pPhysic->setShapePlane(glm::vec3(l_arrayValores[0], l_arrayValores[0], l_arrayValores[0]),
                                    l_arrayValores[0]);
        } else if (l_arrayValores.size() == 4) {
            _pPhysic->setShapePlane(glm::vec3(l_arrayValores[0], l_arrayValores[1], l_arrayValores[2]),
                                    l_arrayValores[3]);
        } else {
            throw Chimera::Exception("Shape Plane Array valores invalidos: " + url);
        }

    } else if (strcmp(l_tipoShape, "box") == 0) {

        tinyxml2::XMLElement* l_nBox = _nShape->FirstChildElement();
        const char* l_size = l_nBox->GetText();

        std::vector<float> l_arrayValores;
        loadArrayBtScalar(l_size, l_arrayValores);

        if (l_arrayValores.size() == 1) {
            _pPhysic->setShapeBox(glm::vec3(l_arrayValores[0], l_arrayValores[0], l_arrayValores[0]));
        } else if (l_arrayValores.size() == 3) {
            _pPhysic->setShapeBox(glm::vec3(l_arrayValores[0], l_arrayValores[1], l_arrayValores[2]));
        } else {
            throw Chimera::Exception("Shape Box Array valores invalidos: " + url);
        }

    } else if (strcmp(l_tipoShape, "cylinder") == 0) {

        tinyxml2::XMLElement* l_nCyl = _nShape->FirstChildElement();
        const char* l_size = l_nCyl->GetText();

        std::vector<float> l_arrayValores;
        loadArrayBtScalar(l_size, l_arrayValores);

        if (l_arrayValores.size() == 1) {
            _pPhysic->setShapeCilinder(glm::vec3(l_arrayValores[0], l_arrayValores[0], l_arrayValores[0]));
        } else if (l_arrayValores.size() == 3) {
            _pPhysic->setShapeCilinder(glm::vec3(l_arrayValores[0], l_arrayValores[1], l_arrayValores[2]));
        } else {
            throw Chimera::Exception("Shape Cylinder Array valores invalidos: " + url);
        }
    } else if (strcmp(l_tipoShape, "mesh") == 0) {

        // TODO: implementar triMesh de colisao abaixo
        tinyxml2::XMLElement* l_nMesh = _nShape->FirstChildElement(); // instance_geometry
        const char* idMesh = l_nMesh->Attribute("url");
        std::string l_mesh = &idMesh[1];

        Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pListNodes->mapMeshNode[l_mesh];
        if (pMesh != nullptr) {
            // btTriangleIndexVertexArray* indexVertexArray =
            //     new btTriangleIndexVertexArray(pMesh->meshData->vertexIndex.size(),    // num triangles
            //                                    (int*)&pMesh->meshData->vertexIndex[0], // lista de indice
            //                                    3 * sizeof(int),                    // tamanho do indice por  elemento
            //                                    pMesh->meshData->vertexList.size(), // num Vertices
            //                                    (float*)&pMeshmeshData->->vertexList[0], //
            //                                    vList.ptrVal(),                          //
            //                                    3 * sizeof(float));                      //

            // // btTriangleIndexVertexArray *indexVertexArray = new
            // btTriangleIndexVertexArray (
            // //     pMesh->vIndex.getSize() / 3,  //num triangles
            // //     pMesh->vIndex.ptrVal(),		//lista de indice
            // //     3 * sizeof ( int ),					//tamanho do indice
            // por elemento
            // //     pMesh->vList.getSize() / 3,	//num Vertices
            // //     pMesh->vList.ptrVal(),		//lista de vertice
            // //     3 * sizeof ( float )				    //tamanho do vertice
            // por elemento
            // // );

            // _pPhysic->setIndexVertexArray ( indexVertexArray );
        }
    }
}
} // namespace ChimeraLoaders