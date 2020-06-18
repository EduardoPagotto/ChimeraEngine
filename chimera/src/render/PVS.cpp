#include "chimera/render/PVS.hpp"

namespace Chimera {

PVS::PVS() {
    NumberOfPolygons = 0;
    NumberOfNodes = 0;
    NumberOfLeafs = 0;
    NumberOfPlanes = 0;
    NumberOfPortals = 0;
}

unsigned int PVS::selectBestSplitter(std::vector<Triangle*>& _poliyList) {

    if (_poliyList.size() == 0)
        return 0;

    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (unsigned indice_splitter = 0; indice_splitter < _poliyList.size(); indice_splitter++) {

        if (_poliyList[indice_splitter]->beenUsedAsSplitter == true)
            continue;

        Triangle* th = _poliyList[indice_splitter];
        Plane hyperPlane(vPosVal(th, 0), th->getNormal());

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        for (unsigned indice_current = 0; indice_current < _poliyList.size(); indice_current++) {

            if (indice_current != indice_splitter) {

                Triangle* currentPoly = _poliyList[indice_current];
                SIDE result = hyperPlane.classifyPoly(vPosVal(currentPoly, 0), vPosVal(currentPoly, 1), vPosVal(currentPoly, 2), &temp);
                switch (result) {
                    case SIDE::CP_ONPLANE:
                        break;
                    case SIDE::CP_FRONT:
                        frontfaces++;
                        break;
                    case SIDE::CP_BACK:
                        backfaces++;
                        break;
                    case SIDE::CP_SPANNING:
                        splits++;
                        break;
                    default:
                        break;
                }
            }
        } // end while current poly

        score = abs(frontfaces - backfaces) + (splits * 8);

        if (score < bestScore) {
            bestScore = score;
            selectedPoly = indice_splitter;
        }
    }

    Triangle* th = _poliyList[selectedPoly];
    th->beenUsedAsSplitter = true;
    Plane* p = new Plane();

    p->set(vPosVal(th, 0), th->getNormal());
    this->planes.push_back(p);

    return this->planes.size() - 1; // plane create using poly selected above
}

void PVS::buildBspTree(long Node, std::vector<Triangle*>& PolyList) {

    nodeArray[Node].plane = selectBestSplitter(PolyList);
    nodeArray[Node].boundingBox.setPosition(glm::vec3(0.0f), glm::vec3(4000));

    std::vector<Triangle*> FrontList;
    std::vector<Triangle*> BackList;

    // polyTest = PolyList;
    // while (PolyList.empty() == false) {
    float result;
    glm::vec3 resultClip(0.0f);

    for (auto it = PolyList.begin(); it != PolyList.end(); it++) {

        Triangle* polyTest = *it;

        SIDE clipTest = planeArray[nodeArray[Node].plane].classifyPoly(vPosVal(polyTest, 0), // PA old poly.vertex[0].position
                                                                       vPosVal(polyTest, 1), // PB
                                                                       vPosVal(polyTest, 2), // PC
                                                                       &resultClip);         // Clip Test Result (A,B,C)

        switch (clipTest) {
            case SIDE::CP_ONPLANE: {
                if (planeArray[nodeArray[Node].plane].collinearNormal(polyTest->getNormal()) == false) {
                    // polyTest->Next = FrontList;
                    FrontList.push_back(polyTest);
                } else {
                    // polyTest->Next = BackList;
                    BackList.push_back(polyTest);
                }
            } break;

            case SIDE::CP_FRONT:
                // polyTest->Next = FrontList;
                FrontList.push_back(polyTest);
                break;

            case SIDE::CP_BACK:
                // polyTest->Next = BackList;
                BackList.push_back(polyTest);
                break;
            case SIDE::CP_SPANNING:
                // TODO: DEPOIS!!!! muita merda aqui!!!
                break;

            default:
                break;
        }
    }

    int count = 0;
    for (Triangle* th : FrontList) {
        if (th->beenUsedAsSplitter == false)
            count++;
    }

    // CalculateBox(&NodeArray[Node].BoundingBox, FrontList);
    AABB LeafBox = nodeArray[Node].boundingBox;
    // CalculateBox(&NodeArray[Node].BoundingBox, BackList);

    if (count == 0) {
        // POLYGON* Iterator = FrontList;
        // POLYGON* Temp;
        // LeafArray[NumberOfLeafs].StartPolygon = NumberOfPolygons;

        // No loop adiciona os vertices a lista de desenho
        // while (Iterator != NULL) {
        //     // PolygonArray[NumberOfPolygons] = *Iterator;
        //     // IncreaseNumberOfPolygons();
        //     // Temp = Iterator;
        //     // Iterator = Iterator->Next;
        //     // delete Temp; // we use the normal delete operator here because we dont want to delete the actual vertexLists and index
        //     lists
        //     //              // because we are still referenceing them in the Polygon Array
        // }
        LeafArray[NumberOfLeafs].EndPolygon = NumberOfPolygons;
        LeafArray[NumberOfLeafs].BoundingBox = LeafBox;
        nodeArray[Node].front = NumberOfLeafs;
        nodeArray[Node].IsLeaf = 1;
        // IncreaseNumberOfLeafs();
    } else {
        nodeArray[Node].IsLeaf = 0;
        nodeArray[Node].front = NumberOfNodes + 1;
        // IncreaseNumberOfNodes();
        buildBspTree(NumberOfNodes, FrontList);
    }

    if (BackList.empty() == true) {
        nodeArray[Node].back = -1;
    } else {
        nodeArray[Node].back = NumberOfNodes + 1;
        // IncreaseNumberOfNodes();
        buildBspTree(NumberOfNodes, BackList);
    }
}

} // namespace Chimera