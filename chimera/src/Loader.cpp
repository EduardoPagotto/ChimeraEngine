// #include "Loader.h"
// 
// #include "ChimeraUtils.h"
// 
// namespace Chimera {
// 
// Loader::Loader() {
// 
//     doc = nullptr;
//     root = nullptr;
//     //logger = log4cxx::Logger::getLogger ( "Loader" );
// 
// }
// 
// Loader::~Loader() {
// 
//     if ( doc != nullptr ) {
//         delete doc;
//         doc = nullptr;
//     }
// 
//     while ( !listaNodeRemover.empty() ) {
//         Graph::Node *pNode = listaNodeRemover.front();
//         listaNodeRemover.pop();
//         delete pNode;
//         pNode = nullptr;
//     }
// 
// }
// 
// Graph::Node* Loader::loadDAE ( const std::string &_file ) {
// 
//     Graph::Node *pRootScene = nullptr;
//     std::string l_nomeArquivo = m_modelDir + _file;
// 
//     doc = new tinyxml2::XMLDocument();
//     doc->LoadFile ( l_nomeArquivo.c_str() );
//     root = doc->FirstChildElement ( "COLLADA" );
//     if ( root == nullptr ) {
//         throw ExceptionChimera ( ExceptionCode::READ, "Falha Doc XML:" + _file + std::string ( " :" ) + doc->GetErrorStr1() );
//     }
// 
//     libCam();
//     libLight();
//     libEffect();
//     libTexture();
//     libMaterial();
//     libGeometry();
//     pRootScene = libScene();//Carrega componentes da cena
//     libPhysicsMaterial();
//     libPhysicsModels();
// 
//     libConstraint();
// 
//     libPhysicsScenes();
// 
//     return pRootScene;
// }
// 
// std::string Loader::retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node ) {
// 
//     const char *l_value = _node->Attribute ( _atributo.c_str() );
//     if ( l_value != nullptr ) {
//         return std::string ( l_value );
//     }
// 
//     //LOG4CXX_WARN ( logger , std::string ( "Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado" ) );
// 
//     return std::string ( "" );
// }
// 
// void Loader::libCam ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_cameras" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "camera" );
//         while ( l_nNode != nullptr ) {
// 
//             Graph::Camera *pCamera = new Graph::Camera ( Graph::CameraType::Base, retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
//             listaNodeRemover.push ( pCamera );
// 
//             pCamera->loadCollada ( l_nNode );
// 
//             l_nNode = l_nNode->NextSiblingElement ( "camera" );
//         }
//     }
// }
// 
// void Loader::libLight() {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_lights" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "light" );
//         while ( l_nNode != nullptr ) {
// 
//             Graph::Light *pLight = new Graph::Light ( Graph::LightType::POINT, retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
//             listaNodeRemover.push ( pLight ); //Salva na lista de remocao
// 
//             pLight->loadCollada ( l_nNode );
// 
//             l_nNode = l_nNode->NextSiblingElement ( "light" );
//         }
//     }
// }
// 
// void Loader::libEffect ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_effects" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "effect" );
//         while ( l_nNode != nullptr ) {
// 
//             Graph::Effect *pEffect = new Graph::Effect ( retornaAtributo ( "id", l_nNode ), "Effect" );
//             listaNodeRemover.push ( pEffect );
// 
//             pEffect->loadCollada ( l_nNode );
// 
//             l_nNode = l_nNode->NextSiblingElement ( "effect" );
//         }
// 
//     }
// }
// 
// void Loader::libTexture ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_images" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "image" );
//         while ( l_nNode != nullptr ) {
//             Graph::Texture *pTexture = new Graph::Texture ( retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
//             listaNodeRemover.push ( pTexture );
// 
//             const char* l_val = l_nNode->FirstChildElement ( "init_from" )->GetText();
//             pTexture->setPathFile ( m_imageDir + std::string ( l_val ) );
//             pTexture->init();
// 
//             l_nNode = l_nNode->NextSiblingElement ( "image" );
//         }
//     }
// }
// 
// void Loader::libMaterial ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_materials" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "material" );
//         while ( l_nNode != nullptr ) {
// 
//             Graph::Material *pMaterial = new Graph::Material ( retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
//             listaNodeRemover.push ( pMaterial );
// 
//             const char* l_val = l_nNode->FirstChildElement ( "instance_effect" )->Attribute ( "url" );
//             if ( l_val != nullptr ) {
// 
//                 Graph::Effect *pEffe = ( Graph::Effect* ) Graph::Node::findNodeById ( EntityKind::EFFECT, &l_val[1] ); //m_mEffect[ l_nomeEffect ];
//                 if ( pEffe ) {
//                     pMaterial->pEffect = pEffe;
//                     if ( pEffe->getNameTextureId().size() > 0 ) {
// 
//                         Graph::Texture *pTexture = ( Graph::Texture* ) Graph::Node::findNodeById ( EntityKind::TEXTURE, pEffe->getNameTextureId() ); //m_mTextura[pEffe->getNameTextureId()];//( Texture* ) Node::findNodeById ( EntityKind::IMAGE, pEffe->getNameTextureId() );
//                         if ( pTexture != nullptr ) {
//                             pMaterial->pTextura = pTexture;//new Texture( *pTexture );
//                         }
// 
//                     }
//                 }
// 
//             }
// 
//             l_nNode = l_nNode->NextSiblingElement ( "material" );
//         }
//     }
// }
// 
// void Loader::libGeometry ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_geometries" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "geometry" );
//         while ( l_nNode != nullptr ) {
// 
//             Graph::DrawTriMesh *pDrawTriMesh = new Graph::DrawTriMesh ( retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
//             listaNodeRemover.push ( pDrawTriMesh );
// 
//             pDrawTriMesh->loadCollada ( l_nNode );
// 
//             l_nNode = l_nNode->NextSiblingElement ( "geometry" );
//         }
//     }
// }
// 
// Graph::Node* Loader::libScene ( void ) {
// 
//     Graph::Node *pScene = nullptr;
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_visual_scenes" );
//     if ( l_nNode != nullptr ) {
// 
//         l_nNode = l_nNode->FirstChildElement ( "visual_scene" );
//         pScene = new Graph::Node ( EntityKind::NODE, retornaAtributo ( "id", l_nNode ), retornaAtributo ( "name", l_nNode ) );
// 
//         l_nNode = l_nNode->FirstChildElement ( "node" );
//         while ( l_nNode != nullptr ) {
// 
//             createNode ( l_nNode, pScene );
//             l_nNode = l_nNode->NextSiblingElement ( "node" );
// 
//         }
//     }
// 
//     return pScene;
// }
// 
// void Loader::carregaMatrix ( btTransform *_pTrans, const std::vector<float> &listaMatrix ) {
// 
//     if ( listaMatrix.size() != 16 ) {
//         throw ExceptionChimera ( ExceptionCode::READ, "Tamanho da Matrix invalido" + std::to_string ( listaMatrix.size() ) );
//     }
// 
//     btScalar ponteiroFloat[16];
//     int indice = 0;
//     for ( int i = 0; i < 4; i++ ) {
//         for ( int j = 0; j < 4; j++ ) {
//             int pos = i + ( 4 * j );
//             ponteiroFloat[pos] = listaMatrix[indice];
//             indice++;
//         }
//     }
// 
//     _pTrans->setFromOpenGLMatrix ( ponteiroFloat );
// }
// 
// void Loader::createNode ( tinyxml2::XMLElement* _nNodeXML, Graph::Node *_pNode ) {
// 
//     const char* l_id = _nNodeXML->Attribute ( "id" );
//     const char* l_nome = _nNodeXML->Attribute ( "name" );
// 
//     Graph::Node *pFilho = _pNode;
// 
//     tinyxml2::XMLElement* l_nNodeVal = _nNodeXML->FirstChildElement();
// 
//     std::queue<btTransform*> pilhaTransformacao;
// 
//     while ( l_nNodeVal != nullptr ) {
// 
//         const char* l_nomeElemento = l_nNodeVal->Value();
// 
//         if ( strcmp ( l_nomeElemento, "matrix" ) == 0 ) {
// 
//             btTransform *l_pTransform = new btTransform;
// 
//             std::vector<float> l_arrayValores;
// 
//             const char* l_matrix = l_nNodeVal->GetText();
//             loadArrayBtScalar ( l_matrix, l_arrayValores );
// 
//             carregaMatrix ( l_pTransform, l_arrayValores );
// 
//             pilhaTransformacao.push ( l_pTransform );
// 
//         } else if ( strcmp ( l_nomeElemento, "instance_camera" ) == 0 ) {
// 
//             const char* l_url = l_nNodeVal->Attribute ( "url" );
// 
//             btTransform* pTrans = pilhaTransformacao.front();
//             pilhaTransformacao.pop();
// 
//             Graph::Camera *pCam = ( Graph::Camera* ) Graph::Node::findNodeById ( EntityKind::CAMERA, ( char* ) &l_url[1] );
//             if ( pCam != nullptr ) {
// 
//                 Graph::Camera *pCamScena = nullptr;
//                 pCam->clone ( ( Graph::Node** ) &pCamScena );
//                 pCamScena->setTransform ( *pTrans );
// 
//                 _pNode->addChild ( pCamScena );
//                 pFilho = pCamScena;
// 
//             }
// 
//             delete pTrans;
// 
//         } else if ( strcmp ( l_nomeElemento, "instance_light" ) == 0 ) {
// 
//             const char* l_url = l_nNodeVal->Attribute ( "url" );
// 
//             btTransform* pTrans = pilhaTransformacao.front();
//             pilhaTransformacao.pop();
// 
//             Graph::Light *pLight = ( Graph::Light* ) Graph::Node::findNodeById ( EntityKind::LIGHT, ( char* ) &l_url[1] );
//             if ( pLight != nullptr ) {
// 
//                 Graph::Light *pLightScena = nullptr;
//                 pLight->clone ( ( Graph::Node** ) &pLightScena );
//                 pLightScena->setTransform ( *pTrans );
// 
//                 _pNode->addChild ( pLightScena );
//                 pFilho = pLightScena;
// 
//             }
// 
//             delete pTrans;
// 
//         } else if ( strcmp ( l_nomeElemento, "instance_geometry" ) == 0 ) {
// 
// 
//             btTransform* pTrans = pilhaTransformacao.front();
//             pilhaTransformacao.pop();
// 
//             Graph::DrawTriMesh *pDrawTriMesh = nullptr;
//             Graph::Material *pMaterial = nullptr;
// 
//             Graph::Object *pObj = new Graph::Object ( l_id, l_nome );
//             pObj->setTransform ( *pTrans );
// 
//             const char* l_url = l_nNodeVal->Attribute ( "url" );
//             pDrawTriMesh = ( Graph::DrawTriMesh* ) Graph::Node::findNodeById ( EntityKind::DRAW, ( char* ) &l_url[1] );
// 
//             tinyxml2::XMLElement* l_nBindMat = l_nNodeVal->FirstChildElement ( "bind_material" );
//             if ( l_nBindMat != nullptr ) {
//                 tinyxml2::XMLElement* l_nTecnicCommon = l_nBindMat->FirstChildElement ( "technique_common" );
//                 if ( l_nTecnicCommon != nullptr ) {
//                     tinyxml2::XMLElement* l_nInstanceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_material" );
//                     if ( l_nInstanceMaterial != nullptr ) {
// 
//                         const char *l_target = l_nInstanceMaterial->Attribute ( "target" );
//                         pMaterial = ( Graph::Material* ) Graph::Node::findNodeById ( EntityKind::MATERIAL, ( char* ) &l_target[1] );
// 
//                     }
//                 }
//             }
// 
//             //Se ha material carregue
//             if ( pMaterial != nullptr ) {
// 
//                 //Se ha effeito (color_material) carregue
//                 if ( pMaterial->pEffect != nullptr ) {
//                     Graph::Effect *novoEffect = nullptr;
//                     pMaterial->pEffect->clone ( ( Graph::Node** ) &novoEffect );
//                     pObj->addChild ( novoEffect );
//                 }
// 
//                 //Se há textura carregue
//                 if ( pMaterial->pTextura != nullptr ) {
// 
//                     Graph::Texture *novaTextura = nullptr;
//                     pMaterial->pTextura->clone ( ( Graph::Entity** ) &novaTextura );
//                     pObj->addChild ( novaTextura );
//                 }
//             }
// 
//             //se há trimesh carregue
//             if ( pDrawTriMesh ) {
//                 Graph::DrawTriMesh *nodeFinal = nullptr;
//                 pDrawTriMesh->clone ( ( Graph::Node** ) &nodeFinal );
//                 pObj->addChild ( nodeFinal );
//             }
// 
//             //ajusta ele como filho atual
//             pFilho = pObj;
//             _pNode->addChild ( pObj );
// 
//             delete pTrans;
// 
//         } else if ( strcmp ( l_nomeElemento, "node" ) == 0 ) {
// 
//             Graph::Node *pBuffer = pFilho;
//             createNode ( l_nNodeVal, pFilho );
//             pFilho = pBuffer;
// 
//         }
// 
//         l_nNodeVal = l_nNodeVal->NextSiblingElement();
//     }
// }
// 
// void Loader::libPhysicsMaterial ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_materials" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "physics_material" );
//         while ( l_nNode != nullptr ) {
// 
//             const char* l_id = l_nNode->Attribute ( "id" );
//             const char *l_nome = l_nNode->Attribute ( "name" );
// 
//             btMaterial *pPMat = new btMaterial;
//             pPMat->m_friction = 0;
//             pPMat->m_restitution = 0;
// 
//             m_pPhMaterial[l_nome] = pPMat;
// 
//             tinyxml2::XMLElement* l_nTec = l_nNode->FirstChildElement ( "technique_common" );
//             if ( l_nTec != nullptr ) {
// 
//                 tinyxml2::XMLElement* l_nFric = l_nTec->FirstChildElement ( "dynamic_friction" );
//                 if ( l_nFric != nullptr ) {
//                     const char *l_fric = l_nFric->GetText();
//                     if ( l_fric != nullptr ) {
//                         pPMat->m_friction = atof ( l_fric );
//                     }
//                 }
// 
//                 tinyxml2::XMLElement* l_nRes = l_nTec->FirstChildElement ( "restitution" );
//                 if ( l_nRes != nullptr ) {
//                     const char *l_res = l_nRes->GetText();
//                     if ( l_res != nullptr ) {
//                         pPMat->m_restitution = atof ( l_res );
//                     }
//                 }
// 
//             }
// 
//             l_nNode = l_nNode->NextSiblingElement ( "physics_material" );
//         }
//     }
// }
// 
// void Loader::libConstraint() {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_models" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "physics_model" );
//         if ( l_nNode != nullptr ) {
//             l_nNode = l_nNode->FirstChildElement ( "rigid_constraint" );
//             while ( l_nNode != nullptr ) {
// 
//                 Graph::Constraint *pConstraint = new Graph::Constraint ( l_nNode->Attribute ( "name" ), l_nNode->Attribute ( "sid" ) );
//                 listaNodeRemover.push ( pConstraint );
// 
//                 tinyxml2::XMLElement* l_nRefAttachent = l_nNode->FirstChildElement ( "ref_attachment" );
//                 if ( l_nRefAttachent != nullptr ) {
// 
//                     const char *l_rigidA = l_nRefAttachent->Attribute ( "rigid_body" );
// 
//                     Graph::Physics *pPhysc = ( Graph::Physics* ) Graph::Node::findNodeById ( EntityKind::PHYSICS, l_rigidA );
//                     pConstraint->pPhysicsA = pPhysc;
// 
//                     tinyxml2::XMLElement* l_nTrans = l_nRefAttachent->FirstChildElement ( "matrix" );
//                     if ( l_nTrans != nullptr ) {
// 
//                         btTransform transform;
// 
//                         std::vector<float> l_arrayValores;
// 
//                         const char* l_matrix = l_nTrans->GetText();
//                         loadArrayBtScalar ( l_matrix, l_arrayValores );
// 
//                         carregaMatrix ( &transform, l_arrayValores );
// 
//                         pConstraint->transformA = transform;
// 
//                     }
// 
//                     tinyxml2::XMLElement* l_nAttachent = l_nNode->FirstChildElement ( "attachment" );
//                     if ( l_nAttachent != nullptr ) {
// 
//                         const char *l_rigidB = l_nAttachent->Attribute ( "rigid_body" );
// 
//                         Graph::Physics *pPhysc = ( Graph::Physics* ) Graph::Node::findNodeById ( EntityKind::PHYSICS, l_rigidB );
//                         pConstraint->pPhysicsB = pPhysc;
// 
//                         tinyxml2::XMLElement* l_nTrans = l_nAttachent->FirstChildElement ( "matrix" );
//                         if ( l_nTrans != nullptr ) {
// 
//                             btTransform transform;
// 
//                             std::vector<float> l_arrayValores;
// 
//                             const char* l_matrix = l_nTrans->GetText();
//                             loadArrayBtScalar ( l_matrix, l_arrayValores );
// 
//                             carregaMatrix ( &transform, l_arrayValores );
// 
//                             pConstraint->transformB = transform;
// 
//                         }
//                     }
//                 }
//                 l_nNode = l_nNode->NextSiblingElement ( "rigid_constraint" );
//             }
//         }
//     }
// 
// }
// 
// void Loader::libPhysicsModels() {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_models" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "physics_model" );
//         if ( l_nNode != nullptr ) {
//             l_nNode = l_nNode->FirstChildElement ( "rigid_body" );
//             while ( l_nNode != nullptr ) {
// 
//                 Graph::Physics *pPhysc = new Graph::Physics ( l_nNode->Attribute ( "name" ), l_nNode->Attribute ( "sid" ) );
//                 //listaNodeRemover.push(pPhysc);
// 
//                 tinyxml2::XMLElement* l_nTecnicCommon = l_nNode->FirstChildElement ( "technique_common" );
//                 if ( l_nTecnicCommon != nullptr ) {
// 
//                     tinyxml2::XMLElement* l_nMass = l_nTecnicCommon->FirstChildElement ( "mass" );
//                     if ( l_nMass != nullptr ) {
//                         const char* l_mass = l_nMass->GetText();
//                         pPhysc->setMass ( atof ( l_mass ) );
//                     }
// 
//                     tinyxml2::XMLElement* l_nInstaceMaterial = l_nTecnicCommon->FirstChildElement ( "instance_physics_material" );
//                     if ( l_nInstaceMaterial != nullptr ) {
//                         const char* l_url = l_nInstaceMaterial->Attribute ( "url" );
//                         if ( l_url != nullptr ) {
// 
//                             btMaterial *pPMat = m_pPhMaterial[ ( const char* ) &l_url[1]];
//                             if ( pPMat ) {
//                                 pPhysc->setFriction ( pPMat->m_friction );
//                                 pPhysc->setRestitution ( pPMat->m_restitution );
//                             }
//                         }
//                     }
// 
//                     tinyxml2::XMLElement* l_nShape = l_nTecnicCommon->FirstChildElement ( "shape" );
//                     if ( l_nShape != nullptr ) {
// 
//                         l_nShape = l_nShape->FirstChildElement();
//                         const char *l_tipoShape = l_nShape->Value();
// 
//                         if ( strcmp ( l_tipoShape, "sphere" ) == 0 ) {
// 
//                             tinyxml2::XMLElement* l_nEsfera = l_nShape->FirstChildElement();
//                             const char *l_raio = l_nEsfera->GetText();
// 
//                             std::vector<float> l_arrayValores;
// 
//                             loadArrayBtScalar ( l_raio, l_arrayValores );
//                             if ( l_arrayValores.size() == 1 ) {
// 
//                                 //btVector3 raio(l_arrayValores[0], l_arrayValores[0], l_arrayValores[0]);
//                                 pPhysc->setShapeSphere ( l_arrayValores[0] );
// 
//                             } else if ( l_arrayValores.size() == 3 ) {
// 
//                                 //btVector3 raio(l_arrayValores[0], l_arrayValores[1], l_arrayValores[2]);
//                                 pPhysc->setShapeSphere ( l_arrayValores[0] );
// 
//                             } else {
// 
//                             }
// 
//                         } else if ( strcmp ( l_tipoShape, "box" ) == 0 ) {
// 
//                             tinyxml2::XMLElement* l_nBox = l_nShape->FirstChildElement();
//                             const char *l_size = l_nBox->GetText();
// 
//                             std::vector<float> l_arrayValores;
//                             loadArrayBtScalar ( l_size, l_arrayValores );
// 
//                             if ( l_arrayValores.size() == 1 ) {
// 
//                                 btVector3 cubo ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] );
//                                 pPhysc->setShapeBox ( cubo );
// 
//                             } else if ( l_arrayValores.size() == 3 ) {
// 
//                                 btVector3 caixa ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] );
//                                 pPhysc->setShapeBox ( caixa );
// 
//                             } else {
// 
//                             }
// 
//                         } else if ( strcmp ( l_tipoShape, "cylinder" ) == 0 ) {
// 
//                             tinyxml2::XMLElement* l_nCyl = l_nShape->FirstChildElement();
//                             const char *l_size = l_nCyl->GetText();
// 
//                             std::vector<float> l_arrayValores;
//                             loadArrayBtScalar ( l_size, l_arrayValores );
// 
//                             if ( l_arrayValores.size() == 1 ) {
// 
//                                 btVector3 cili ( l_arrayValores[0], l_arrayValores[0], l_arrayValores[0] );
//                                 pPhysc->setShapeCilinder ( cili );
// 
//                             } else if ( l_arrayValores.size() == 3 ) {
// 
//                                 btVector3 cili ( l_arrayValores[0], l_arrayValores[1], l_arrayValores[2] );
//                                 pPhysc->setShapeCilinder ( cili );
// 
//                             } else {
// 
//                             }
//                             //Cylinder
// 
// 
//                             //Cylinder
//                         } else if ( strcmp ( l_tipoShape, "mesh" ) == 0 ) { //FIXME ERRADO!!!!
// 
//                             //instance_geometry
//                             tinyxml2::XMLElement* l_nMesh = l_nShape->FirstChildElement();
//                             if ( l_nMesh != nullptr ) {
//                                 const char *l_mesh = l_nMesh->Attribute ( "url" );
//                                 if ( l_mesh != nullptr ) {
// 
//                                     Graph::DrawTriMesh *pDrawTriMesh = ( Graph::DrawTriMesh* ) Graph::Node::findNodeById ( EntityKind::DRAW, &l_mesh[1] );
// 
//                                     if ( pDrawTriMesh != nullptr ) {
// 
//                                         btTriangleIndexVertexArray *indexVertexArray = new btTriangleIndexVertexArray (
//                                             pDrawTriMesh->vIndex.getSize() / 3,	//num triangles
//                                             pDrawTriMesh->vIndex.ptrVal(),	//lista de indice
//                                             3 * sizeof ( int ),				//tamanho do indice por elemento
//                                             pDrawTriMesh->vList.getSize(),	//num Vertices
//                                             pDrawTriMesh->vList.ptrVal(),	//lista de vertice
//                                             3 * sizeof ( float )				//tamanho do vertice por elemento
//                                         );
// 
//                                         pPhysc->setIndexVertexArray ( indexVertexArray );
// 
//                                     }
//                                 }
//                             }
//                         }
//                     }
// 
//                     //TODO carga do shape de colisao
// 
//                 }
// 
//                 l_nNode = l_nNode->NextSiblingElement ( "rigid_body" );
// 
//             }
//         }
//     }
// }
// 
// void Loader::libPhysicsScenes ( void ) {
// 
//     tinyxml2::XMLElement* l_nNode = root->FirstChildElement ( "library_physics_scenes" );
//     if ( l_nNode != nullptr ) {
//         l_nNode = l_nNode->FirstChildElement ( "physics_scene" );
//         if ( l_nNode != nullptr ) {
//             l_nNode = l_nNode->FirstChildElement ( "instance_physics_model" );
// 
//             if ( l_nNode != nullptr ) {
//                 l_nNode = l_nNode->FirstChildElement ( "instance_rigid_body" );
// 
//                 while ( l_nNode != nullptr ) {
// 
//                     const char *l_body = l_nNode->Attribute ( "body" );
//                     const char *l_target = l_nNode->Attribute ( "target" );
// 
//                     Graph::Physics *pPhysic = ( Graph::Physics* ) Graph::Node::findNodeById ( EntityKind::PHYSICS, l_body ); //m_mPhysics[ ( const char* ) l_pBody];
//                     Graph::Object *pObj = ( Graph::Object* ) Graph::Node::findNodeById ( EntityKind::OBJECT, &l_target[1] );
// 
//                     if ( ( pPhysic ) && ( pObj ) ) {
//                         //FIXME ERRADO, Refazer tudo, usar o XML como um DB partudo de cima para baixo e nao o contrario
//                         //Physics *novoPhy = nullptr;
//                         //pPhysic->clone((Node**)&novoPhy);
//                         pObj->addChild ( pPhysic );
// 
//                     }
// 
//                     l_nNode = l_nNode->NextSiblingElement ( "instance_rigid_body" );
//                 }
//             }
//         }
//     }
// 
// }
// 
// }
// // kate: indent-mode cstyle; indent-width 4; replace-tabs on;
