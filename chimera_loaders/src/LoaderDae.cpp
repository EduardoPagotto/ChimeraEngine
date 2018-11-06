#include <iostream>

#include "LoaderDae.h"
#include "LoaderDaeUtils.h"
#include "ExceptionChimera.h"

#include "Singleton.h"
#include "Transform.h"

namespace Chimera {

LoaderDae::LoaderDae(const std::string &_file) {

    doc = nullptr;
    root = nullptr;
	pRootNode = nullptr;
    //this->textureDir = _textureDir;
    //this->modelDir = _modelDir;
 
    log = spdlog::get("chimera");
    log->debug("Constructor LoaderDae File:{}", _file);

    pPhysicsControl = Singleton<PhysicsControl>::getRefSingleton();
	texManager = Singleton<TextureManager>::getRefSingleton();

    doc = new tinyxml2::XMLDocument();

    //Verifica se arquivo existe
    tinyxml2::XMLError a_eResult = doc->LoadFile ( _file.c_str());
    if ( a_eResult != 0 ) {
        throw ExceptionChimera ( ExceptionCode::OPEN, "Falha ao ler arquivo erro: " + std::to_string(a_eResult) );
    }

    //vefifica se ele � uma estrutura compativel com collada
    root = doc->FirstChildElement ( "COLLADA" );
    if ( root == nullptr ) {
        throw ExceptionChimera ( ExceptionCode::OPEN, "Nao é um arquivo colada" );
    }

    //carrega elementos de Texture, Material e Geometrias
    int totalTexture = LoaderDae::libTextureMap(root, texManager);
    int totalGeometry = LoaderDae::libGeometryMap(root, mapaGeometria);

    //cria lista de entidade fisicas a serem usadas
    getPhysicSceneInfile();

    //Carrega hierarquia dos nodes
    getNodeSceneInFile();
}

LoaderDae::~LoaderDae() {

    log->debug("Destructor LoaderDae");

    if ( doc != nullptr ) {
        delete doc;
        doc = nullptr;
    }

    Singleton<PhysicsControl>::releaseRefSingleton();
	Singleton<TextureManager>::releaseRefSingleton();
}

void LoaderDae::getPhysicSceneInfile() {

    tinyxml2::XMLElement* l_nPhysicScene = LoaderDae::findSceneLib (root, ( const char* ) "Physic Scene", ( const char* ) "instance_physics_scene", ( const char* ) "library_physics_scenes" );
    if ( l_nPhysicScene != nullptr ) {

        const char *l_nome = l_nPhysicScene->Attribute ( "name" );
        const char *l_id = l_nPhysicScene->Attribute ( "id" );

        LoaderDae::loadPhysicControlCollada( l_nPhysicScene, pPhysicsControl);

        getDadosInstancePhysicModel ( l_nPhysicScene );
    }
}

void LoaderDae::getDadosInstancePhysicModel ( tinyxml2::XMLElement* _nPhysicScene ) {

    tinyxml2::XMLElement* l_nInstancePhysicModel = _nPhysicScene->FirstChildElement ( "instance_physics_model" );
    if ( l_nInstancePhysicModel != nullptr ) {

        const char *l_url = l_nInstancePhysicModel->Attribute ( "url" );

        tinyxml2::XMLElement* l_nInstanceRigidBody = l_nInstancePhysicModel->FirstChildElement ( "instance_rigid_body" );
        if ( l_nInstanceRigidBody != nullptr ) {
            while ( l_nInstanceRigidBody != nullptr ) {

                const char *l_body = l_nInstanceRigidBody->Attribute ( "body" );
                const char *l_target = l_nInstanceRigidBody->Attribute ( "target" );

                tinyxml2::XMLElement* l_nRigidBody = LoaderDae::getDadoRigidBody (root, l_url, l_body );

                const char* l_name = l_nRigidBody->Attribute ( "name" );
                const char* l_sid = l_nRigidBody->Attribute ( "sid" );

                std::string nomeMesh = "";

				Solid *pPhysic = new Solid(nullptr, std::string(l_name));
                LoaderDae::loadColladaPhysicsModel( root, l_nRigidBody, nomeMesh, pPhysic);

				Mesh *pDrawTriMesh = (Mesh*)mapaGeometria[nomeMesh];
                if ( pDrawTriMesh != nullptr ) {
                    // // FIXME: verificar porque trava
                    // btTriangleIndexVertexArray *indexVertexArray = new btTriangleIndexVertexArray (
                    //     pDrawTriMesh->vertexIndex.size(),       //num triangles
                    //     (int*)&pDrawTriMesh->vertexIndex[0],    //lista de indice
                    //     3 * sizeof ( int ),                     //tamanho do indice por elemento
                    //     pDrawTriMesh->vertexList.size(),        //num Vertices
                    //     (float*)&pDrawTriMesh->vertexList[0],   //vList.ptrVal(),       //lista de vertice
                    //     3 * sizeof ( float )                    //tamanho do vertice por elemento
                    // );
                    // // btTriangleIndexVertexArray *indexVertexArray = new btTriangleIndexVertexArray (
                    // //     pDrawTriMesh->vIndex.getSize() / 3,  //num triangles
                    // //     pDrawTriMesh->vIndex.ptrVal(),		//lista de indice
                    // //     3 * sizeof ( int ),					//tamanho do indice por elemento
                    // //     pDrawTriMesh->vList.getSize() / 3,	//num Vertices
                    // //     pDrawTriMesh->vList.ptrVal(),		//lista de vertice
                    // //     3 * sizeof ( float )				    //tamanho do vertice por elemento
                    // // );
                    // pPhysic->setIndexVertexArray ( indexVertexArray );
                }

                mapaEntidadeFisica[std::string((const char*)&l_target[1])] = pPhysic;

                l_nInstanceRigidBody = l_nInstanceRigidBody->NextSiblingElement ( "instance_rigid_body" );
            }
        }
    }

    tinyxml2::XMLElement* l_nInstanceRigidConstraint = l_nInstancePhysicModel->FirstChildElement ( "instance_rigid_constraint" );
    if ( l_nInstanceRigidConstraint != nullptr ) {
        while ( l_nInstanceRigidConstraint != nullptr ) {

            const char *l_constraint = l_nInstanceRigidConstraint->Attribute ( "constraint" );
            //TODO implementar


            l_nInstanceRigidConstraint = l_nInstanceRigidConstraint->NextSiblingElement ( "instance_rigid_constraint" );
        }
    }
}

tinyxml2::XMLElement* LoaderDae::getDadoRigidBody (tinyxml2::XMLElement* _root, const char* _url, const char* _sid ) {

    tinyxml2::XMLElement* l_nNodeSourceData = nullptr;

    loadNodeLib ( _root, ( const char* ) &_url[1], "library_physics_models", "physics_model", &l_nNodeSourceData );
    tinyxml2::XMLElement* l_nRigidBody = l_nNodeSourceData->FirstChildElement ( "rigid_body" );
    if ( l_nRigidBody != nullptr ) {
        while ( l_nRigidBody != nullptr ) {

            const char *l_name = l_nRigidBody->Attribute ( "name" );
            const char *l_sid = l_nRigidBody->Attribute ( "sid" );
            if ( strcmp ( _sid, l_sid ) == 0 ) {
                return l_nRigidBody;
            }
            l_nRigidBody = l_nRigidBody->NextSiblingElement ( "rigid_body" );
        }
    }

     return nullptr;
//     //TODO implementar em outro metodo somente para constrain
//     //tinyxml2::XMLElement* l_nRigidBodyConstraint = l_nNodeSourceData->FirstChildElement("rigid_constraint");
//     //if (l_nRigidBodyConstraint != nullptr) {
//     //	while (l_nRigidBodyConstraint != nullptr) {
//     //		//TODO constraints
//     //		const char *l_name = l_nRigidBodyConstraint->Attribute("name");
//     //		const char *l_sid = l_nRigidBodyConstraint->Attribute("sid");
//     //
//     //		const char * l_teste = l_nRigidBodyConstraint->Name();
//     //		std::cout << "DEBUG: " << l_teste <<std::endl;
//     //
//     //
//     //		l_nRigidBodyConstraint = l_nRigidBodyConstraint->NextSiblingElement("rigid_constraint");
//     //	}
//     //}
}

void LoaderDae::getNodeSceneInFile() {

    pRootNode = nullptr;

    tinyxml2::XMLElement* l_nVisualScene = LoaderDae::findSceneLib ( root, ( const char* ) "Visual Scene", ( const char* ) "instance_visual_scene", ( const char* ) "library_visual_scenes" );
    if ( l_nVisualScene != nullptr ) {

        const char *l_nome = l_nVisualScene->Attribute ( "name" );
        const char *l_id = l_nVisualScene->Attribute ( "id" );

        pRootNode = new Group(nullptr, l_id );
        tinyxml2::XMLElement* l_nNode = l_nVisualScene->FirstChildElement ( "node" );
        if ( l_nNode != nullptr ) {
            while ( l_nNode != nullptr ) {

                const char * l_id = l_nNode->Attribute ( "id" );
                const char * l_name = l_nNode->Attribute ( "name" );
                const char * l_type = l_nNode->Attribute ( "type" );

                tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
                if ( l_nDadoNode != nullptr ) {
                    carregaNode ( (Node*)pRootNode, l_nDadoNode, l_id, l_name, l_type );
                } else {
                    log->warn("Node sem filho");
                }

                l_nNode = l_nNode->NextSiblingElement ( "node" );
            }
        } else {
            log->warn("Node: vazio");
        }
    }
}

void LoaderDae::carregaNode ( Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type ) {

    glm::mat4 l_pTransform;
    Node *pLastNodeDone = nullptr;

    while ( _nNode != nullptr ) {

        tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
        const char* l_url = _nNode->Attribute ( "url" );
        const char* l_target = _nNode->Attribute ( "target" );
        const char* l_nomeElemento = _nNode->Value();

        if ( strcmp ( l_nomeElemento, ( const char* ) "matrix" ) == 0 ) {

            const char* l_tipoTransform = _nNode->Attribute ( "sid" );
            if ( strcmp ( l_tipoTransform, ( const char* ) "transform" ) == 0 ) {

                l_pTransform = loadTransformMatrix ( _nNode->GetText() );

            } else {

                throw ExceptionChimera ( ExceptionCode::READ, "Matrix de transformacao invalida" );
                //TODO: implementar carga de posicao, rotacao e transformar em matricial em l_pTransform
            }


         } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_camera" ) == 0 ) {

           Camera *pCamera = LoaderDae::carregaCamera(root, _nNode, l_url, _id, _name);
           pCamera->setTransform ( l_pTransform );

           _pNodePai->addChild ( pCamera );

           pLastNodeDone = pCamera;

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_light" ) == 0 ) {

            loadNodeLib ( root, ( const char* ) &l_url[1], "library_lights", "light", &l_nNodeSourceData );

            Light *pLight = new Light (nullptr,_id );

            auto ret_data = LoaderDae::loadDiffuseLightColor(l_nNodeSourceData);
            pLight->setDiffuse(std::get<0>(ret_data));
            pLight->setType(std::get<1>(ret_data));
            pLight->setTransform ( l_pTransform );

            _pNodePai->addChild ( pLight );

            pLastNodeDone = pLight;

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "instance_geometry" ) == 0 ) {

             const char* pUrl = _nNode->Attribute("url");
             std::string l_nomeGeo = &pUrl[1];

             Material *pMaterial = nullptr;
            // Texture *pTexture = nullptr;
			 std::string textureName = "";

             Draw *pDraw = mapaGeometria[l_nomeGeo];

            tinyxml2::XMLElement* l_nBindMat = _nNode->FirstChildElement ( "bind_material" );
            if ( l_nBindMat != nullptr ) {
                tinyxml2::XMLElement* l_nTecnicCommon = l_nBindMat->FirstChildElement ( "technique_common" );
                if ( l_nTecnicCommon != nullptr ) {

                    tinyxml2::XMLElement* l_nInstanceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_material" );

                    const char* pTarguet = l_nInstanceMaterial->Attribute("target");
                    loadNodeLib ( root, ( const char* ) &pTarguet[1], "library_materials", "material", &l_nNodeSourceData );
                    if (l_nNodeSourceData != nullptr) {

                        std::string nomeMaterial = retornaAtributo("id", l_nNodeSourceData);

                        pMaterial = new Material(nomeMaterial);

                        LoaderDae::loadMaterial(root, l_nNodeSourceData, pMaterial);

                        tinyxml2::XMLElement* l_nInstanceEffect = l_nNodeSourceData->FirstChildElement ( "instance_effect" );
                        if (l_nInstanceEffect != nullptr) {

                            tinyxml2::XMLElement* l_nNodeSourceTex = nullptr;
                            const char* l_pUrlEffect = l_nInstanceEffect->Attribute ( "url" );
                            loadNodeLib ( root, ( const char* ) &l_pUrlEffect[1], "library_effects", "effect", &l_nNodeSourceTex );

                            if (l_nNodeSourceTex != nullptr) {

                                tinyxml2::XMLElement* l_nProfile = l_nNodeSourceTex->FirstChildElement("profile_COMMON");
                                if (l_nProfile != nullptr) {

                                    tinyxml2::XMLElement* l_nParam = l_nProfile->FirstChildElement("newparam");
                                    if (l_nParam != nullptr) {
                                        const char* l_val = l_nParam->FirstChildElement("surface")->FirstChildElement("init_from")->GetText();
                                        if (l_val != nullptr)
											textureName.assign(l_val);
                                    }
                                }
                            }
                        }
                    }
                }
            }

			if (pMaterial != nullptr) {

				if (textureName.length() > 0) {
					unsigned serialTex = texManager->getIdByName(textureName);
					pMaterial->defineTextureByIndex(serialTex);
				}

				pDraw->setMaterial(pMaterial);
			}

			Solid *ph = mapaEntidadeFisica[_id];
            if (ph != nullptr) {

				ph->setName(_id);//Preciso ???
				ph->setMatrix(l_pTransform);
				ph->addChild(pDraw);
				pLastNodeDone = ph;
				_pNodePai->addChild(ph);

            } else {

                Transform *pTrans = new Transform(_pNodePai, _id);
                pTrans->setMatrix(l_pTransform);
                pTrans->addChild(pDraw);
                pLastNodeDone = pTrans;
            }

         } else if ( strcmp ( l_nomeElemento, ( const char* ) "node" ) == 0 ) {

            const char * l_id = _nNode->Attribute ( "id" );
            const char * l_name = _nNode->Attribute ( "name" );
            const char * l_type = _nNode->Attribute ( "type" );

            if ( pLastNodeDone != nullptr ) {
                carregaNode ( pLastNodeDone, _nNode->FirstChildElement(), l_id, l_name, l_type );
            } else {
                throw ExceptionChimera ( ExceptionCode::READ, "Falha, objeto hierarquia: " + std::string ( l_id ) );
            }

        } else  {

            throw ExceptionChimera ( ExceptionCode::READ, "Falha, objeto desconhecido: " + std::string ( l_nomeElemento ) );

        }
        _nNode = _nNode->NextSiblingElement();
    }
}

