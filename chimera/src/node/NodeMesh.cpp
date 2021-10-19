#include "chimera/node/NodeMesh.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/node/VisitorInterface.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iterator>

namespace Chimera {

NodeMesh::NodeMesh(Node* _parent, std::string _name) : Node(_parent, Kind::MESH, _name) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor NodeMesh: %s", _name.c_str());
}

NodeMesh::NodeMesh(const NodeMesh& _cpy) : Node(_cpy) {
    // FIXME: criar a copia do meshdata
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Copy NodeMesh %s", _cpy.getName().c_str());
}

NodeMesh::~NodeMesh() {
    // FIXME: remover o NodeMesh data
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Destructor NodeMesh: %s", getName().c_str());
}

void NodeMesh::init() {

    if (material == nullptr)
        material = new Material();

    material->init();

    glm::vec3 min, max, size;
    vertexDataMeshMinMaxSize(&meshData, min, max, size);

    aabb.setBoundary(min, max);

    pTransform->init(size);

    setVertexBuffer();
}

void NodeMesh::accept(VisitorInterface* v) { v->visit(this); }

void NodeMesh::setVertexBuffer() {
    std::vector<VertexData> vertexDataIn;
    vertexDataFromMesh(&meshData, vertexDataIn);

    std::vector<unsigned int> index;
    if (meshData.singleIndex == false) {
        std::vector<VertexData> vertexDataOut;
        vertexDataIndexCompile(vertexDataIn, vertexDataOut, index);
        pRenderStat = new RenderableStatic(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
    } else {
        pRenderStat = new RenderableStatic(&vertexDataIn[0], vertexDataIn.size(), &meshData.vertexIndex[0], meshData.vertexIndex.size());
    }
}

void NodeMesh::debugDados(bool _showAll) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Debug Mess Nome: %s", getName().c_str());
    vertexDataMeshDataDebug(&meshData, _showAll);
}

void NodeMesh::replaceTransform(Transform* _pTransform) {

    glm::mat4 matrix = pTransform->getMatrix();
    _pTransform->setMatrix(matrix);

    delete pTransform;
    pTransform = nullptr;

    pTransform = _pTransform;
}

NodeMesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial) {

    // NodeMesh
    NodeMesh* pMesh = new NodeMesh(_pParent, _name);
    Material* pMatFinal = _pMaterial;
    if (pMatFinal == nullptr)
        pMatFinal = new Material();

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}

void createMeshTexturizade(Node* parent, std::string name, std::string file, float scale, glm::vec3 _position) {

    Material* pMap = new Material();
    NodeMesh* pMesh = new NodeMesh(parent, name);

    int ret = loadObjFile(file, &pMesh->meshData, pMap);
    // TODO: validar erro!!!

    vertexDataMeshScale(&pMesh->meshData, scale, pMap->hasTexture());
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(pMap);
}

} // namespace Chimera
