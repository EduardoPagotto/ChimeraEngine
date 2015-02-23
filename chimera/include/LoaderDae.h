#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <vector>
#include <string>
#include <LinearMath/btScalar.h>
#include <LinearMath/btTransform.h>
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "DrawTriMesh.h"

namespace ChimeraLoader {

	class LoaderDae {
	public:
		LoaderDae(const std::string &textureDir, const std::string &file);
		virtual ~LoaderDae();

	private:

		tinyxml2::XMLElement* findVisualScene();

		void carregaNode(tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type);
		
		void loadTransformMatrix(tinyxml2::XMLElement* _nNode, btTransform *_pTransform);
		void loadArrayBtScalar(const char *_val, std::vector<btScalar> &_arrayF);
		void carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix);
		
		std::string retornaAtributo(const std::string &_atributo, tinyxml2::XMLElement* _node);
		
		void loadCamera(const char* _id, Chimera::Camera **_ppCam);
		void loadLight(const char* _id, Chimera::Light **_ppLight);
		
		std::string textureDir;
		tinyxml2::XMLDocument* doc;
		tinyxml2::XMLElement* root;
		
		Chimera::Node *pRootScene;

	};
}


#endif
