#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>

#include "PhysicsControl.h"
#include "Material.h"
#include "Group.h"
#include "Solid.h"
#include "Draw.h"
#include "TextureManager.h"

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

    std::map<std::string, Solid*> mapaEntidadeFisica;
    std::map<std::string, Draw*> mapaGeometria;
    
    Group *pRootNode;
    
    PhysicsControl *pPhysicsControl;
	TextureManager *texManager;
};
}


#endif
