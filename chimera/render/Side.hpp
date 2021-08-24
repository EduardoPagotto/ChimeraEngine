#ifndef __CHIMERA_SIDE__HPP
#define __CHIMERA_SIDE__HPP

namespace Chimera {

#define EPSILON 1e-2
//#define EPSILON 1e-1 // Falhas em calculos de aproximação verificar projeto bsptree

enum class SIDE { CP_ONPLANE = 0, CP_FRONT, CP_BACK, CP_SPANNING };

} // namespace Chimera
#endif