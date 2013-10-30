#include "ChimeraUtils.h"

namespace Chimera {

void loadArrayBtScalar ( const char *_val, std::vector<btScalar> &_arrayF ) {
    
    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit,' ' );
        if ( pchFim==nullptr )
            pchFim = deadEnd;
        
        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp,pchInit,l_tam );
        l_numTemp[l_tam ] = 0;
        _arrayF.push_back ( ( float ) atof ( l_numTemp ) );
        
        pchInit = pchFim+1;
    } while ( pchInit < deadEnd );
}


void loadArrayI ( const char *_val, std::vector<int> &_arrayI ) {
    char l_numTemp[32];
    const char *pchFim;
    const char *pchInit = _val;
    const char *deadEnd = _val + strlen ( _val );
    do {
        pchFim = strchr ( pchInit,' ' );
        if ( pchFim==nullptr )
            pchFim = deadEnd;

        int l_tam = pchFim - pchInit;
        memcpy ( l_numTemp,pchInit,l_tam );
        l_numTemp[l_tam ] = 0;
        _arrayI.push_back ( atoi ( l_numTemp ) );

        pchInit = pchFim+1;
    } while ( pchInit < deadEnd );
}

}