#include "BSPTree.h"
#include <SDL2/SDL.h>

float classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Conferir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

void parserTree(BSPTreeNode* tree, glm::vec3* eye, std::vector<Chimera::VertexDataFull>* _pOutVertex) {
    if (tree == nullptr)
        return;

    glm::vec3 normal = tree->partition.normal();
    float result = classify(&normal, eye);
    if (result > 0) {

        parserTree(tree->back, eye, _pOutVertex);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
            Chimera::Triangle t = (*it);
            _pOutVertex->push_back(t.vertex[0]);
            _pOutVertex->push_back(t.vertex[1]);
            _pOutVertex->push_back(t.vertex[2]);

            // if (debug_parse == true)
            //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face F: %d", t.getSerial());
        }

        parserTree(tree->front, eye, _pOutVertex);

    } else if (result < 0) {
        parserTree(tree->front, eye, _pOutVertex);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
            Chimera::Triangle t = (*it);
            _pOutVertex->push_back(t.vertex[0]);
            _pOutVertex->push_back(t.vertex[1]);
            _pOutVertex->push_back(t.vertex[2]);

            // if (debug_parse == true)
            //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face B: %d", t.getSerial());
        }

        parserTree(tree->back, eye, _pOutVertex);

    } else {
        // the eye point is on the partition plane...
        parserTree(tree->front, eye, _pOutVertex);
        parserTree(tree->back, eye, _pOutVertex);
    }
}

void drawBSPTree(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexDataFull>* _pOutVertex) {
    parserTree(_pRoot, eye, _pOutVertex);
}