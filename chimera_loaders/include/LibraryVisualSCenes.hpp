#ifndef __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES__HPP
#define __CHIMERA_LOADER_LIBRARY_VISUAL_SCENES__HPP

#include "Group.hpp"
#include "Library.hpp"
#include "ListNodes.hpp"

namespace ChimeraLoaders {

class LibraryVisualScenes : public Library {

  public:
    LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string& _url,
                        Chimera::Group* _pRootNode);
    virtual ~LibraryVisualScenes();
    void target();

  private:
    glm::mat4 getTransformation(tinyxml2::XMLElement* _nNode);
    void carregaNode(Chimera::Node* _pNodePai, tinyxml2::XMLElement* _nNode,
                     const char* _id, const char* _name, const char* type);
    ListNodes* pListNodes;
    Chimera::Group* pRootNode;
};
} // namespace ChimeraLoaders

#endif