#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "Plane.hpp"
#include "Triangle.hpp"
#include "VertexData.hpp"
#include <list>
#include <vector>

namespace Chimera {

// TODO: classe sera subistituida por VertexNode (integrada com EBO)
class Leaf { // each leaf to have a maximum of 50 portals which is way to many but lets not
  public:
    Leaf();
    virtual ~Leaf();
    void addFace(uint32_t face, uint32_t _a, uint32_t _b, uint32_t _c);
    std::list<uint32_t> index;
    std::list<uint32_t> faces;
    // AABB boundingBox;
};

class BSPTreeNode {
  public:
    BSPTreeNode(const Plane& _hyperPlane);
    virtual ~BSPTreeNode();
    void destroy();
    void addPolygon(Triangle* _triangle);
    void addIndexPolygon(std::list<Triangle*>& _vTriangle);

    Leaf* pLeaf;
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isSolid;
    bool isLeaf;
};

class BspTreeBase {
  public:
    BspTreeBase();
    virtual ~BspTreeBase();
    void createSequencial(std::vector<Chimera::VertexData>& _vVertex);
    void createIndexed(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex);
    void destroy();

    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData);

  protected:
    virtual BSPTreeNode* bsptreeBuild(std::list<Triangle*>& _vTriangle) = 0;

    void drawPolygon(BSPTreeNode* tree, bool frontSide);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos);

    void collapse(BSPTreeNode* tree);
    Plane selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);

    inline glm::vec3 vPosVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]].position; }
    inline VertexData vVerVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]]; }

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    bool logdata;
    BSPTreeNode* root;

    std::vector<Chimera::VertexData> vVertex;
    std::vector<VertexData>* resultVertex;
};

class BspTree : public BspTreeBase {
  public:
    BspTree();
    virtual ~BspTree();

  protected:
    virtual BSPTreeNode* bsptreeBuild(std::list<Triangle*>& _vTriangle);

  private:
    bool isConvex(std::list<Triangle*>& _vTriangle, Triangle* _poly);
};
} // namespace Chimera
#endif
