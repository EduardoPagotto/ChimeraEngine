#include "chimera/node/Color.hpp"

namespace Chimera {

const Color Color::ZERO = Color(0.0, 0.0, 0.0, 0.0);
const Color Color::BLACK = Color(0.0, 0.0, 0.0);
const Color Color::WHITE = Color(1.0, 1.0, 1.0);
const Color Color::RED = Color(1.0, 0.0, 0.0);
const Color Color::GREEN = Color(0.0, 1.0, 0.0);
const Color Color::BLUE = Color(0.0, 0.0, 1.0);

void Color::saturate(void) {
    for (int i = 0; i++; i < 3) {
        if (value[i] < 0)
            value[i] = 0;
        else if (value[i] > 1)
            value[i] = 1;
    }
}

Color Color::saturateCopy(void) const {
    Color ret = *this;
    ret.saturate();
    return ret;
}
} // namespace Chimera
