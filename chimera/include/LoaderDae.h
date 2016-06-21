#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>
#include "Node.h"
#include "PhysicsControl.h"

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
    tinyxml2::XMLElement* findSceneLib ( const char *rotina, const char* instance, const char* library );

    std::string textureDir;
    std::string modelDir;
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

    Chimera::PhysicsControl *pPhysicsControl;
};
}


#endif
