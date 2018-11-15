#ifndef __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES_H
#define __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES_H

#include "Library.h"
#include "Group.h"
#include "ListNodes.h"

namespace ChimeraLoaders {

class LibraryVisualScenes: public Library {

public:
    LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string &_url, Chimera::Group *_pRootNode);
    virtual ~LibraryVisualScenes();
    void target();
private:
    glm::mat4 getTransformation(tinyxml2::XMLElement* _nNode);
    void carregaNode ( Chimera::Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type );
    ListNodes *pListNodes;
    Chimera::Group *pRootNode;
};
}

#endif