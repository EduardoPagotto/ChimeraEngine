#include "Loader.h"

namespace Chimera {

Loader::Loader () {
    m_numNodes = 0;
    //m_physicWorld = Singleton<PhysicsControl>::getRefSingleton();
    logger = log4cxx::Logger::getLogger ( "Loader" );
}

Loader::~Loader() {
    //Singleton<PhysicsControl>::releaseRefSingleton();
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
    m_doc = xmlParseFile ( l_nomeArquivo.c_str() );
    if ( m_doc == nullptr )
        throw ExceptionChimera ( ExceptionCode::READ, "Falha Doc XML:" + _file );

    m_root = xmlDocGetRootElement ( m_doc );
    if ( m_root == nullptr ) {
        xmlFreeDoc ( m_doc );
        throw ExceptionChimera ( ExceptionCode::READ, "Falha no root XML:" + _file );
    }

    if ( xmlStrcmp ( m_root->name, ( const xmlChar * ) "COLLADA" ) ) {
        xmlFreeDoc ( m_doc );
        throw ExceptionChimera ( ExceptionCode::READ, "Falha no root COLLADA:" + _file );
    }

    //Carrega Cameras
    libCam();

    //Carrega Iluminacao
    libLight();

    //Carrega Efeitos
    libEffect();

    //Carrega Texturas
    libTexture();

    //Carrega Material
    libMaterial();

    //Carrega DrawTriMesh
    libGeometry();

    //Carrega componentes da cena
    pRootScene = libScene();

    libPhysicsMaterial();
    
    libPhysicsModels();
    
    libPhysicsScenes();
    
    return pRootScene;
}

Node *Loader::cloneDraw ( Draw *_srcDraw ) {
    
    Node *novo = nullptr;
    
    switch ( _srcDraw->getType() ) {
        
        case DrawType::MESH:
            novo = new DrawTriMesh ( * ( ( DrawTriMesh* ) _srcDraw ) );
            break;
        case DrawType::BOX:
            novo = new DrawBox ( * ( ( DrawBox* ) _srcDraw ) );
            break;
        case DrawType::GRID:
            novo = new DrawGrid ( * ( ( DrawGrid* ) _srcDraw ) );
            break;
        case DrawType::SPHERE:
            //novo = new Draw(*((DrawBox*)_srcDraw));
            break;
            
    }
    
    return novo;
}

Node *Loader::clone ( Node *_src ) {
    
    Node *novo = nullptr;
    
    switch ( _src->getKind() ) {
        case EntityKind::NODE:
            novo = new Node( * (( Node*) _src));
            break;       
        case EntityKind::CAMERA:
            novo = new Camera ( * ( ( Camera* ) _src ) );
            break;
        case EntityKind::LIGHT:
            novo = new Light ( * ( ( Light* ) _src ) );
            break;
        case EntityKind::OBJECT:
            novo = new Object ( * ( ( Object* ) _src ) );
            break;
        case EntityKind::MATERIAL:
            novo = new Material ( * ( ( Material* ) _src ) );
            break;
        case EntityKind::EFFECT:
            novo = new Effect ( * ( ( Effect* ) _src ) );
            break;
        case EntityKind::TEXTURE:
            novo = new Texture ( * ( ( Texture* ) _src ) );
            break;
        case EntityKind::DRAW:
            novo = cloneDraw ( ( Draw* ) _src );
            break;
        case EntityKind::PHYSICS:
            novo = new Physics(*((Physics*) _src));
            break;
        default:
            throw ExceptionChimera ( ExceptionCode::READ, "Tipo de clonagem nao implementada para:" + _src->getId() );
            break;
    }
    
    for ( Node *pNode : _src->listChild ) {
        novo->addChild ( clone ( pNode ) );
    }
    
    return novo;
}

std::string Loader::getAtributoXML ( const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode ) {
    xmlChar *pStr = xmlGetProp ( _xmlNode, ( const xmlChar* ) chave.c_str() );
    if ( pStr != nullptr ) {
        return std::string ( ( const char* ) pStr );
    } else {
        std::string l_erro = "Entidade '" + tipoNomeNode +"' com Propriedade '" + chave + "' nao localizado";
        LOG4CXX_WARN ( logger , l_erro );
        //throw ExceptionChimera ( ExceptionCode::READ, l_erro );
    }
    
    return std::string("");
}

std::string Loader::getValueFromProp ( const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode ) {
    std::string retorno;
    xmlNodePtr l_nElem = findNode ( chave.c_str(), _xmlNode );
    if ( l_nElem != nullptr ) {
        xmlChar* l_pVal = xmlNodeListGetString ( m_doc, l_nElem->children, 1 );

        if ( l_pVal != nullptr ) {
            retorno = ( const char* ) l_pVal;
            xmlFree ( l_pVal );
            return retorno;
        } else {
            std::string l_erro = "Propriedade:" + tipoNomeNode + " Valor Procurado:" + chave +"nao encontrado";
            LOG4CXX_WARN ( logger , l_erro );
        }
    }

    std::string l_erro = "Entidade '" + tipoNomeNode +"' com Atributo '" + chave + "' nao localizado";
    LOG4CXX_WARN ( logger ,l_erro );

    return std::string ( "" );
}

void Loader::loadArrayI ( const char *_val, std::vector<int> &_arrayI ) {
    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit,' ' );
        if ( pchFim==nullptr )
            pchFim = deadEnd;

        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp,pchInit,l_tam );
        l_numTemp[l_tam ] = 0;
        _arrayI.push_back ( atoi ( l_numTemp ) );

        pchInit = pchFim+1;
    } while ( pchInit < deadEnd );
}