tinyxml2::XMLElement* LoaderDae::findSceneLib (tinyxml2::XMLElement* pRoot, const char *rotina, const char* instance, const char* library ) {

    std::shared_ptr<spdlog::logger> log = spdlog::get("chimera");
    log->info("findSceneLib");

    tinyxml2::XMLElement* l_nNodeScene = pRoot->FirstChildElement ( "scene" );
    if ( l_nNodeScene != nullptr ) {

        tinyxml2::XMLElement* l_nNode = l_nNodeScene->FirstChildElement ( instance );
        if ( l_nNode != nullptr ) {

            const char* l_url = l_nNode->Attribute ( "url" );
            l_nNode = pRoot->FirstChildElement ( library );
            if ( l_nNode != nullptr ) {

                l_nNode = l_nNode->FirstChildElement();//TODO colocar um while para percorrer todas as instancias de scena(pode haver mais de 1)
                if ( l_nNode != nullptr ) {
                    while ( l_nNode != nullptr ) {

                        const char *l_id = l_nNode->Attribute ( "id" );
                        if ( strcmp ( l_id, ( const char* ) &l_url[1] ) == 0 ) {
                            return l_nNode;
                        }

                        l_nNode = l_nNode->NextSiblingElement();
                    }

                    log->warn("{0} : {1} nao encontrado", std::string(rotina), std::string((const char*) &l_url[1]));
                } else {
                    log->warn("{0} vazio", std::string(rotina));
                }

            } else {
                log->warn("{0} nao encontrado", std::string(library));
            }
        } else {
            log->warn("{0} nao encontrado", std::string(instance));
        }
    }

    return nullptr;
}

