#ifndef CHIMERA_UTILS_H_
#define CHIMERA_UTILS_H_

#include <string>
#include <vector>
#include <tinyxml2.h>
#include <glm/glm.hpp>

namespace Chimera
{
void loadArrayBtScalar ( const char *_val, std::vector<float> &_arrayF );
void loadArrayI ( const char *_val, std::vector<int> &_arrayI );
glm::mat4 loadTransformMatrix ( const char* _matrix);
glm::mat4 carregaMatrix (const std::vector<float> &listaMatrix );
std::string retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node );
void loadNodeLib ( tinyxml2::XMLElement* _root, const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode );
}

#endif