void Loader::loadArrayF ( const char *_val, std::vector<float> &_arrayF ) {
    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit,' ' );
        if ( pchFim==nullptr )
            pchFim = deadEnd;

        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp,pchInit,l_tam );
        l_numTemp[l_tam ] = 0;
        _arrayF.push_back ( ( float ) atof ( l_numTemp ) );

        pchInit = pchFim+1;
    } while ( pchInit < deadEnd );
}

xmlNodePtr Loader::findNode ( const char* _nomeNode, xmlNodePtr _nodePos ) {
    xmlNodePtr retorno = nullptr;
    if ( _nodePos != nullptr ) {
        if ( _nodePos->type == XML_ELEMENT_NODE ) {
            if ( strcmp ( ( const char* ) _nodePos->name,_nomeNode ) ==0 )
                return _nodePos;
        }

        if ( _nodePos->children != nullptr )
            retorno = findNode ( _nomeNode, _nodePos->children );

        if ( ( retorno == nullptr ) && ( _nodePos->next != nullptr ) )
            retorno = findNode ( _nomeNode, _nodePos->next );
    }

    return retorno;
}

Color Loader::getPhong ( xmlNodePtr _nPos,const char* _name ) {
    Color retorno;

    xmlChar *l_pVal = nullptr;
    std::vector<float> l_arrayValores;
    xmlNodePtr l_nPos = _nPos;

    l_nPos = findNode ( _name,_nPos->children );
    if ( l_nPos ) {
        l_nPos = findNode ( ( char* ) "color",l_nPos->children );
        if ( l_nPos ) {
            l_pVal = xmlNodeListGetString ( m_doc, l_nPos->children, 1 );
            if ( l_pVal ) {
                loadArrayF ( ( char* ) l_pVal ,l_arrayValores );
                retorno.set ( l_arrayValores[0],l_arrayValores[1],l_arrayValores[2],l_arrayValores[3] );

                l_arrayValores.clear();
                xmlFree ( l_pVal );
            }
        }
    }
    return retorno;
}

int Loader::getSource ( xmlNodePtr _nSource, ListPtr<float> &_arrayValores ) {
    int l_numSource = 0;
    xmlNodePtr l_nSource = findNode ( ( char* ) "float_array", _nSource );
    if ( l_nSource ) {
        xmlChar* l_pVal = xmlGetProp ( l_nSource, ( const xmlChar* ) "count" );
        if ( l_pVal ) {
            l_numSource = atoi ( ( char* ) l_pVal );
            l_pVal = xmlNodeListGetString ( m_doc, l_nSource->children, 1 );
            if ( l_pVal ) {
                std::vector<float> l_array;
                loadArrayF ( ( char* ) l_pVal ,l_array );
                if ( l_numSource == l_array.size() ) {
                    _arrayValores.create ( l_numSource );
                    for ( unsigned l_contador = 0 ; l_contador < l_array.size() ; l_contador++ )
                        _arrayValores[l_contador] = l_array[l_contador];
                } else
                    l_numSource = -1; //ERRO

                xmlFree ( l_pVal );
                l_array.clear();
            }
        }
    }
    return l_numSource;
}

