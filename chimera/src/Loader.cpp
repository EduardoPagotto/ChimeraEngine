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
//     libPhysicsMaterial();
//     libPhysicsModels();
//     libPhysicsScenes();

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

            pEffect->loadCollada(l_nNode);

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
           
            const char* l_val = l_nNode->FirstChildElement("init_from")->GetText();
            pTexture->setPathFile( m_imageDir + std::string(l_val) );
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
            
            const char* l_val = l_nNode->FirstChildElement("instance_effect")->Attribute("url");
            if (l_val != nullptr) {
             
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

            DrawTriMesh *pDrawTriMesh = new DrawTriMesh (retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode )  );
            listaNodeRemover.push ( pDrawTriMesh );            
            
            pDrawTriMesh->loadCollada(l_nNode);
            
            l_nNode = l_nNode->NextSiblingElement ( "geometry" );
        }
    }             
}

Node* Loader::libScene ( void ) {

    Node *pScene = nullptr;

    tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_visual_scenes" );
    if ( l_nNode != nullptr ) {
        l_nNode = l_nNode->FirstChildElement ( "visual_scene" );
        while ( l_nNode != nullptr ) {    
    
            pScene = new Node ( EntityKind::NODE, retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode )  );
            createNode ( l_nNode, pScene );            

        }
    }
    
//     xmlNodePtr l_nScene = findNode ( ( char* ) "library_visual_scenes", m_root );
//     if ( l_nScene!=nullptr ) {
//         l_nScene = findNode ( ( char* ) "visual_scene", l_nScene->children );
//         if ( l_nScene!=nullptr ) {
// 
//             pScene = new Node ( EntityKind::NODE, getAtributoXML ( "Scene","id",l_nScene ),getAtributoXML ( "Scene","name",l_nScene ) );
//             xmlNodePtr l_nNode = findNode ( ( char* ) "node",l_nScene->children );
//             createNode ( l_nNode, pScene );
//         }
//     }
// 
//     if ( m_numNodes == 0 ) {
//         LOG4CXX_WARN ( logger , "Nao ha Nodes de cena registrados!!!" );
//     } else {
//         LOG4CXX_INFO ( logger , std::string ( std::string ( "Nodes de cena Registrados:" ) + std::to_string ( m_numNodes ) ) );
//     }

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