int LoaderDae::libTextureMap(tinyxml2::XMLElement* _root, TextureManager *_texManager) {

	int totalTexturas = 0;
    tinyxml2::XMLElement* l_nNode = _root->FirstChildElement ( "library_images" );
    if ( l_nNode != nullptr ) {

        l_nNode = l_nNode->FirstChildElement ( "image" ); //FIXME quando outras texturas presentes 0 difuse, 1 specula, 2 emissive
        while ( l_nNode != nullptr ) {

            std::string l_id = retornaAtributo ( "id", l_nNode );
            std::string l_name = retornaAtributo( "name", l_nNode);
            std::string l_val = l_nNode->FirstChildElement( "init_from" )->GetText();

			_texManager->fromFile(l_id, TEX_SEQ::DIFFUSE, l_val);

            l_nNode = l_nNode->NextSiblingElement ( "image" );
			totalTexturas++;
        }
    }

	return totalTexturas;
}

int LoaderDae::libGeometryMap(tinyxml2::XMLElement* _root, std::map<std::string, Draw*> &mapaGeometria) {

    tinyxml2::XMLElement* l_nNode = _root->FirstChildElement ( "library_geometries" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "geometry" );
        while ( l_nNode != nullptr ) {

            std::string l_id = retornaAtributo ( "id", l_nNode );
            std::string l_name = retornaAtributo ( "name", l_nNode );

            Mesh *pDraw = new Mesh(nullptr, l_id);
            LoaderDae::loadMeshCollada(l_nNode, pDraw);
            mapaGeometria[l_id] = pDraw;

            l_nNode = l_nNode->NextSiblingElement ( "geometry" );
        }
    }
    return mapaGeometria.size();
}

