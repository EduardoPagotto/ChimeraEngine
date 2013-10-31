#include "Loader.h"

namespace Chimera {

Loader::Loader () {

    doc = nullptr;
    root = nullptr;
    logger = log4cxx::Logger::getLogger ( "Loader" );

}

Loader::~Loader() {

    if ( doc != nullptr ) {
        delete doc;
        doc = nullptr;
    }

    while ( !listaNodeRemover.empty() ) {
        Node *pNode = listaNodeRemover.front();
        listaNodeRemover.pop();
        delete pNode;
        pNode = nullptr;
    }

}

Node* Loader::loadDAE ( const std::string &_file ) {

    Node *pRootScene = nullptr;
    std::string l_nomeArquivo = m_modelDir +  _file;

    doc = new tinyxml2::XMLDocument();
    doc->LoadFile ( l_nomeArquivo.c_str() );
    root = doc->FirstChildElement ( "COLLADA" );
    if ( root == nullptr ) {
        throw ExceptionChimera ( ExceptionCode::READ, "Falha Doc XML:" + _file + std::string ( " :" ) + doc->GetErrorStr1() );
    }

    libCam();
    libLight();
    libEffect();
    libTexture();
    libMaterial();
    libGeometry();
    pRootScene = libScene();//Carrega componentes da cena
    libPhysicsMaterial();
    libPhysicsModels();
    libPhysicsScenes();

    return pRootScene;
}

std::string Loader::retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node ) {

    const char *l_value = _node->Attribute ( _atributo.c_str() );
    if ( l_value != nullptr ) {
        return std::string ( l_value );
    }

    LOG4CXX_WARN ( logger , std::string ( "Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado" ) );

    return std::string ( "" );
}

void Loader::libCam ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_cameras" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "camera" );
        while ( l_nNode != nullptr ) {

            Camera *pCamera = new Camera ( CameraType::Base, retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
            listaNodeRemover.push ( pCamera );

            pCamera->loadCollada ( l_nNode );

            l_nNode = l_nNode->NextSiblingElement ( "camera" );
        }
    }
}

void Loader::libLight () {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_lights" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "light" );
        while ( l_nNode != nullptr ) {

            Light *pLight = new Light ( LightType::POINT,  retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
            listaNodeRemover.push ( pLight ); //Salva na lista de remocao

            pLight->loadCollada ( l_nNode );

            l_nNode = l_nNode->NextSiblingElement ( "light" );
        }
    }
}

void Loader::libEffect ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_effects" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "effect" );
        while ( l_nNode != nullptr ) {

            Effect *pEffect = new Effect ( retornaAtributo ( "id",l_nNode ),"Effect" );
            listaNodeRemover.push ( pEffect );

            pEffect->loadCollada ( l_nNode );

            l_nNode = l_nNode->NextSiblingElement ( "effect" );
        }

    }
}

void Loader::libTexture ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_images" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "image" );
        while ( l_nNode != nullptr ) {
            Texture *pTexture = new Texture ( retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
            listaNodeRemover.push ( pTexture );

            const char* l_val = l_nNode->FirstChildElement ( "init_from" )->GetText();
            pTexture->setPathFile ( m_imageDir + std::string ( l_val ) );
            pTexture->init();

            l_nNode = l_nNode->NextSiblingElement ( "image" );
        }
    }
}

void Loader::libMaterial ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_materials" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "material" );
        while ( l_nNode != nullptr ) {

            Material *pMaterial = new Material ( retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
            listaNodeRemover.push ( pMaterial );

            const char* l_val = l_nNode->FirstChildElement ( "instance_effect" )->Attribute ( "url" );
            if ( l_val != nullptr ) {

                Effect *pEffe = ( Effect* ) Node::findNodeById ( EntityKind::EFFECT,  &l_val[1] ); //m_mEffect[ l_nomeEffect ];
                if ( pEffe ) {
                    pMaterial->pEffect = pEffe;
                    if ( pEffe->getNameTextureId().size() > 0 ) {

                        Texture *pTexture = ( Texture* ) Node::findNodeById ( EntityKind::TEXTURE,pEffe->getNameTextureId() ); //m_mTextura[pEffe->getNameTextureId()];//( Texture* ) Node::findNodeById ( EntityKind::IMAGE, pEffe->getNameTextureId() );
                        if ( pTexture!=nullptr ) {
                            pMaterial->pTextura = pTexture;//new Texture( *pTexture );
                        }

                    }
                }

            }

            l_nNode = l_nNode->NextSiblingElement ( "material" );
        }
    }
}

void Loader::libGeometry ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_geometries" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "geometry" );
        while ( l_nNode != nullptr ) {

            DrawTriMesh *pDrawTriMesh = new DrawTriMesh ( retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
            listaNodeRemover.push ( pDrawTriMesh );

            pDrawTriMesh->loadCollada ( l_nNode );

            l_nNode = l_nNode->NextSiblingElement ( "geometry" );
        }
    }
}

