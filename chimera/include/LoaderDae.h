#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>
#include "Node.h"
#include "PhysicsControl.h"

#include "Material.h"
#include "Effect.h"
#include "Texture.h"
#include "DrawTriMesh.h"

namespace ChimeraLoader
{

class LoaderDae
{
public:
    LoaderDae ( const std::string &textureDir, const std::string &modelDir );
    virtual ~LoaderDae();
    Chimera::Graph::Node *loadFile ( const std::string &file );

private:
    void getPhysicSceneInfile();
    void getDadosInstancePhysicModel ( tinyxml2::XMLElement* _nPhysicScene );
    void carregaNode ( Chimera::Graph::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type );
    Chimera::Graph::Node* getNodeSceneInFile();
    tinyxml2::XMLElement* getDadoRigidBody ( const char* _url, const char* _sid );
    
    static tinyxml2::XMLElement* findSceneLib (tinyxml2::XMLElement* pRoot, const char *rotina, const char* instance, const char* library );

    std::string textureDir;
    std::string modelDir;
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

    std::map<std::string, Chimera::Graph::Effect*> mapaEfeito; 
    std::map<std::string, Chimera::Graph::Texture*> mapaTextura;
    std::map<std::string, Chimera::Graph::Material*> mapaMaterial;
    std::map<std::string, Chimera::Graph::Draw*> mapaGeometria;
    
    Chimera::PhysicsControl *pPhysicsControl;
};
}


#endif