//---------Camera-------

Camera *LoaderDae::carregaCamera(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nNode, const char* l_url, const char* _id, const char* _name) {

    tinyxml2::XMLElement* l_nNodeSourceData = nullptr;

    loadNodeLib ( _root, ( const char* ) &l_url[1], "library_cameras", "camera", &l_nNodeSourceData );
    Camera *pCamera = nullptr;

    tinyxml2::XMLElement* l_nExtra = _nNode->FirstChildElement ( "extra" );
    if ( l_nExtra != nullptr ) {
        tinyxml2::XMLElement* l_nTechnique = l_nExtra->FirstChildElement ( "technique" );
        if ( l_nTechnique != nullptr ) {

            const char* l_profile = l_nTechnique->Attribute ( "profile" );
            if ( ( l_profile != nullptr ) && ( strcmp ( l_profile, ( const char* ) "chimera" ) == 0 ) ) {

                tinyxml2::XMLElement* l_nOrbital = l_nTechnique->FirstChildElement ( "orbital" );
                if ( l_nOrbital != nullptr ) {

                    const char* min = nullptr;
                    const char* max = nullptr;

                    tinyxml2::XMLElement* l_param = l_nOrbital->FirstChildElement();
                    while ( l_param != nullptr ) {

                        if ( strcmp ( l_param->Value(), ( const char* ) "min" ) == 0 ) {
                            min = l_param->GetText();
                        } else if ( strcmp ( l_param->Value(), ( const char* ) "max" ) == 0 ) {
                            max = l_param->GetText();
                        }

                        l_param = l_param->NextSiblingElement();
                    }

                    if ( ( min != nullptr ) && ( max != nullptr ) ) {
                        CameraSpherical *pCameraNew = new CameraSpherical ( _id );
                        pCameraNew->setDistanciaMaxima ( atof ( max ) );
                        pCameraNew->setDistanciaMinima ( atof ( min ) );
                        pCamera = pCameraNew;
                    }

                }
            }
        }
    }

    if ( pCamera == nullptr ) {
        pCamera = new Camera (nullptr, CameraType::Base, _id );
    }

    pCamera->loadCollada ( l_nNodeSourceData );

    return pCamera;
}

