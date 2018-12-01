#include "chimera/node/Color.hpp"

namespace Chimera {

const Color Color::ZERO = Color(0.0, 0.0, 0.0, 0.0);
const Color Color::BLACK = Color(0.0, 0.0, 0.0);
const Color Color::WHITE = Color(1.0, 1.0, 1.0);
const Color Color::RED = Color(1.0, 0.0, 0.0);
const Color Color::GREEN = Color(0.0, 1.0, 0.0);
const Color Color::BLUE = Color(0.0, 0.0, 1.0);

bool Color::operator==(const Color& rhs) const { return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a); }

bool Color::operator!=(const Color& rhs) const { return !(*this == rhs); }

void Color::saturate(void) {
    if (r < 0) {
        r = 0;
    } else if (r > 1) {
        r = 1;
    }

    if (g < 0) {
        g = 0;
    } else if (g > 1) {
        g = 1;
    }

    if (b < 0) {
        b = 0;
    } else if (b > 1) {
        b = 1;
    }

    if (a < 0) {
        a = 0;
    } else if (a > 1) {
        a = 1;
    }
}

// Color Color::saturateCopy(void)
//{
//    Color ret = *this;
//    ret.saturate();
//    return ret;
//}
} // namespace Chimera