void Loader::libCam ( void ) {
    int l_count = 0;

    //Pega Raiz de Elemento conjunto Cameras
    xmlNodePtr l_nCam = findNode ( ( char* ) "library_cameras", m_root );

    //Encontra o primeiro elemento
    l_nCam = findNode ( ( char* ) "camera", l_nCam->children );
    while ( l_nCam != nullptr ) {

        try {

            Camera *pCamera = new Camera(CameraType::Base, getAtributoXML ( "Camera","id",l_nCam ),getAtributoXML ( "Camera","name",l_nCam ) );
            listaNodeRemover.push ( pCamera ); //Salva na lista de remocao

            //Verifica se exite Elemento Perspectiva
            xmlNodePtr l_nTech = findNode ( ( char* ) "perspective",l_nCam );
            if ( l_nTech != nullptr ) {

                pCamera->setPerspective ( true );

                std::string val = getValueFromProp ( "Camera","xfov",l_nTech );
                if ( val.size() >0 )
                    pCamera->setFov ( ( float ) atof ( val.c_str() ) );

                val = getValueFromProp ( "Camera","znear",l_nTech );
                if ( val.size() >0 )
                    pCamera->setNear ( ( float ) atof ( val.c_str() ) );

                val = getValueFromProp ( "Camera","zfar",l_nTech );
                if ( val.size() >0 )
                    pCamera->setFar ( ( float ) atof ( val.c_str() ) );

            } else {

                l_nTech = findNode ( ( char* ) "orthogonal",l_nCam );
                if ( l_nTech!=nullptr ) {
                    //TODO: implementar camera ortogonal
                    pCamera->setPerspective ( false );
                    throw ExceptionChimera ( ExceptionCode::READ, "Ortogonal nao implementada" );
                } else {
                    throw ExceptionChimera ( ExceptionCode::READ, "Nem Perspectiva ne Ortogonal encontrada" );
                }

            }

        } catch ( const ExceptionChimera& ec ) {
            LOG4CXX_WARN ( logger , ec.getMessage() );
        }

        l_nCam = findNode ( ( char* ) "Camera", l_nCam->next );
        l_count++;
    }

    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Camera registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Cameras Registradas:" ) + std::to_string ( l_count ) ) );
    }

}

void Loader::libLight () {
    int l_count = 0;

    //Pega Raiz de Elemento conjunto Luzes
    xmlNodePtr l_nLight = findNode ( ( char* ) "library_lights", m_root );
    if ( l_nLight != nullptr ) {
        xmlChar* l_pVal = nullptr;
        std::vector<float> l_arrayValores;
        l_nLight = findNode ( ( char* ) "light", l_nLight->children );
        while ( l_nLight!=nullptr ) {
            //Carrega atributos

            try {

                Light *pLight = new Light ( LightType::POINT, getAtributoXML ( "Light","id",l_nLight ), getAtributoXML ( "Light","name",l_nLight ));
                listaNodeRemover.push ( pLight ); //Salva na lista de remocao

                xmlNodePtr l_nTipo = findNode ( ( char* ) "point", l_nLight->children ); //TODO implementar depois tipos direcional e spot
                if ( l_nTipo != nullptr ) {
                    std::string cores = getValueFromProp ( "Light","color",l_nTipo->children );
                    if ( cores.size() >0 ) {
                        loadArrayF ( cores.c_str() ,l_arrayValores );
                        //pLight->setAmbient ( Color ( l_arrayValores[0] , l_arrayValores[1] , l_arrayValores[2], 1.0f ) );
                        pLight->setDiffuse( Color ( l_arrayValores[0] , l_arrayValores[1] , l_arrayValores[2], 1.0f ) );
                        l_arrayValores.clear();
                    }
                } else {
                    throw ExceptionChimera ( ExceptionCode::READ, "Luz tipo ponto nao encontrata" );
                }
            } catch ( const ExceptionChimera& ec ) {
                LOG4CXX_WARN ( logger , ec.getMessage() );
            }

            l_nLight = findNode ( ( char* ) "light", l_nLight->next );
            l_count++;
        }
    }

    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha luzes registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Luzes Registradas:" ) + std::to_string ( l_count ) ) );
    }

}

