#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(ListPolygon* polygons);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static BSPTreeNode* buildBSPTreeNode(ListPolygon polygons);
    static SIDE classifyPolygon(Polygon* _pPartition, Polygon* _pPolygon);
    static void splitPolygon(Polygon* _poly, Polygon* _partition, Polygon* _front_piece, Polygon* _back_piece);

    BSPTreeNode* root;
};

#endif