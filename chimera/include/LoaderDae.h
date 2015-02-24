#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <vector>
#include <string>
#include <LinearMath/btScalar.h>
#include <LinearMath/btTransform.h>
#include "Node.h"

namespace ChimeraLoader {

	class LoaderDae {
	public:
		LoaderDae(const std::string &textureDir, const std::string &modelDir);
		virtual ~LoaderDae();

		Chimera::Node *loadFile(const std::string &file);

	private:

		tinyxml2::XMLElement* findVisualScene();

		void carregaNode(Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type);
		void loadTransformMatrix(tinyxml2::XMLElement* _nNode, btTransform *_pTransform);
		void loadArrayBtScalar(const char *_val, std::vector<btScalar> &_arrayF);
		void carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix);
		
		std::string retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node);
				
		void loadNodeLib(const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode);

		std::string textureDir;
		std::string modelDir;
		tinyxml2::XMLDocument* doc;
		tinyxml2::XMLElement* root;
		
	};
}


#endif