//------Light--------

std::tuple<Color, LightType> LoaderDae::loadDiffuseLightColor(tinyxml2::XMLElement* _nNode)
{
    tinyxml2::XMLElement *l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "point" );
    if ( l_nPoint != nullptr ) {

        std::vector<float> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement("color")->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );
        Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

        return std::make_tuple(cor, LightType::POSITIONAL);
    }

    l_nPoint = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "directional" );
    if ( l_nPoint != nullptr ) {

        std::vector<float> l_arrayF;
        const char *l_val = l_nPoint->FirstChildElement ( "color" )->GetText();
        loadArrayBtScalar ( l_val, l_arrayF );

        Color cor(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);

        return std::make_tuple(cor, LightType::DIRECTIONAL);
    }

}

//----------Material-----

void LoaderDae::loadMaterial(tinyxml2::XMLElement* root, tinyxml2::XMLElement* _nNode, Material *_pMat) {

    tinyxml2::XMLElement* l_nInstanceEffect = _nNode->FirstChildElement ( "instance_effect" );
    if (l_nInstanceEffect != nullptr) {

        tinyxml2::XMLElement* l_nNodeSourceData = nullptr;
        const char* l_pUrlEffect = l_nInstanceEffect->Attribute ( "url" );
        loadNodeLib ( root, ( const char* ) &l_pUrlEffect[1], "library_effects", "effect", &l_nNodeSourceData );

        if (l_nNodeSourceData != nullptr) {
            LoaderDae::loadMaterialProfile(l_nNodeSourceData, _pMat);
        }
    }
}

