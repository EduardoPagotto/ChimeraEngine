#pragma once
#include "chimera_core/visible/Mesh.hpp"
#include "chimera_space/BSPTreeNode.hpp"
#include "chimera_space/Triangle.hpp"

namespace ce {
    class BspTree { // Ref: https://github.com/taylorstine/BSP_Tree
      public:
        BspTree() = default;
        virtual ~BspTree() = default;
        BSPTreeNode* create(Mesh& mesh, std::vector<TrisIndex>& vp_leaf_out);

      private:
        // bool tringleListIsConvex(std::vector<std::shared_ptr<Triangle>>& _vTriangle);
        BSPTreeNode* build(std::list<std::shared_ptr<Triangle>>& v_triangle);
        std::shared_ptr<Triangle> select_best_splitter(std::list<std::shared_ptr<Triangle>>& v_triangle);
        void split_triangle(const glm::vec3& fx, std::shared_ptr<Triangle> p_triangle, Plane& hyper_plane,
                            std::list<std::shared_ptr<Triangle>>& v_triangle);
        void create_leafy(BSPTreeNode* tree, std::list<std::shared_ptr<Triangle>>& v_triangle);

        std::vector<VertexData> vertex_;
        std::vector<TrisIndex> vp_leaf_;
    };
} // namespace ce
