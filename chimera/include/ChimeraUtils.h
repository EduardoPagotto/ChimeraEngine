#ifndef CHIMERA_UTILS_H_
#define CHIMERA_UTILS_H_

#include "string.h"

#include <vector>
#include <LinearMath/btScalar.h>

namespace Chimera {
    
void loadArrayBtScalar ( const char *_val, std::vector<btScalar> &_arrayF );
void loadArrayI ( const char *_val, std::vector<int> &_arrayI );    

}




#endif