void LoaderDae::loadMaterialProfile(tinyxml2::XMLElement* _nNode, Material *_pMat) {

	tinyxml2::XMLElement* l_nProfile = _nNode->FirstChildElement("profile_COMMON");
	if (l_nProfile != nullptr) {

        Color cor;
        if (getPhong("emission", cor, l_nProfile) == true) {
            _pMat->setEmission(cor);
        }

        if (getPhong("ambient", cor, l_nProfile) == true) {
            _pMat->setAmbient(cor);
        }

        if (getPhong("diffuse", cor, l_nProfile) == true) {
            _pMat->setDiffuse(cor);
        }

        if (getPhong("specular", cor, l_nProfile) == true) {
            _pMat->setSpecular(cor);
        }

        tinyxml2::XMLElement* l_nNode = l_nProfile->FirstChildElement("technique");
        if (l_nNode->Attribute("sid") != nullptr) {

            tinyxml2::XMLElement* l_nPhong = l_nNode->FirstChildElement("phong");
            if (l_nPhong != nullptr) {

                tinyxml2::XMLElement* l_nShinnes = l_nPhong->FirstChildElement("shininess");
                if (l_nShinnes != nullptr) {

                    const char *l_val = l_nShinnes->FirstChildElement("float")->GetText();
                    if (l_val != nullptr) {

                        _pMat->setShine(atof(l_val));

                    }
                }
            }
        }
    }
}

bool LoaderDae::getPhong ( const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode ) {

    tinyxml2::XMLElement* l_nNode = _nNode->FirstChildElement ( "technique" );
    if ( l_nNode->Attribute ( "sid" ) != nullptr ) {

        tinyxml2::XMLElement* l_nPhong = l_nNode->FirstChildElement ( "phong" );
        if ( l_nPhong != nullptr ) {
            tinyxml2::XMLElement* l_nColor = l_nPhong->FirstChildElement ( _tipoCor );
            if ( l_nColor != nullptr ) {
				tinyxml2::XMLElement* l_nColorVal = l_nColor->FirstChildElement ( "color" );
                if (l_nColorVal != nullptr ) {
                    std::vector<float> l_arrayF;
                    const char* l_cor = l_nColorVal->GetText();
                    loadArrayBtScalar ( l_cor, l_arrayF );

                    _color.set ( l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0 );
                    return true;
                } else {

					l_nColorVal = l_nColor->FirstChildElement("texture");
					if (l_nColorVal != nullptr) {

						_color.set(Color::WHITE);
						return true;
					}
                }
            }
         }
    }

    return false;
}

//------solid----

void LoaderDae::loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, std::string &_meshName, Solid *_pPhysic) {

    if ( _nShape != nullptr ) {
        _nShape = _nShape->FirstChildElement();
        const char *l_tipoShape = _nShape->Value();

        if ( strcmp ( l_tipoShape, "sphere" ) == 0 ) {

            tinyxml2::XMLElement* l_nEsfera = _nShape->FirstChildElement();
            const char *l_raio = l_nEsfera->GetText();

            std::vector<float> l_arrayValores;
            loadArrayBtScalar ( l_raio, l_arrayValores );
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
            loadArrayBtScalar ( l_size, l_arrayValores );

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
            loadArrayBtScalar ( l_size, l_arrayValores );

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
            loadArrayBtScalar ( l_size, l_arrayValores );

            if ( l_arrayValores.size() == 1 ) {
                _pPhysic->setShapeCilinder ( glm::vec3 ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] ) );
            } else if ( l_arrayValores.size() == 3 ) {
                _pPhysic->setShapeCilinder ( glm::vec3 ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] ) );
            }
        } else if ( strcmp ( l_tipoShape, "mesh" ) == 0 ) { //FIXME ERRADO!!!!

            //setShapeBox(btVector3(1.0, 1.0, 1.0));

            //instance_geometry
            tinyxml2::XMLElement* l_nMesh = _nShape->FirstChildElement();
            if ( l_nMesh != nullptr ) {
                const char *l_mesh = l_nMesh->Attribute ( "url" );
                if ( l_mesh != nullptr ) {
                    _meshName = ( const char* ) &l_mesh[1];
                }
            }
        }
    }
}