Node* Loader::libScene ( void ) {

    Node *pScene = nullptr;
    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_visual_scenes" );
    if ( l_nNode != nullptr ) {

        l_nNode = l_nNode->FirstChildElement ( "visual_scene" );
        pScene = new Node ( EntityKind::NODE, retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );

        l_nNode = l_nNode->FirstChildElement ( "node" );
        while ( l_nNode != nullptr ) {

            createNode ( l_nNode, pScene );
            l_nNode = l_nNode->NextSiblingElement ( "node" );

        }
    }

    return pScene;
}

void Loader::carregaMatrix ( btTransform *_pTrans, const std::vector<float> &listaMatrix ) {

    if ( listaMatrix.size() != 16 )
        throw ExceptionChimera ( ExceptionCode::READ, "Tamanho da Matrix invalido" + std::to_string ( listaMatrix.size() ) );

    btScalar ponteiroFloat[16];
    int indice = 0;
    for ( int i=0; i<4; i++ ) {
        for ( int j=0; j<4; j++ ) {
            int pos =  i + ( 4*j );
            ponteiroFloat[ pos ] = listaMatrix[indice];
            indice++;
        }
    }

    _pTrans->setFromOpenGLMatrix ( ponteiroFloat );
}

void Loader::createNode ( tinyxml2::XMLElement* _nNodeXML, Node *_pNode ) {

    const char* l_id = _nNodeXML->Attribute ( "id" );
    const char* l_nome = _nNodeXML->Attribute ( "name" );

    Node *pFilho = _pNode;

    tinyxml2::XMLElement* l_nNodeVal = _nNodeXML->FirstChildElement();

    std::queue<btTransform*> pilhaTransformacao;

    while ( l_nNodeVal != nullptr ) {

        const char* l_nomeElemento = l_nNodeVal->Value();

        if ( strcmp ( l_nomeElemento,"matrix" ) == 0 ) {

            btTransform *l_pTransform = new btTransform;

            std::vector<float> l_arrayValores;

            const char* l_matrix = l_nNodeVal->GetText();
            loadArrayBtScalar ( l_matrix, l_arrayValores );

            carregaMatrix ( l_pTransform, l_arrayValores );

            pilhaTransformacao.push ( l_pTransform );

        } else if ( strcmp ( l_nomeElemento, "instance_camera" ) ==0 ) {

            const char* l_url = l_nNodeVal->Attribute ( "url" );

            btTransform* pTrans = pilhaTransformacao.front();
            pilhaTransformacao.pop();

            Camera *pCam = ( Camera* ) Node::findNodeById ( EntityKind::CAMERA, ( char* ) &l_url[1] );
            if ( pCam!=nullptr ) {

                Camera *pCamScena = nullptr;
                pCam->clone ( ( Node** ) &pCamScena );
                pCamScena->setTransform ( *pTrans );

                _pNode->addChild ( pCamScena );
                pFilho = pCamScena;

            }

            delete pTrans;

        } else if ( strcmp ( l_nomeElemento, "instance_light" ) ==0 ) {

            const char* l_url = l_nNodeVal->Attribute ( "url" );

            btTransform* pTrans = pilhaTransformacao.front();
            pilhaTransformacao.pop();

            Light *pLight = ( Light* ) Node::findNodeById ( EntityKind::LIGHT, ( char* ) &l_url[1] );
            if ( pLight!=nullptr ) {

                Light *pLightScena = nullptr;
                pLight->clone ( ( Node** ) &pLightScena );
                pLight->setTransform ( *pTrans );

                _pNode->addChild ( pLightScena );
                pFilho = pLightScena;

            }

            delete pTrans;

        } else if ( strcmp ( l_nomeElemento, "instance_geometry" ) ==0 ) {


            btTransform* pTrans = pilhaTransformacao.front();
            pilhaTransformacao.pop();

            DrawTriMesh *pDrawTriMesh =nullptr;
            Material *pMaterial = nullptr;

            Object *pObj = new Object ( l_id ,l_nome );
            pObj->setTransform ( *pTrans );

            const char* l_url = l_nNodeVal->Attribute ( "url" );
            pDrawTriMesh = ( DrawTriMesh* ) Node::findNodeById ( EntityKind::DRAW, ( char* ) &l_url[1] );

            tinyxml2::XMLElement* l_nBindMat =  l_nNodeVal->FirstChildElement ( "bind_material" );
            if ( l_nBindMat != nullptr ) {
                tinyxml2::XMLElement* l_nTecnicCommon =  l_nBindMat->FirstChildElement ( "technique_common" );
                if ( l_nTecnicCommon != nullptr ) {
                    tinyxml2::XMLElement* l_nInstanceMaterial =  l_nTecnicCommon->FirstChildElement ( "instance_material" );
                    if ( l_nInstanceMaterial != nullptr ) {

                        const char *l_target = l_nInstanceMaterial->Attribute ( "target" );
                        pMaterial = ( Material* ) Node::findNodeById ( EntityKind::MATERIAL, ( char* ) &l_target[1] );

                    }
                }
            }

            //Se ha material carregue
            if ( pMaterial != nullptr ) {

                //Se ha effeito (color_material) carregue
                if ( pMaterial->pEffect != nullptr ) {
                    Effect *novoEffect = nullptr;
                    pMaterial->pEffect->clone ( ( Node** ) &novoEffect );
                    pObj->addChild ( novoEffect );
                }

                //Se há textura carregue
                if ( pMaterial->pTextura != nullptr ) {

                    Texture *novaTextura = nullptr;
                    pMaterial->pTextura->clone ( ( Node** ) &novaTextura );
                    pObj->addChild ( novaTextura );
                }
            }

            //se há trimesh carregue
            if ( pDrawTriMesh ) {
                DrawTriMesh *nodeFinal = nullptr;
                pDrawTriMesh->clone ( ( Node** ) &nodeFinal );
                pObj->addChild ( nodeFinal );
            }

            //ajusta ele como filho atual
            pFilho = pObj;
            _pNode->addChild ( pObj );

            delete pTrans;

        } else if ( strcmp ( l_nomeElemento, "node" ) ==0 ) {

            Node *pBuffer = pFilho;
            createNode ( l_nNodeVal,pFilho );
            pFilho = pBuffer;

        }

        l_nNodeVal = l_nNodeVal->NextSiblingElement();
    }
}

