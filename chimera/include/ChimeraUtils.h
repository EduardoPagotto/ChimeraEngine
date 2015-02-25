#ifndef CHIMERA_UTILS_H_
#define CHIMERA_UTILS_H_

#include <string>
#include <vector>
#include <tinyxml2.h>
#include <LinearMath/btScalar.h>
#include <LinearMath/btTransform.h>

namespace Chimera {
	void loadArrayBtScalar(const char *_val, std::vector<btScalar> &_arrayF);
	void loadArrayI(const char *_val, std::vector<int> &_arrayI);
	void loadTransformMatrix(tinyxml2::XMLElement* _nNode, btTransform *_pTransform);
	void carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix);
	std::string retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node);

	void loadNodeLib(tinyxml2::XMLElement* _root, const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode);
}

#endif