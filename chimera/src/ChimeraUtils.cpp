#include <iostream>
#include "ChimeraUtils.h"
#include "ExceptionChimera.h"

namespace Chimera {

	void loadArrayBtScalar(const char *_val, std::vector<btScalar> &_arrayF) {

		char l_numTemp[32];
		const char *pchFim;
		const char *pchInit = _val;
		const char *deadEnd = _val + strlen(_val);
		do {
			pchFim = strchr(pchInit, ' ');
			if (pchFim == nullptr)
				pchFim = deadEnd;

			int l_tam = pchFim - pchInit;
			memcpy(l_numTemp, pchInit, l_tam);
			l_numTemp[l_tam] = 0;
			_arrayF.push_back((float)atof(l_numTemp));

			pchInit = pchFim + 1;
		} while (pchInit < deadEnd);
	}


	void loadArrayI(const char *_val, std::vector<int> &_arrayI) {
		char l_numTemp[32];
		const char *pchFim;
		const char *pchInit = _val;
		const char *deadEnd = _val + strlen(_val);
		do {
			pchFim = strchr(pchInit, ' ');
			if (pchFim == nullptr)
				pchFim = deadEnd;

			int l_tam = pchFim - pchInit;
			memcpy(l_numTemp, pchInit, l_tam);
			l_numTemp[l_tam] = 0;
			_arrayI.push_back(atoi(l_numTemp));

			pchInit = pchFim + 1;
		} while (pchInit < deadEnd);
	}

	void loadTransformMatrix(tinyxml2::XMLElement* _nNode, btTransform *_pTransform) {
		
		std::vector<float> l_arrayValores;
		const char* l_matrix = _nNode->GetText();
		loadArrayBtScalar(l_matrix, l_arrayValores);
		carregaMatrix(_pTransform, l_arrayValores);
		
	}
	
	void carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix) {
		
		if (listaMatrix.size() != 16)
			throw ExceptionChimera(ExceptionCode::READ, "Tamanho da Matrix invalido" + std::to_string(listaMatrix.size()));
		
		btScalar ponteiroFloat[16];
		int indice = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int pos = i + (4 * j);
				ponteiroFloat[pos] = listaMatrix[indice];
				indice++;
			}
		}
		
		_pTrans->setFromOpenGLMatrix(ponteiroFloat);
	}
	
	std::string retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node) {
		
		const char *l_value = _node->Attribute(_atributo.c_str());
		if (l_value != nullptr) {
			return std::string(l_value);
		}
		
		std::cout << std::string("Atributo [ " + _atributo + " ] Elemento [ " + _node->Value() + " ] nao encontrado") << std::endl;
		return std::string("");
	}
	
}