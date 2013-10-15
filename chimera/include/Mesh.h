#ifndef MESH_H_
#define MESH_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include <vector>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Draw.h"

namespace Chimera {

template <class T>
class ListPtr {
public:
    ListPtr() {
        ptr = NULL;
        size = 0;
    }

    ~ListPtr() {
        clear();
    }

    ListPtr ( const ListPtr &_cpy ) {
        if ( _cpy.ptr != NULL ) {
            create ( _cpy.size );
            memcpy ( ptr, _cpy.ptr, sizeof ( T ) * size );
        } else {
            ptr = NULL;
            size = 0;
        }
    }

    void set ( const ListPtr &_cpy ) {
        if ( _cpy.ptr != nullptr ) {
            create ( _cpy.size );
            memcpy ( ptr, _cpy.ptr, sizeof ( T ) * size );
        } else {
            ptr = NULL;
            size = 0;
        }
    }

    void create ( unsigned _size ) {
        ptr = new T[_size];
        size = _size;
    }

    void clear ( void ) {
        if ( ptr != NULL ) {
            delete [] ptr;
            ptr = NULL;
            size = 0;
        }
    }

    unsigned getSize() const {
        return size;
    }

    inline T & operator[] ( int n ) {
        return ptr[n];
    }

    inline T operator[] ( int n ) const {
        return ptr[n];
    }

    T* ptrVal ( void ) {
        return ptr;
    }

private:
    T* ptr;
    unsigned size;
};


class Mesh : public Draw {
public:
    friend class Loader;
    Mesh();
    Mesh ( const Mesh &_cpy );
    virtual ~Mesh();
    virtual btVector3 sizeQuadratic ( void );

protected:
    virtual void renderizar();
private:
    ListPtr<float> vList;
    ListPtr<float> nList;
    ListPtr<float> uvList;

    ListPtr<int> vIndex;
    ListPtr<int> nIndex;
    ListPtr<int> tIndex;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