void LoaderDae::loadColladaPhysicsModel ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nRigidBody, std::string &_meshName, Solid *_pPhysic) {

    tinyxml2::XMLElement* l_nTecnicCommon = _nRigidBody->FirstChildElement ( "technique_common" );
    if ( l_nTecnicCommon != nullptr ) {

        //Massa
        tinyxml2::XMLElement* l_nMass = l_nTecnicCommon->FirstChildElement ( "mass" );
        if ( l_nMass != nullptr ) {
            const char* l_mass = l_nMass->GetText();
            _pPhysic->setMass ( atof ( l_mass ) );
        }
    }

    //shape
    tinyxml2::XMLElement* l_nShape = l_nTecnicCommon->FirstChildElement ( "shape" );
    loadColladaShape ( _root, l_nShape, _meshName, _pPhysic);

    //material
    tinyxml2::XMLElement* l_nInstaceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_physics_material" );
    if ( l_nInstaceMaterial != nullptr ) {
        const char* l_url = l_nInstaceMaterial->Attribute ( "url" );

        tinyxml2::XMLElement* l_nMateriual = nullptr;
        Chimera::loadNodeLib ( _root, ( const char* ) &l_url[1], "library_physics_materials", "physics_material", &l_nMateriual );

        tinyxml2::XMLElement* l_nTec = l_nMateriual->FirstChildElement ( "technique_common" );
        if ( l_nTec != nullptr ) {

            tinyxml2::XMLElement* l_nFric = l_nTec->FirstChildElement ( "dynamic_friction" );
            if ( l_nFric != nullptr ) {
                const char *l_fric = l_nFric->GetText();
                if ( l_fric != nullptr ) {
                    _pPhysic->setFrictionDynamic ( atof ( l_fric ) );
                }
            }

            tinyxml2::XMLElement* l_nRes = l_nTec->FirstChildElement ( "restitution" );
            if ( l_nRes != nullptr ) {
                const char *l_res = l_nRes->GetText();
                if ( l_res != nullptr ) {
                    _pPhysic->setRestitution ( atof ( l_res ) );
                }
            }
        }
    }
}

//------- PhysicControl

void LoaderDae::loadPhysicControlCollada ( tinyxml2::XMLElement* _nNode, PhysicsControl *_pPhysicsControl ) {

    tinyxml2::XMLElement* l_nTecnicCommon = _nNode->FirstChildElement ( "technique_common" );
    if ( l_nTecnicCommon != nullptr ) {

        tinyxml2::XMLElement* l_nNodeGravity = l_nTecnicCommon->FirstChildElement ( "gravity" );
        if ( l_nNodeGravity != nullptr ) {

            std::vector<btScalar> l_arrayF;
            const char* vetor = l_nNodeGravity->GetText();
            loadArrayBtScalar ( vetor, l_arrayF );

            _pPhysicsControl->setGravity ( btVector3 ( l_arrayF[0], l_arrayF[1], l_arrayF[2] ) );

        }
    }

}

//-------Mesh

int LoaderDae::getSource ( tinyxml2::XMLElement* _source, std::vector<float> &_arrayValores ) {

    tinyxml2::XMLElement* l_nSource = _source->FirstChildElement ( "float_array" );
    if ( l_nSource != nullptr ) {

        const char *l_numCount = l_nSource->Attribute ( "count" );
        if ( l_numCount != nullptr ) {

            //std::vector<float> l_array;
            const char* l_vals = l_nSource->GetText();
            loadArrayBtScalar( l_vals, _arrayValores );
            return _arrayValores.size();
        }
    }

    return -1;
}

