#ifndef MESH_H_
#define MESH_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <vector>
#include <string.h>

#include "Draw.h"

namespace Chimera {

template <class T>
class ListPtr {
public:
    ListPtr() {
        ptr = nullptr;
        size = 0;
    }

    ~ListPtr() {
        clear();
    }

    ListPtr ( const ListPtr &_cpy ) {
        if ( _cpy.ptr != nullptr ) {
            create ( _cpy.size );
            memcpy ( ptr, _cpy.ptr, sizeof ( T ) * size );
        } else {
            ptr = nullptr;
            size = 0;
        }
    }

    void set ( const ListPtr &_cpy ) {
        if ( _cpy.ptr != nullptr ) {
            create ( _cpy.size );
            memcpy ( ptr, _cpy.ptr, sizeof ( T ) * size );
        } else {
            ptr = nullptr;
            size = 0;
        }
    }

    void create ( unsigned _size ) {
        ptr = new T[_size];
        size = _size;
    }

    void clear ( void ) {
        if ( ptr != nullptr ) {
            delete [] ptr;
            ptr = nullptr;
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


class DrawTriMesh : public Draw {
public:
    friend class Loader;
    
    DrawTriMesh(std::string _id, std::string _name);
    DrawTriMesh ( const DrawTriMesh &_cpy );
    virtual ~DrawTriMesh();
    
    virtual void update ( DataMsg *dataMsg );
    
    virtual void init();
    virtual void render();
    virtual btVector3 getSizeBox();

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