void Loader::createNode (tinyxml2::XMLElement* _nNodeXML, Node *_pNode ) {
    
    tinyxml2::XMLElement* l_nNode = _nNodeXML->FirstChildElement("node");
    
    while (l_nNode != nullptr) {
        
        btTransform l_transform;
        Node *pFilho = _pNode;
   
        tinyxml2::XMLElement* l_nNodeVal = l_nNode->FirstChildElement();
        
        const char* l_nomeElemento = l_nNodeVal->Value();
        
        if (strcmp(l_nomeElemento,"matrix") == 0) {
            
            std::vector<float> l_arrayValores;
            
            const char* l_matrix = l_nNodeVal->GetText();
            loadArrayBtScalar(l_matrix, l_arrayValores);
            
            carregaMatrix(&l_transform, l_arrayValores);
            
        }
        
        l_nNodeVal = l_nNodeVal->NextSiblingElement();
        if (l_nNodeVal != nullptr) {
            
            const char* l_filho =  l_nNodeVal->Value();
            
            if (strcmp( l_filho , "node")==0) {
                
                createNode ( l_nNodeVal, pFilho );
                
            } else if ( strcmp(l_filho, "instance_camera")==0) {
                
                const char* l_url = l_nNodeVal->Attribute("url");
                
                Camera *pCam = ( Camera* ) Node::findNodeById ( EntityKind::CAMERA, ( char* ) &l_url[1] );
                if ( pCam!=nullptr ) {

                    Camera *pCamScena = nullptr;
                    pCam->clone ( ( Node** ) &pCamScena );
                    pCamScena->setTransform(l_transform);

                    _pNode->addChild ( pCamScena );
                    pFilho = pCamScena;

                }

                
            } else if ( strcmp( l_filho, "instance_light")==0) {
                
                const char* l_url = l_nNodeVal->Attribute("url");
                
                Light *pLight = ( Light* ) Node::findNodeById ( EntityKind::LIGHT, ( char* ) &l_url[1] );
                if ( pLight!=nullptr ) {
                    
                    Light *pLightScena = nullptr;
                    pLight->clone ( ( Node** ) &pLightScena );
                    pLight->setTransform(l_transform);
                    
                    _pNode->addChild ( pLightScena );
                    pFilho = pLightScena;
                    
                }
                
            } else if ( strcmp( l_filho, "instance_geometry")==0) {
                
                DrawTriMesh *pDrawTriMesh =nullptr;
                Material *pMaterial = nullptr;

                Object *pObj = new Object ( retornaAtributo ( "id",l_nNode ),retornaAtributo ( "name",l_nNode ) );
                pObj->setTransform ( l_transform );
                
                const char* l_url = l_nNodeVal->Attribute("url");
                pDrawTriMesh = ( DrawTriMesh* ) Node::findNodeById ( EntityKind::DRAW, ( char* ) &l_url[1] );
                
                tinyxml2::XMLElement* l_nBindMat =  l_nNodeVal->FirstChildElement("bind_material");
                if (l_nBindMat != nullptr) {
                    tinyxml2::XMLElement* l_nTecnicCommon =  l_nBindMat->FirstChildElement("technique_common");
                    if (l_nTecnicCommon != nullptr) {
                        tinyxml2::XMLElement* l_nInstanceMaterial =  l_nTecnicCommon->FirstChildElement("instance_material");
                        if (l_nInstanceMaterial != nullptr) {
                            
                            const char *l_target = l_nInstanceMaterial->Attribute("target");
                            pMaterial = ( Material* ) Node::findNodeById ( EntityKind::MATERIAL,( char* ) &l_target[1] );
                           
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
            } 
            
            createNode(l_nNodeVal,pFilho);
           
        }
        
        
        l_nNode = l_nNode->NextSiblingElement("node");
    }
    
    
    
//     xmlNodePtr l_nNode = _nodeXML;//findNode((char*)"node", _nodeXML->children);
// 
//     while ( l_nNode ) {
// 
//         if ( l_nNode->type==XML_ELEMENT_NODE ) {
//             xmlChar* l_pVal = nullptr;
//             xmlChar *pSID = nullptr;
//             Node *pFilho = nullptr;
//             m_numNodes++;
//             std::vector<float> l_arrayValores;
//             const xmlChar *l_pNomeNode = l_nNode->name;
// 
//             btTransform l_transform;
// 
//             xmlNodePtr l_nTransforma = findNode ( ( char* ) "matrix", l_nNode->children );
// 
//             if ( l_nTransforma ) {
//                 pSID = xmlGetProp ( l_nTransforma, ( const xmlChar* ) "sid" );
//                 if ( xmlStrcmp ( pSID, ( xmlChar* ) "transform" ) == 0 ) {
//                     l_pVal = xmlNodeListGetString ( m_doc, l_nTransforma->children, 1 );
//                     loadArrayF ( ( char* ) l_pVal ,l_arrayValores );
//                     carregaMatrix ( &l_transform, l_arrayValores );
//                     l_arrayValores.clear();
//                     xmlFree ( l_pVal );
//                 }
//             }
// 
//             xmlNodePtr l_nInstance;;
//             if ( l_nInstance = findNode ( ( char* ) "instance_geometry", l_nNode->children ) ) {
// 
//                 DrawTriMesh *pDrawTriMesh =nullptr;
//                 Material *pMaterial = nullptr;
// 
//                 Object *pObj = new Object ( getAtributoXML ( "Node","id",l_nNode ),getAtributoXML ( "Node","name",l_nNode ) );
//                 pObj->setTransform ( l_transform );
// 
//                 xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
//                 if ( pURL != nullptr ) {
//                     pDrawTriMesh = ( DrawTriMesh* ) Node::findNodeById ( EntityKind::DRAW, ( char* ) &pURL[1] ); //m_mDrawTriMesh[ l_nomeDrawTriMesh];
// 
//                     if ( l_nInstance->children ) {
//                         xmlNodePtr l_nBindMat = findNode ( ( char* ) "bind_material", l_nInstance->children );
//                         if ( l_nBindMat ) {
//                             l_nBindMat = findNode ( ( char* ) "technique_common", l_nBindMat->children );
//                             if ( l_nBindMat ) {
//                                 l_nBindMat = findNode ( ( char* ) "instance_material", l_nBindMat->children );
//                                 if ( l_nBindMat ) {
//                                     pURL = xmlGetProp ( l_nBindMat, ( const xmlChar* ) "target" );
//                                     if ( pURL )
//                                         pMaterial = ( Material* ) Node::findNodeById ( EntityKind::MATERIAL,( char* ) &pURL[1] );
//                                 }
//                             }
//                         }
//                     }
// 
//                 }
// 
//                 //Se ha material carregue
//                 if ( pMaterial != nullptr ) {
// 
//                     //Se ha effeito (color_material) carregue
//                     if ( pMaterial->pEffect != nullptr ) {
// 
//                         Effect *novoEffect = nullptr;
//                         pMaterial->pEffect->clone ( ( Node** ) &novoEffect );
//                         pObj->addChild ( novoEffect );
//                         //pObj->pEffect = (Effect*)clone( pMaterial->pEffect );
//                     }
// 
//                     //Se há textura carregue
//                     if ( pMaterial->pTextura != nullptr ) {
// 
//                         Texture *novaTextura = nullptr;
//                         pMaterial->pTextura->clone ( ( Node** ) &novaTextura );
//                         pObj->addChild ( novaTextura );
//                         //pObj->pTexture = (Texture*)clone(pMaterial->pTextura);
//                     }
//                 }
// 
//                 //se há trimesh carregue
//                 if ( pDrawTriMesh ) {
//                     DrawTriMesh *nodeFinal = nullptr;
//                     pDrawTriMesh->clone ( ( Node** ) &nodeFinal );
//                     pObj->addChild ( nodeFinal );
//                 }
// 
//                 //ajusta ele como filho atual
//                 pFilho = pObj;
// 
//                 //Carrega OBJ na cena
//                 _pNode->addChild ( pObj );
// 
//             } else if ( l_nInstance = findNode ( ( char* ) "instance_light", l_nNode->children ) ) {
//                 xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
//                 if ( pURL != nullptr ) {
// 
//                     Light *pLight = ( Light* ) Node::findNodeById ( EntityKind::LIGHT, ( char* ) &pURL[1] );
//                     if ( pLight!=nullptr ) {
// 
//                         Light *pLightScene = nullptr;
//                         pLight->clone ( ( Node** ) &pLightScene );
//                         pLightScene->transform = l_transform;
// 
//                         _pNode->addChild ( pLightScene );
//                         pFilho = pLightScene;
// 
//                     }
//                 }
//             } else if ( l_nInstance = findNode ( ( char* ) "instance_camera", l_nNode->children ) ) {
//                 xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
//                 if ( pURL != nullptr ) {
// 
//                     Camera *pCam = ( Camera* ) Node::findNodeById ( EntityKind::CAMERA, ( char* ) &pURL[1] );
//                     if ( pCam!=nullptr ) {
// 
//                         Camera *pCamScena = nullptr;//( Camera * ) clone ( pCam ); //Camera *pCamScena = new Camera ( *pCam );
//                         pCam->clone ( ( Node** ) &pCamScena );
//                         pCamScena->transform = l_transform;
// 
//                         _pNode->addChild ( pCamScena );
//                         pFilho = pCamScena;
// 
//                     }
//                 }
//             }
// 
//             Node* l_buffer = _pNode;
//             createNode ( l_nInstance->next, pFilho );
//             _pNode = l_buffer;
//         }
//         l_nNode = l_nNode->next;
//     }
}
//
// void Loader::libPhysicsMaterial ( void ) {
//     unsigned l_num = 0;
//     xmlNodePtr l_nPhMat = findNode ( ( char* ) "library_physics_materials", m_root );
//     if ( l_nPhMat ) {
//         l_nPhMat =  findNode ( ( char* ) "physics_material",l_nPhMat->children );
//         while ( l_nPhMat ) {
//             l_num++;
//             xmlChar *l_pName = xmlGetProp ( l_nPhMat, ( const xmlChar* ) "name" );
//
//             btMaterial *pPMat = new btMaterial;
//             m_pPhMaterial[ ( const char* ) l_pName] = pPMat;
//
//             xmlNodePtr l_nTec =  findNode ( ( char* ) "technique_common",l_nPhMat->children );
//             if ( l_nTec ) {
//                 xmlNodePtr l_nFric =  findNode ( ( char* ) "dynamic_friction",l_nTec->children );
//                 if ( l_nFric ) {
//                     xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nFric->children, 1 );
//                     if ( l_pVal ) {
//                         pPMat->m_friction = atof ( ( const char* ) l_pVal );
//                         xmlFree ( l_pVal );
//                     }
//                 }
//
//                 l_nFric =  findNode ( ( char* ) "restitution",l_nTec->children );
//                 if ( l_nFric ) {
//                     xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nFric->children, 1 );
//                     if ( l_pVal ) {
//                         pPMat->m_restitution = atof ( ( const char* ) l_pVal );
//                         xmlFree ( l_pVal );
//                     }
//                 }
//             }
//
//             l_nPhMat =  findNode ( ( char* ) "physics_material",l_nPhMat->next );
//         }
//     }
//
//     if ( l_num == 0 ) {
//         LOG4CXX_WARN ( logger , "Nao ha Material Fisico registrada!!!" );
//     } else {
//         LOG4CXX_INFO ( logger , std::string ( std::string ( "Materia Fisico Registradas:" ) + std::to_string ( l_num ) ) );
//     }
// }
//
// void Loader::libPhysicsModels () {
//
//     int l_count = 0;
//     xmlNodePtr l_nPhysics = findNode ( ( char* ) "library_physics_models", m_root );
//     if ( l_nPhysics ) {
//         l_nPhysics = findNode ( ( char* ) "physics_model", l_nPhysics->children );
//         if ( l_nPhysics ) {
//             l_nPhysics = findNode ( ( char* ) "rigid_body", l_nPhysics );
//             while ( l_nPhysics ) {
//                 l_count++;
//                 xmlChar *l_pName = xmlGetProp ( l_nPhysics, ( const xmlChar* ) "name" );
//                 xmlChar *l_pSid = xmlGetProp ( l_nPhysics, ( const xmlChar* ) "sid" );
//                 if ( ( l_pName ) && ( l_pSid ) ) {
//
//                     Physics *pPhysc = new Physics ( std::string ( ( char* ) l_pName ),std::string ( ( char* ) l_pSid ) );
//                     listaNodeRemover.push ( pPhysc );
//
//                     xmlNodePtr l_nTec = findNode ( ( char* ) "technique_common", l_nPhysics->children );
//                     if ( l_nTec ) {
//                         xmlNodePtr l_nProp = findNode ( ( char* ) "dynamic", l_nTec->children );
//                         if ( l_nProp ) {
//                             xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nProp->children, 1 );
//                             if ( l_pVal ) {
//                                 if ( strcmp ( ( const char* ) l_pVal, ( const char* ) "true" ) ==0 ) {
//                                     l_nProp = findNode ( ( char* ) "mass", l_nProp );
//                                     if ( l_nProp ) {
//                                         xmlFree ( l_pVal );
//                                         l_pVal = xmlNodeListGetString ( m_doc, l_nProp->children, 1 );
//                                         if ( l_pVal )
//                                             pPhysc->setMass ( ( float ) atof ( ( char* ) l_pVal ) );
//                                         //TODO carga do shape de colisao
//                                     }
//                                 }
//                                 xmlFree ( l_pVal );
//                             }
//                         }
//
//                         l_nProp = findNode ( ( char* ) "instance_physics_material", l_nTec->children );
//                         if ( l_nProp ) {
//                             xmlChar *l_pName = xmlGetProp ( l_nProp, ( const xmlChar* ) "url" );
//                             if ( l_pName ) {
//                                 btMaterial *pPMat =m_pPhMaterial[ ( const char* ) &l_pName[1]];
//                                 if ( pPMat ) {
//                                     pPhysc->setFriction ( pPMat->m_friction );
//                                     pPhysc->setRestitution ( pPMat->m_restitution );
//                                 }
//                             }
//                         }
//                     }
//                 }
//                 l_nPhysics = findNode ( ( char* ) "rigid_body", l_nPhysics->next );
//             }
//         }
//     }
//
//     if ( l_count == 0 ) {
//         LOG4CXX_WARN ( logger , "Nao ha Modelo Fisico registrada!!!" );
//     } else {
//         LOG4CXX_INFO ( logger , std::string ( std::string ( "Modelo Fisico Registradas:" ) + std::to_string ( l_count ) ) );
//     }
//
// }
//
// void Loader::libPhysicsScenes ( void ) {
//     unsigned l_num=0;
//     xmlNodePtr l_nPhyScene = findNode ( ( char* ) "library_physics_scenes", m_root );
//     if ( l_nPhyScene!=nullptr ) {
//         l_nPhyScene = findNode ( ( char* ) "instance_physics_model", l_nPhyScene->children );
//         if ( l_nPhyScene ) {
//             xmlNodePtr l_nRigid = findNode ( ( char* ) "instance_rigid_body", l_nPhyScene->children );
//             while ( l_nRigid ) {
//                 xmlChar *l_pBody = xmlGetProp ( l_nRigid, ( const xmlChar* ) "body" );
//                 xmlChar *l_pTarget = xmlGetProp ( l_nRigid, ( const xmlChar* ) "target" );
//                 if ( ( l_pBody ) && ( l_pTarget ) ) {
//
//                     Physics *pPhysic = ( Physics* ) Node::findNodeById ( EntityKind::PHYSICS, std::string ( ( char* ) l_pBody ) ); //m_mPhysics[ ( const char* ) l_pBody];
//                     Object *pObj = ( Object* ) Node::findNodeById ( EntityKind::OBJECT, ( const char* ) &l_pTarget[1] );
//
//                     if ( ( pPhysic ) && ( pObj ) ) {
//
//                         Physics *novoPhy = nullptr;
//                         pPhysic->clone ( ( Node** )&novoPhy );
//                         pObj->addChild ( novoPhy );
//
//                         l_num++;
//                     }
//                 }
//                 l_nRigid = findNode ( ( char* ) "instance_rigid_body",l_nRigid->next );
//             }
//         }
//     }
//
//     if ( l_num == 0 ) {
//         LOG4CXX_WARN ( logger , "Nao ha Modelo Fisico registrada!!!" );
//     } else {
//         LOG4CXX_INFO ( logger , std::string ( std::string ( "Modelo Fisico Registradas:" ) + std::to_string ( l_num ) ) );
//     }
// }
//
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
