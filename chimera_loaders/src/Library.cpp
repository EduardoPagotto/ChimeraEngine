#include "Library.h"
#include "ExceptionChimera.h"

namespace ChimeraLoaders {

Library::Library(tinyxml2::XMLElement* _root, const std::string &_url) {

    std::size_t posicao = _url.find('#');// != std::string::npos
    if (posicao != 0) {

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
}