void LoaderDae::loadMeshCollada ( tinyxml2::XMLElement* _nNode, Mesh *_pDraw) {

    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement ( "mesh" );
    if ( l_nMesh != nullptr ) {

        tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement ( "source" );

        //Carrega lista de vetores
        while ( l_nSource != nullptr ) {

            const char *l_id = l_nSource->Attribute ( "id" );
            if ( strstr ( l_id, ( char* ) "-positions" ) != nullptr ) {

                //Carrega todos os vetores ponto
                std::vector<float> lista;
                getSource ( l_nSource, lista );

                for (unsigned int indice=0; indice < lista.size(); indice += 3)
                    _pDraw->vertexList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]) );

            } else if ( strstr ( l_id, ( char* ) "-normals" ) != nullptr ) {

                //carrega todos os vetores normal
                std::vector<float> lista;
                getSource ( l_nSource, lista );

                for (unsigned int indice=0; indice < lista.size(); indice += 3)
                    _pDraw->normalList.push_back( glm::vec3(lista[indice],lista[indice+1],lista[indice+2]));

            } else if ( strstr ( l_id, ( char* ) "-map-0" ) != nullptr ) {

                //carrega vetor posicao textura
                std::vector<float> lista;
                getSource ( l_nSource, lista );
                for (unsigned int indice=0; indice < lista.size(); indice += 2)
                    _pDraw->textureList.push_back( glm::vec2(lista[indice],lista[indice+1]) );

            }

            l_nSource = l_nSource->NextSiblingElement ( "source" );
        }

        //Carrega Lista de indices
        tinyxml2::XMLElement* l_nPoly = l_nMesh->FirstChildElement ( "polylist" );
        if ( l_nPoly != nullptr ) {

            const char *l_count = l_nPoly->Attribute ( "count" );
            const char *l_mat = l_nPoly->Attribute ( "material" );

            tinyxml2::XMLElement* l_nInput = l_nPoly->FirstChildElement ( "input" );

            std::vector<const char*> l_vOffset;
            std::vector<const char*> l_vSemantic;
            std::vector<const char*> l_vSource;

            while ( l_nInput != nullptr ) {

                const char *l_offSet = l_nInput->Attribute ( "offset" );
                const char *l_semantic = l_nInput->Attribute ( "semantic" );
                const char *l_source = l_nInput->Attribute ( "source" );

                l_vOffset.push_back ( l_offSet );
                l_vSemantic.push_back ( l_semantic );
                l_vSource.push_back ( l_source );

                l_nInput = l_nInput->NextSiblingElement ( "input" );
            }

            tinyxml2::XMLElement* l_nP = l_nPoly->FirstChildElement ( "p" );
            const char *l_indices = l_nP->GetText();
            std::vector<int> l_arrayIndex;

            loadArrayI ( l_indices, l_arrayIndex );

            int l_numTriangles = atoi ( l_count );

            for ( unsigned l_contador = 0; l_contador < l_arrayIndex.size(); l_contador++ ) {
                int index = l_contador % l_vOffset.size();

                const char *l_offSet = l_vOffset[index];
                const char *l_semantic = l_vSemantic[index];
                const char *l_source = l_vSource[index];

                if ( strstr ( l_source, ( char* ) "-vertices" ) != nullptr ) { //indices de vetor ponto

                    _pDraw->vertexIndex.push_back( l_arrayIndex[l_contador]  );

                } else if ( strstr ( l_source, ( char* ) "-normals" ) != nullptr ) { //indice de vetor normal

                    _pDraw->normalIndex.push_back(l_arrayIndex[l_contador]);

                } else if ( strstr ( l_source, ( char* ) "-map-0" ) != nullptr ) { //indice de vetor posicao textura

                    _pDraw->textureIndex.push_back( l_arrayIndex[l_contador] );

                }
            }
            l_arrayIndex.clear();

            l_vOffset.clear();
            l_vSemantic.clear();
            l_vSource.clear();

        }
    }

    std::shared_ptr<spdlog::logger> log = spdlog::get("chimera");
    log->debug("Nome: {0}", _pDraw->getName().c_str());
    log->debug("Vertex  Indice / Lista ------ ( {0:03d} / {1:03d} )", _pDraw->vertexIndex.size(), _pDraw->vertexList.size());
	log->debug("Normal  Indice / Lista ------ ( {0:03d} / {1:03d} )", _pDraw->normalIndex.size(), _pDraw->normalList.size());
    log->debug("Texture Indice / Lista ------ ( {0:03d} / {1:03d} )", _pDraw->textureIndex.size(), _pDraw->textureList.size());
}

}
