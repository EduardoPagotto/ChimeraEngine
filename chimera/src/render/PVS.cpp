#include "chimera/render/PVS.hpp"

namespace Chimera {

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
    Plane* p = new Plane();

    p->set(vPosVal(th, 0), th->getNormal());
    this->planes.push_back(p);

    return this->planes.size() - 1; // the last
}
} // namespace Chimera