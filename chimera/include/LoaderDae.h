#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>

#include "PhysicsControl.h"
#include "Material.h"
#include "DrawTriMesh.h"
#include "Object.h"
#include "Group.h"

namespace Chimera
{

class LoaderDae
{
public:
    LoaderDae ( const std::string &_textureDir, const std::string &_modelDir,const std::string &_file );
    virtual ~LoaderDae();
	
    Group* getNodes() {
        return pRootNode;
    }
    
private:
    void loadFile ( const std::string &file );
    void getNodeSceneInFile();
    
    void getPhysicSceneInfile();
    void getDadosInstancePhysicModel ( tinyxml2::XMLElement* _nPhysicScene );
    void carregaNode ( Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type );

    tinyxml2::XMLElement* getDadoRigidBody ( const char* _url, const char* _sid );
    
    static tinyxml2::XMLElement* findSceneLib (tinyxml2::XMLElement* pRoot, const char *rotina, const char* instance, const char* library );

    std::string textureDir;
    std::string modelDir;
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

    std::map<std::string, Effect*> mapaEfeito; 
    std::map<std::string, Texture*> mapaTextura;
    std::map<std::string, Material*> mapaMaterial;
    std::map<std::string, Draw*> mapaGeometria;
	std::map<std::string, Object*> mapaObjeto;
    
    Group *pRootNode;
    
    PhysicsControl *pPhysicsControl;
};
}


#endif
