#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>
#include "Node.h"
#include "PhysicsControl.h"

namespace ChimeraLoader {

	class LoaderDae {
	public:
		LoaderDae(const std::string &textureDir, const std::string &modelDir);
		virtual ~LoaderDae();

		Chimera::Node *loadFile(const std::string &file);

	private:

		Chimera::Node* getNodeSceneInFile();
		
		void getPhysicSceneInfile();
		void getDadosPhysicModel(const char* _url);
		
		void getDadosInstancePhysicModel(tinyxml2::XMLElement* _nPhysicScene);
		
		tinyxml2::XMLElement* findSceneLib(const char *rotina, const char* instance, const char* library);
		
		//tinyxml2::XMLElement* findPhysicScene();
		//tinyxml2::XMLElement* findVisualScene();
		void carregaNode(Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type);
		void loadNodeLib(const char* _url, const char* _libName, const char* _nodeName, tinyxml2::XMLElement** _nNode);

		std::string textureDir;
		std::string modelDir;
		tinyxml2::XMLDocument* doc;
		tinyxml2::XMLElement* root;
		
		
		Chimera::PhysicsControl *pPhysicsControl;
	};
}


#endif
