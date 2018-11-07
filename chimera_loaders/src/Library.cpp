#include "Library.h"
#include "ExceptionChimera.h"

namespace ChimeraLoaders {

Library::Library(tinyxml2::XMLElement* _root, const std::string &_url) {

    std::size_t posicao = _url.find('!');
    if (posicao == 0 ) {

        //carrega arquivo root
        std::string file = _url.substr(posicao + 1, std::string::npos);

        doc = new tinyxml2::XMLDocument();

        //Verifica se arquivo existe
        tinyxml2::XMLError a_eResult = doc->LoadFile( file.c_str());
        if ( a_eResult != 0 ) {
            throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Falha ao ler arquivo erro: " + std::to_string(a_eResult));
        }

        //vefifica se ele � uma estrutura compativel com collada
        root = doc->FirstChildElement ("COLLADA");
        if ( root == nullptr ) {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Nao é um arquivo colada: " + file);
        }

        return;
    }

    posicao = _url.find('#');

    if (posicao == std::string::npos) {

        //Carrega tag se # exlicito (local file)
        doc = nullptr;
        root = _root;
        url = _url;

    } else if (posicao !=0 ) {

        //Carrega tag de outro arquivo
        std::string file = _url.substr (0, posicao - 1);

        url = _url.substr(posicao + 1, std::string::npos);
        doc = new tinyxml2::XMLDocument();

        //Verifica se arquivo existe
        tinyxml2::XMLError a_eResult = doc->LoadFile( file.c_str());
        if ( a_eResult != 0 ) {
            throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN, "Falha ao ler arquivo erro: " + std::to_string(a_eResult));
        }

        //vefifica se ele � uma estrutura compativel com collada
        root = doc->FirstChildElement ("COLLADA");
        if ( root == nullptr ) {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::OPEN, "Nao é um arquivo colada: " + file);
        }

    } else {

        // carrega com tag na posicao zero
        doc = nullptr;
        root = _root;
        url = _url.substr(posicao + 1, std::string::npos);

    }
}

Library::~Library() {
    if ( doc != nullptr ) {
        delete doc;
        doc = nullptr;
    }
}

tinyxml2::XMLElement* Library::findExtra(tinyxml2::XMLElement* _nNode) {

    tinyxml2::XMLElement* l_nTec = _nNode->FirstChildElement("extra")->FirstChildElement("technique");
    for (l_nTec; l_nTec; l_nTec = l_nTec->NextSiblingElement()) {
        const char* l_profile = l_nTec->Attribute ( "profile" );
        if (strcmp(l_profile, (const char*)"chimera") == 0) {
            return l_nTec;
        }
    }
    return nullptr;
}

int findParams(tinyxml2::XMLElement* _nNode, VectorParam *_pVectorParam) {

    tinyxml2::XMLElement* l_nParam = _nNode->FirstChildElement ( "param" );
    for (l_nParam; l_nParam; l_nParam->NextSiblingElement()) {
        ParamCollada novo;
        novo.name = l_nParam->Attribute ( "name" );
        novo.sid = l_nParam->Attribute ( "sid" );
        novo.type = l_nParam->Attribute ( "type" );
        novo.value = l_nParam->GetText();
        _pVectorParam->push_back(novo);

        l_nParam = l_nParam->NextSiblingElement();    
    }
    return _pVectorParam->size();
}



}