void Loader::libPhysicsMaterial ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_materials" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "physics_material" );
        while ( l_nNode != nullptr ) {

            const char* l_id = l_nNode->Attribute ( "id" );
            const char *l_nome = l_nNode->Attribute ( "name" );

            btMaterial *pPMat = new btMaterial;
            m_pPhMaterial[ l_nome ] = pPMat;

            tinyxml2::XMLElement* l_nTec = l_nNode->FirstChildElement ( "technique_common" );
            if ( l_nTec != nullptr ) {

                tinyxml2::XMLElement* l_nFric = l_nTec->FirstChildElement ( "dynamic_friction" );
                if ( l_nFric != nullptr ) {
                    const char *l_fric = l_nFric->GetText();
                    if ( l_fric !=nullptr )
                        pPMat->m_friction = atof ( l_fric );
                }

                tinyxml2::XMLElement* l_nRes = l_nTec->FirstChildElement ( "restitution" );
                if ( l_nRes != nullptr ) {
                    const char *l_res = l_nRes->GetText();
                    if ( l_res !=nullptr )
                        pPMat->m_friction = atof ( l_res );
                }

            }

            l_nNode = l_nNode->NextSiblingElement ( "physics_material" );
        }
    }
}

void Loader::libPhysicsModels () {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_models" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "physics_model" );
        if ( l_nNode != nullptr ) {
            l_nNode = l_nNode->FirstChildElement ( "rigid_body" );
            while ( l_nNode != nullptr ) {

                Physics *pPhysc = new Physics ( l_nNode->Attribute ( "name" ),l_nNode->Attribute ( "sid" ) );
                listaNodeRemover.push ( pPhysc );

                tinyxml2::XMLElement* l_nTecnicCommon = l_nNode->FirstChildElement ( "technique_common" );
                if ( l_nTecnicCommon != nullptr ) {

                    tinyxml2::XMLElement* l_nMass = l_nTecnicCommon->FirstChildElement ( "mass" );
                    if ( l_nMass != nullptr ) {
                        const char* l_mass = l_nMass->GetText();
                        pPhysc->setMass ( atof ( l_mass ) );
                    }

                    tinyxml2::XMLElement* l_nInstaceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_physics_material" );
                    if ( l_nInstaceMaterial != nullptr ) {
                        const char* l_url = l_nInstaceMaterial->Attribute ( "url" );
                        if ( l_url != nullptr ) {

                            btMaterial *pPMat =m_pPhMaterial[ ( const char* ) &l_url[1]];
                            if ( pPMat ) {
                                pPhysc->setFriction ( pPMat->m_friction );
                                pPhysc->setRestitution ( pPMat->m_restitution );
                            }
                        }
                    }

                    //TODO carga do shape de colisao

                }

                l_nNode = l_nNode->NextSiblingElement ( "rigid_body" );
            }
        }
    }
}

void Loader::libPhysicsScenes ( void ) {

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_scenes" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "physics_scene" );
        if ( l_nNode != nullptr ) {
            l_nNode = l_nNode->FirstChildElement ( "instance_physics_model" );
            
            if (l_nNode != nullptr) {
                l_nNode = l_nNode->FirstChildElement ( "instance_rigid_body" );
                
                while ( l_nNode != nullptr ) {
                    
                    const char *l_body = l_nNode->Attribute("body");
                    const char *l_target = l_nNode->Attribute("target");
                    
                    Physics *pPhysic = ( Physics* ) Node::findNodeById ( EntityKind::PHYSICS, l_body ); //m_mPhysics[ ( const char* ) l_pBody];
                    Object *pObj = ( Object* ) Node::findNodeById ( EntityKind::OBJECT, &l_target[1] );
                    
                    if ( ( pPhysic ) && ( pObj ) ) {
                        
                        Physics *novoPhy = nullptr;
                        pPhysic->clone ( ( Node** )&novoPhy );
                        pObj->addChild ( novoPhy );
                        
                    }              
                    
                    l_nNode = l_nNode->NextSiblingElement ( "instance_rigid_body" );
                }                
            }
        }
    }

}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