void Loader::libEffect ( void ) {
    int l_count = 0;
    xmlNodePtr l_nMat = findNode ( ( char* ) "library_effects", m_root );
    if ( l_nMat ) {
        l_nMat = findNode ( ( char* ) "effect",l_nMat->children );
        while ( l_nMat != nullptr ) {
            l_count++;
            try {
               
                Effect *pEffect = new Effect(getAtributoXML ( "Effects","id",l_nMat ), "Effects");
                listaNodeRemover.push(pEffect);
                
                //pega referencia textura
                std::string idTextura = getValueFromProp ( "Effects","init_from",l_nMat->children );
                if ( idTextura.size() >0 ) {
                    pEffect->setNameTextureId ( idTextura );
                }

                //pega cores
                xmlNodePtr l_nPos = findNode ( ( char* ) "phong",l_nMat->children );
                if ( l_nPos ) {
                    xmlNodePtr l_nRootPhong = l_nPos;
                    pEffect->setEmissive ( getPhong ( l_nPos, ( char* ) "emission" ) );
                    pEffect->setAmbient ( getPhong ( l_nPos, ( char* ) "ambient" ) );
                    pEffect->setDiffuse ( getPhong ( l_nPos, ( char* ) "diffuse" ) );
                    pEffect->setSpecular ( getPhong ( l_nPos, ( char* ) "specular" ) );

                    l_nPos = findNode ( ( char* ) "shininess",l_nPos->children );
                    if ( l_nPos ) {

                        std::string l_shininess = getValueFromProp ( "Effect","float",l_nPos->children );
                        if ( l_shininess.size() >0 ) {
                            pEffect->setShininess ( ( float ) atof ( ( char* ) l_shininess.c_str() ) );
                        }
                    }
                }
                

            } catch ( const ExceptionChimera& ec ) {
                LOG4CXX_WARN ( logger , ec.getMessage() );
            }

            l_nMat = findNode ( ( char* ) "effect", l_nMat->next );
        }
    }

    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Efeitos registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Efeitos Registradas:" ) + std::to_string ( l_count ) ) );
    }
}

void Loader::libTexture ( void ) {
    int l_count = 0;
    xmlNodePtr l_nTexture = findNode ( ( char* ) "library_images", m_root );
    if ( l_nTexture ) {
        l_nTexture = findNode ( ( char* ) "image", l_nTexture->children );
        while ( l_nTexture ) {
            try {

                Texture *pTexture = new Texture(getAtributoXML ( "Texture","id",l_nTexture ),getAtributoXML ( "Texture","name",l_nTexture ));
                listaNodeRemover.push( pTexture );

                std::string l_pathFile = getValueFromProp ( "Texture","init_from",l_nTexture->children );
                if ( l_pathFile.size() >0 ) {
                    std::string l_arquivo = m_imageDir + l_pathFile;
                    pTexture->setPathFile ( l_arquivo.c_str() );
                    pTexture->init();
                    //pTexture->loadImage();
                    l_count++;
                }

            } catch ( const ExceptionChimera& ec ) {
                LOG4CXX_WARN ( logger , ec.getMessage() );
            }

            l_nTexture = findNode ( ( char* ) "image", l_nTexture->next );
        }
    }

    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Texturens registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Texturens Registradas:" ) + std::to_string ( l_count ) ) );
    }
}

void Loader::libMaterial ( void ) {
    int l_count = 0;
    xmlNodePtr l_nMat = findNode ( ( char* ) "library_materials", m_root );
    if ( l_nMat ) {
        l_nMat = findNode ( ( char* ) "material", m_root );
        while ( l_nMat ) {
            l_count++;
            try {
                std::string l_idDesenhoBase = getAtributoXML ( "Material","id",l_nMat );
                if ( l_idDesenhoBase.size() > 0 ) {

                    Material *pMaterial = new Material(getAtributoXML ( "Material","id",l_nMat ),getAtributoXML ( "Material","name",l_nMat ));
                    listaNodeRemover.push(pMaterial);
                    
                    xmlNodePtr l_nEffect = findNode ( ( char* ) "instance_effect", l_nMat->children ); //TODO:Corrigir ha um bug aqui ele passa 2 vezes
                    if ( l_nEffect ) {

                        xmlChar *l_val = xmlGetProp ( l_nEffect, ( const xmlChar* ) "url" );
                        if ( l_val ) {
                            std::string l_nomeEffect = ( char* ) &l_val[1];
                            if (l_nomeEffect.size() >0 ) {
                                Effect *pEffe =(Effect*) Node::findObjById(EntityKind::EFFECT, l_nomeEffect); //m_mEffect[ l_nomeEffect ];
                                if ( pEffe ) {
                                    pMaterial->pEffect = pEffe;//new Effect( *pEffe );
                                    if (pEffe->getNameTextureId().size() > 0) {
                                        Texture *pTexture = (Texture*) Node::findObjById(EntityKind::TEXTURE,pEffe->getNameTextureId()); //m_mTextura[pEffe->getNameTextureId()];//( Texture* ) Node::findObjById ( EntityKind::IMAGE, pEffe->getNameTextureId() );
                                        if ( pTexture!=nullptr ) {
                                            pMaterial->pTextura = pTexture;//new Texture( *pTexture );
                                        }
                                    }
                                } 
                            }
                        }
                    }
                }

            } catch ( const ExceptionChimera& ec ) {
                LOG4CXX_WARN ( logger , ec.getMessage() );
            }

            l_nMat = findNode ( ( char* ) "material", l_nMat->next );
        }
    }

    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Material registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Material Registradas:" ) + std::to_string ( l_count ) ) );
    }
}

