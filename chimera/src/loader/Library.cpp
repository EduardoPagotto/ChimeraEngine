#include "chimera/loader/Library.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Library::Library(tinyxml2::XMLElement* _root, const std::string& _url) {

    std::size_t posicao = _url.find('!');
    if (posicao == 0) {

        // carrega arquivo root
        std::string file = _url.substr(posicao + 1, std::string::npos);

        doc = new tinyxml2::XMLDocument();

        // Verifica se arquivo existe
        tinyxml2::XMLError a_eResult = doc->LoadFile(file.c_str());
        if (a_eResult != 0) {
            throw std::string("Falha ao ler arquivo erro: " + std::to_string(a_eResult));
        }

        // vefifica se ele � uma estrutura compativel com collada
        root = doc->FirstChildElement("COLLADA");
        if (root == nullptr) {
            throw std::string("Nao é um arquivo colada: " + file);
        }

        file_atual = file;

        return;
    }

    posicao = _url.find('#');

    if (posicao == std::string::npos) {

        // Carrega tag se # exlicito (local file)
        doc = nullptr;
        root = _root;
        url = _url;

    } else if (posicao != 0) {

        // Carrega tag de outro arquivo
        std::string file = _url.substr(0, posicao - 1);

        if (file_atual.compare(file) == 0) {
            // Arqivo apntado e o mesmo que estou
            doc = nullptr;
            root = _root;
            url = _url;
            return;
        }

        url = _url.substr(posicao + 1, std::string::npos);
        doc = new tinyxml2::XMLDocument();

        // Verifica se arquivo existe
        tinyxml2::XMLError a_eResult = doc->LoadFile(file.c_str());
        if (a_eResult != 0) {
            throw std::string("Falha ao ler arquivo erro: " + std::to_string(a_eResult));
        }

        // vefifica se ele � uma estrutura compativel com collada
        root = doc->FirstChildElement("COLLADA");
        if (root == nullptr) {
            throw std::string("Nao é um arquivo colada: " + file);
        }

    } else {

        // carrega com tag na posicao zero
        doc = nullptr;
        root = _root;
        url = _url.substr(posicao + 1, std::string::npos);
    }
}

Library::~Library() {
    if (doc != nullptr) {
        delete doc;
        doc = nullptr;
    }
}

std::string Library::getIdFromUrl(const std::string& _url) {

    std::size_t posicao = _url.find('#');
    if (posicao == std::string::npos)
        return _url;

    return _url.substr(posicao + 1, std::string::npos);
}

tinyxml2::XMLElement* Library::findExtra(tinyxml2::XMLElement* _nNode) {

    for (tinyxml2::XMLElement* l_nTec = _nNode->FirstChildElement("extra")->FirstChildElement("technique"); l_nTec != nullptr;
         l_nTec = l_nTec->NextSiblingElement()) {
        const char* l_profile = l_nTec->Attribute("profile");
        if (strcmp(l_profile, (const char*)"chimera") == 0) {
            return l_nTec;
        }
    }
    return nullptr;
}

void Library::loadArrayBtScalar(const char* _val, std::vector<float>& _arrayF) {

    char l_numTemp[32];
    const char* pchFim;
    const char* pchInit = _val;
    const char* deadEnd = _val + strlen(_val);
    do {
        pchFim = strchr(pchInit, ' ');
        if (pchFim == nullptr) {
            pchFim = deadEnd;
        }

        int l_tam = pchFim - pchInit;
        memcpy(l_numTemp, pchInit, l_tam);
        l_numTemp[l_tam] = 0;
        _arrayF.push_back((float)atof(l_numTemp));

        pchInit = pchFim + 1;
    } while (pchInit < deadEnd);
}

void Library::loadArrayI(const char* _val, std::vector<int>& _arrayI) {
    char l_numTemp[32];
    const char* pchFim;
    const char* pchInit = _val;
    const char* deadEnd = _val + strlen(_val);
    do {
        pchFim = strchr(pchInit, ' ');
        if (pchFim == nullptr) {
            pchFim = deadEnd;
        }

        int l_tam = pchFim - pchInit;
        memcpy(l_numTemp, pchInit, l_tam);
        l_numTemp[l_tam] = 0;
        _arrayI.push_back(atoi(l_numTemp));

        pchInit = pchFim + 1;
    } while (pchInit < deadEnd);
}

glm::mat4 Library::carregaMatrix(const std::vector<float>& listaMatrix) {

    if (listaMatrix.size() != 16) {
        throw std::string("Tamanho da Matrix invalido" + std::to_string(listaMatrix.size()));
    }

    float ponteiroFloat[16];
    int indice = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int pos = i + (4 * j);
            ponteiroFloat[pos] = listaMatrix[indice];
            indice++;
        }
    }

    return glm::make_mat4(&ponteiroFloat[0]);
}

glm::mat4 Library::loadTransformMatrix(const char* _matrix) {

    std::vector<float> l_arrayValores;
    loadArrayBtScalar(_matrix, l_arrayValores);
    return carregaMatrix(l_arrayValores);
}
} // namespace Chimera