#include <iostream>
#include "ChimeraUtils.h"
#include "ExceptionChimera.h"

#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

void loadArrayBtScalar ( const char *_val, std::vector<float> &_arrayF ) {

    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit, ' ' );
        if ( pchFim == nullptr ) {
            pchFim = deadEnd;
        }

        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp, pchInit, l_tam );
        l_numTemp[l_tam] = 0;
        _arrayF.push_back ( ( float ) atof ( l_numTemp ) );

        pchInit = pchFim + 1;
    } while ( pchInit < deadEnd );
}


void loadArrayI ( const char *_val, std::vector<int> &_arrayI ) {
    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit, ' ' );
        if ( pchFim == nullptr ) {
            pchFim = deadEnd;
        }

        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp, pchInit, l_tam );
        l_numTemp[l_tam] = 0;
        _arrayI.push_back ( atoi ( l_numTemp ) );

        pchInit = pchFim + 1;
    } while ( pchInit < deadEnd );
}

glm::mat4 carregaMatrix ( const std::vector<float> &listaMatrix ) {

    if ( listaMatrix.size() != 16 ) {
        throw ExceptionChimera ( ExceptionCode::READ, "Tamanho da Matrix invalido" + std::to_string ( listaMatrix.size() ) );
    }

    float ponteiroFloat[16];
    int indice = 0;
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            int pos = i + ( 4 * j );
            ponteiroFloat[pos] = listaMatrix[indice];
            indice++;
        }
    }

    return glm::make_mat4(&ponteiroFloat[0]);
}

glm::mat4 loadTransformMatrix (const char* _matrix) {

    std::vector<float> l_arrayValores;
    loadArrayBtScalar ( _matrix, l_arrayValores );
    return carregaMatrix ( l_arrayValores );

}

std::string retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node ) {

    const char *l_value = _node->Attribute ( _atributo.c_str() );
    if ( l_value != nullptr ) {
        return std::string ( l_value );
    }

    std::cout << std::string ( "Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado" ) << std::endl;
    return std::string ( "" );
}

void loadNodeLib ( tinyxml2::XMLElement* _root, const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode ) {

    tinyxml2::XMLElement* l_nLib = _root->FirstChildElement ( _libName );
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ( _nodeName );
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {

                const char *l_id = l_nNodeBase->Attribute ( "id" );
                if ( strcmp ( l_id, _url ) == 0 ) {

                    *_nNode = l_nNodeBase;
                    return;
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( _url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( _nodeName ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( _libName ) );
    }
}

}