void Loader::libGeometry ( void ) {
    int l_count = 0;

    xmlNodePtr l_nGeom = findNode ( ( char* ) "library_geometries", m_root );
    if ( l_nGeom!=nullptr ) {
        char *l_pVal = nullptr;
        l_nGeom = findNode ( ( char* ) "geometry", l_nGeom->children );
        while ( l_nGeom!=nullptr ) {

                
            DrawTriMesh *pDrawTriMesh = new DrawTriMesh(getAtributoXML ( "DrawTriMesh","id",l_nGeom ),getAtributoXML ( "DrawTriMesh","name",l_nGeom ));                
            listaNodeRemover.push(pDrawTriMesh);
            
            xmlNodePtr l_nDrawTriMesh = findNode ( ( char* ) "mesh", l_nGeom->children );
            if ( l_nDrawTriMesh ) {
                l_nDrawTriMesh = findNode ( ( char* ) "source",l_nDrawTriMesh->children );
                while ( l_nDrawTriMesh ) {
                    l_pVal = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "id" );
                    std::string l_sVal = l_pVal;

                    size_t posicaoEncontrada = l_sVal.find ( "-positions" );
                    if ( posicaoEncontrada != std::string::npos ) {
                        getSource ( l_nDrawTriMesh,pDrawTriMesh->vList );
                    } else {
                        posicaoEncontrada = l_sVal.find ( "-normals" );
                        if ( posicaoEncontrada != std::string::npos )
                            getSource ( l_nDrawTriMesh,pDrawTriMesh->nList );
                        else {
                            posicaoEncontrada = l_sVal.find ( "-map-0" );
                            if ( posicaoEncontrada != std::string::npos )
                                getSource ( l_nDrawTriMesh,pDrawTriMesh->uvList );
                        }
                    }

                    l_nDrawTriMesh = findNode ( ( char* ) "source",l_nDrawTriMesh->next );
                }

                xmlNodePtr l_nDrawTriMesh = findNode ( ( char* ) "polylist", l_nGeom->children );
                if ( l_nDrawTriMesh ) {
                    l_pVal = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "count" );
                    char *l_pMaterialFile = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "material" );
                    if ( l_pVal ) {
                        xmlNodePtr l_nAnterior;
                        unsigned l_numTriangles = atoi ( l_pVal );
                        l_nDrawTriMesh = findNode ( ( char* ) "input", l_nDrawTriMesh->children );

                        std::vector<char*> l_vOffset;
                        std::vector<char*> l_vSemantic;
                        std::vector<char*> l_vSource;

                        while ( l_nDrawTriMesh ) {
                            char *l_offSet = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "offset" );
                            char *l_semantic = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "semantic" );
                            char *l_source = ( char* ) xmlGetProp ( l_nDrawTriMesh, ( const xmlChar* ) "source" );

                            l_vOffset.push_back ( l_offSet );
                            l_vSemantic.push_back ( l_semantic );
                            l_vSource.push_back ( l_source );

                            l_nAnterior = l_nDrawTriMesh;

                            l_nDrawTriMesh = findNode ( ( char* ) "input",l_nDrawTriMesh->next );
                        }

                        l_nDrawTriMesh = findNode ( ( char* ) "p", l_nAnterior );
                        if ( l_nDrawTriMesh ) {
                            l_pVal = ( char* ) xmlNodeListGetString ( m_doc, l_nDrawTriMesh->children, 1 );
                            if ( l_pVal ) {
                                std::vector<int> l_arrayIndex;
                                loadArrayI ( l_pVal,l_arrayIndex );

                                unsigned l_veCount = 0;
                                unsigned l_noCount = 0;
                                unsigned l_uvCount = 0;

                                for ( unsigned l_contador = 0 ; l_contador < l_arrayIndex.size() ; l_contador++ ) {
                                    int index = l_contador % l_vOffset.size();

                                    char *l_offSet = l_vOffset[index];
                                    char *l_semantic = l_vSemantic[index];
                                    char *l_source = l_vSource[index];
                                    if ( strstr ( l_source, ( char* ) "-vertices" ) != nullptr ) {
                                        if ( pDrawTriMesh->vIndex.getSize() == 0 )
                                            pDrawTriMesh->vIndex.create ( l_numTriangles * 3 );

                                        pDrawTriMesh->vIndex[l_veCount] = l_arrayIndex[l_contador];
                                        l_veCount++;
                                    } else if ( strstr ( l_source, ( char* ) "-normals" ) != nullptr ) {
                                        if ( pDrawTriMesh->nIndex.getSize() ==0 )
                                            pDrawTriMesh->nIndex.create ( l_numTriangles * 3 );

                                        pDrawTriMesh->nIndex[l_noCount] = l_arrayIndex[l_contador];
                                        l_noCount++;
                                    } else if ( strstr ( l_source, ( char* ) "-map-0" ) != nullptr ) {
                                        if ( pDrawTriMesh->tIndex.getSize() ==0 )
                                            pDrawTriMesh->tIndex.create ( l_numTriangles* 3 );

                                        pDrawTriMesh->tIndex[l_uvCount] = l_arrayIndex[l_contador];
                                        l_uvCount++;
                                    }
                                }
                                l_arrayIndex.clear();
                            }
                            xmlFree ( ( xmlChar* ) l_pVal );
                        }
                        l_vOffset.clear();
                        l_vSemantic.clear();
                        l_vSource.clear();
                    }
                }
                //pObj->setDrawTriMesh((*pDrawTriMesh));
            }
            l_nGeom = findNode ( ( char* ) "geometry", l_nGeom->next );
            l_count++;

            
        }
    }
    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Geometria registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Geometria Registradas:" ) + std::to_string ( l_count ) ) );
    }
}

