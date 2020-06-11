#include "chimera/render/PVS.hpp"

namespace Chimera {

unsigned int PVS::selectBestSplitter(std::vector<Tris>& _poliyList) {

    if (_poliyList.size() == 0)
        return 0;

    unsigned int selectedPoly = 0;
    unsigned int bestScore = 100000; // just set to a very high value to begin
    glm::vec3 temp;                  // inutil

    for (unsigned indice_splitter = 0; indice_splitter < _poliyList.size(); indice_splitter++) {

        if (_poliyList[indice_splitter].beenUsedAsSplitter == true)
            continue;

        PlanePoint hyperPlane(_poliyList[indice_splitter].triangle);

        long long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;

        for (unsigned indice_current = 0; indice_current < _poliyList.size(); indice_current++) {

            if (indice_current != indice_splitter) {

                Tris currentPoly = _poliyList[indice_current];

                // SIDE result = classifyPoly(&hyperPlane, currentPoly.triangle, &temp);
                SIDE result = hyperPlane.classifyPoly(currentPoly.triangle, &temp);
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

    this->planes.push_back(new PlanePoint(_poliyList[selectedPoly].triangle));

    return this->planes.size() - 1; //?
}
} // namespace Chimera