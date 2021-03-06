#ifndef __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES__HPP
#define __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/node/NodeGroup.hpp"

namespace ChimeraLoaders {

class LibraryVisualScenes : public Library {

  public:
    LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string& _url, Chimera::NodeGroup* _pRootNode);
    virtual ~LibraryVisualScenes();
    void target();

  private:
    glm::mat4 getTransformation(tinyxml2::XMLElement* _nNode);
    void carregaNode(Chimera::Node* _pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name,
                     const char* type);
    ListNodes* pListNodes;
    Chimera::NodeGroup* pRootNode;
};
} // namespace ChimeraLoaders

#endif