Node* Loader::libScene ( void ) {

    Node *pScene = nullptr;

    xmlNodePtr l_nScene = findNode ( ( char* ) "library_visual_scenes", m_root );
    if ( l_nScene!=nullptr ) {
        l_nScene = findNode ( ( char* ) "visual_scene", l_nScene->children );
        if ( l_nScene!=nullptr ) {
            
            pScene = new Node(EntityKind::NODE, getAtributoXML ( "Scene","id",l_nScene ),getAtributoXML ( "Scene","name",l_nScene ) );
            xmlNodePtr l_nNode = findNode ( ( char* ) "node",l_nScene->children );
            createNode ( l_nNode, pScene );
        }
    }

    if ( m_numNodes == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Nodes de cena registrados!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Nodes de cena Registrados:" ) + std::to_string ( m_numNodes ) ) );
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

void Loader::createNode ( xmlNodePtr _nodeXML, Node *_pNode ) {
    xmlNodePtr l_nNode = _nodeXML;//findNode((char*)"node", _nodeXML->children);

    while ( l_nNode ) {
        if ( l_nNode->type==XML_ELEMENT_NODE ) {
            xmlChar* l_pVal = nullptr;
            xmlChar *pSID = nullptr;
            Node *pFilho = nullptr;
            m_numNodes++;
            std::vector<float> l_arrayValores;
            const xmlChar *l_pNomeNode = l_nNode->name;
                        
            btTransform l_transform;

            xmlNodePtr l_nTransforma = findNode ( ( char* ) "matrix", l_nNode->children );

            if ( l_nTransforma ) {
                pSID = xmlGetProp ( l_nTransforma, ( const xmlChar* ) "sid" );
                if ( xmlStrcmp ( pSID, ( xmlChar* ) "transform" ) == 0 ) {
                    l_pVal = xmlNodeListGetString ( m_doc, l_nTransforma->children, 1 );
                    loadArrayF ( ( char* ) l_pVal ,l_arrayValores );
                    carregaMatrix ( &l_transform, l_arrayValores );
                    l_arrayValores.clear();
                    xmlFree ( l_pVal );
                }
            }

            xmlNodePtr l_nInstance;;
            if ( l_nInstance = findNode ( ( char* ) "instance_geometry", l_nNode->children ) ) {
                               
                Object *pObj = new Object(getAtributoXML ( "Node","id",l_nNode ),getAtributoXML ( "Node","name",l_nNode ) );
                pObj->transform = l_transform;
                
                _pNode->addChild ( pObj );

                pFilho = pObj;

                xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
                if ( pURL != nullptr ) {

                    
                    std::string l_nomeDrawTriMesh = ( char* ) &pURL[1];
                    DrawTriMesh *pDrawTriMesh = (DrawTriMesh*)Node::findObjById(EntityKind::DRAW,l_nomeDrawTriMesh);//m_mDrawTriMesh[ l_nomeDrawTriMesh];

                    if ( pDrawTriMesh ) {

                        pObj->pDraw = (DrawTriMesh*)clone(pDrawTriMesh);//new DrawTriMesh ( *pDrawTriMesh );
                        
                        //Carrega Material se existir
                        if ( l_nInstance->children ) {
                            xmlNodePtr l_nBindMat = findNode ( ( char* ) "bind_material", l_nInstance->children );
                            if ( l_nBindMat ) {
                                l_nBindMat = findNode ( ( char* ) "technique_common", l_nBindMat->children );
                                if ( l_nBindMat ) {
                                    l_nBindMat = findNode ( ( char* ) "instance_material", l_nBindMat->children );
                                    if ( l_nBindMat ) {
                                        pURL = xmlGetProp ( l_nBindMat, ( const xmlChar* ) "target" );
                                        if ( pURL ) {

                                            std::string l_nomeMaterial = ( char* ) &pURL[1];

                                            Material *pMaterial = (Material*)Node::findObjById(EntityKind::MATERIAL,l_nomeMaterial );
                                            //DataDraw *pDataDraw = m_mDesenhoBase[  l_nomeMaterial ];
                                            if ( pMaterial != nullptr ) {
                                                
                                                if (pMaterial->pTextura != nullptr)
                                                    pObj->pTexture = (Texture*)clone(pMaterial->pTextura);
                                                
                                                if (pMaterial->pEffect != nullptr)
                                                    pObj->pEffect = (Effect*)clone( pMaterial->pEffect );

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if ( l_nInstance = findNode ( ( char* ) "instance_light", l_nNode->children ) ) {
                xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
                if ( pURL != nullptr ) {

                    Light *pLight = ( Light* ) Node::findObjById ( EntityKind::LIGHT, ( char* ) &pURL[1] );
                    if ( pLight!=nullptr ) {

                        Light *pLightScene = ( Light* ) clone ( pLight ); //Light *pLightScene = new Light ( *pLight );
                        pLightScene->transform = l_transform;

                        _pNode->addChild ( pLightScene );
                        pFilho = pLightScene;

                    }
                }
            } else if ( l_nInstance = findNode ( ( char* ) "instance_camera", l_nNode->children ) ) {
                xmlChar *pURL = xmlGetProp ( l_nInstance, ( const xmlChar* ) "url" );
                if ( pURL != nullptr ) {

                    Camera *pCam = ( Camera* ) Node::findObjById ( EntityKind::CAMERA, ( char* ) &pURL[1] );

                    if ( pCam!=nullptr ) {

                        Camera *pCamScena = ( Camera * ) clone ( pCam ); //Camera *pCamScena = new Camera ( *pCam );
                        pCamScena->transform = l_transform;

                        _pNode->addChild ( pCamScena );
                        pFilho = pCamScena;

                    }
                }
            }

            Node* l_buffer = _pNode;
            createNode ( l_nInstance->next, pFilho );
            _pNode = l_buffer;
        }
        l_nNode = l_nNode->next;
    }
}

void Loader::libPhysicsMaterial ( void ) {
    unsigned l_num = 0;
    xmlNodePtr l_nPhMat = findNode ( ( char* ) "library_physics_materials", m_root );
    if ( l_nPhMat ) {
        l_nPhMat =  findNode ( ( char* ) "physics_material",l_nPhMat->children );
        while ( l_nPhMat ) {
            l_num++;
            xmlChar *l_pName = xmlGetProp ( l_nPhMat, ( const xmlChar* ) "name" );

            btMaterial *pPMat = new btMaterial;
            m_pPhMaterial[ ( const char* ) l_pName] = pPMat;

            xmlNodePtr l_nTec =  findNode ( ( char* ) "technique_common",l_nPhMat->children );
            if ( l_nTec ) {
                xmlNodePtr l_nFric =  findNode ( ( char* ) "dynamic_friction",l_nTec->children );
                if ( l_nFric ) {
                    xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nFric->children, 1 );
                    if ( l_pVal ) {
                        pPMat->m_friction = atof ( ( const char* ) l_pVal );
                        xmlFree ( l_pVal );
                    }
                }

                l_nFric =  findNode ( ( char* ) "restitution",l_nTec->children );
                if ( l_nFric ) {
                    xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nFric->children, 1 );
                    if ( l_pVal ) {
                        pPMat->m_restitution = atof ( ( const char* ) l_pVal );
                        xmlFree ( l_pVal );
                    }
                }
            }

            l_nPhMat =  findNode ( ( char* ) "physics_material",l_nPhMat->next );
        }
    }
    
    if ( l_num == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Material Fisico registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Materia Fisico Registradas:" ) + std::to_string ( l_num ) ) );
    }
}

void Loader::libPhysicsModels () {
    
    int l_count = 0;
    xmlNodePtr l_nPhysics = findNode ( ( char* ) "library_physics_models", m_root );
    if ( l_nPhysics ) {
        l_nPhysics = findNode ( ( char* ) "physics_model", l_nPhysics->children );
        if ( l_nPhysics ) {
            l_nPhysics = findNode ( ( char* ) "rigid_body", l_nPhysics );
            while ( l_nPhysics ) {
                l_count++;
                xmlChar *l_pName = xmlGetProp ( l_nPhysics, ( const xmlChar* ) "name" );
                xmlChar *l_pSid = xmlGetProp ( l_nPhysics, ( const xmlChar* ) "sid" );
                if ( ( l_pName ) && ( l_pSid ) ) {
                    
                    Physics *pPhysc = new Physics(std::string((char*)l_pName),std::string((char*)l_pSid));
                    listaNodeRemover.push(pPhysc);
                    
                    xmlNodePtr l_nTec = findNode ( ( char* ) "technique_common", l_nPhysics->children );
                    if ( l_nTec ) {
                        xmlNodePtr l_nProp = findNode ( ( char* ) "dynamic", l_nTec->children );
                        if ( l_nProp ) {
                            xmlChar *l_pVal = xmlNodeListGetString ( m_doc, l_nProp->children, 1 );
                            if ( l_pVal ) {
                                if ( strcmp ( ( const char* ) l_pVal, ( const char* ) "true" ) ==0 ) {
                                    l_nProp = findNode ( ( char* ) "mass", l_nProp );
                                    if ( l_nProp ) {
                                        xmlFree ( l_pVal );
                                        l_pVal = xmlNodeListGetString ( m_doc, l_nProp->children, 1 );
                                        if ( l_pVal )
                                            pPhysc->setMass ( ( float ) atof ( ( char* ) l_pVal ) );
                                        
                                        //TODO carga do shape de colisao
                                        
                                    }
                                }

                                xmlFree ( l_pVal );
                            }
                        }

                        l_nProp = findNode ( ( char* ) "instance_physics_material", l_nTec->children );
                        if ( l_nProp ) {
                            xmlChar *l_pName = xmlGetProp ( l_nProp, ( const xmlChar* ) "url" );
                            if ( l_pName ) {
                                btMaterial *pPMat =m_pPhMaterial[ ( const char* ) &l_pName[1]];
                                if ( pPMat )
                                    pPhysc->setFriction( pPMat->m_friction );
                                    pPhysc->setRestitution( pPMat->m_restitution );
                            }
                        }
                    }
                }
                l_nPhysics = findNode ( ( char* ) "rigid_body", l_nPhysics->next );
            }
        }
    }
    
    if ( l_count == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Modelo Fisico registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Modelo Fisico Registradas:" ) + std::to_string ( l_count ) ) );
    }
 
}

void Loader::libPhysicsScenes ( void ) {
    unsigned l_num=0;
    xmlNodePtr l_nPhyScene = findNode ( ( char* ) "library_physics_scenes", m_root );
    if ( l_nPhyScene!=nullptr ) {
        l_nPhyScene = findNode ( ( char* ) "instance_physics_model", l_nPhyScene->children );
        if ( l_nPhyScene ) {
            xmlNodePtr l_nRigid = findNode ( ( char* ) "instance_rigid_body", l_nPhyScene->children );
            while ( l_nRigid ) {
                xmlChar *l_pBody = xmlGetProp ( l_nRigid, ( const xmlChar* ) "body" );
                xmlChar *l_pTarget = xmlGetProp ( l_nRigid, ( const xmlChar* ) "target" );
                if ( ( l_pBody ) && ( l_pTarget ) ) {
                    
                    Physics *pPhysic = (Physics*)Node::findObjById(EntityKind::PHYSICS, std::string((char*)l_pBody));//m_mPhysics[ ( const char* ) l_pBody];
                   
                    Object *pObj = ( Object* ) Node::findObjById(EntityKind::OBJECT, ( const char* ) &l_pTarget[1]);

                    if ( ( pPhysic ) && ( pObj ) ) {
                        //DrawTriMesh *pDrawTriMesh = ( DrawTriMesh* ) pObj->pDraw; //todo pode ser o draw
                        //if ( pDrawTriMesh ) 
                        //    pPhysic->setShapeBox ( pDrawTriMesh->getSizeBox() );

                        pObj->pPhysic = (Physics*) clone( pPhysic );
                        l_num++;
                    }
                }
                l_nRigid = findNode ( ( char* ) "instance_rigid_body",l_nRigid->next );
            }
        }
    }

    if ( l_num == 0 ) {
        LOG4CXX_WARN ( logger , "Nao ha Modelo Fisico registrada!!!" );
    } else {
        LOG4CXX_INFO ( logger , std::string ( std::string ( "Modelo Fisico Registradas:" ) + std::to_string ( l_num ) ) );
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
