#pragma once
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/BSPTreeNode.hpp"
#include "chimera_space/Triangle.hpp"

namespace ce {
    class BspTree { // Ref: https://github.com/taylorstine/BSP_Tree
      public:
        BspTree() = default;
        virtual ~BspTree() = default;
        BSPTreeNode* create(Mesh& mesh, std::vector<TrisIndex>& vpLeafOut);

      private:
        // bool tringleListIsConvex(std::vector<std::shared_ptr<Triangle>>& _vTriangle);
        BSPTreeNode* build(std::list<std::shared_ptr<Triangle>>& _vTriangle);
        std::shared_ptr<Triangle> selectBestSplitter(std::list<std::shared_ptr<Triangle>>& _vTriangle);
        void splitTriangle(const glm::vec3& fx, std::shared_ptr<Triangle> _pTriangle, Plane& hyperPlane,
                           std::list<std::shared_ptr<Triangle>>& _vTriangle);
        void createLeafy(BSPTreeNode* tree, std::list<std::shared_ptr<Triangle>>& _vTriangle);

        std::vector<VertexData> vertex_;
        std::vector<TrisIndex> vp_leaf_;
    };
} // namespace ce
