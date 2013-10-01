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
        m_ptr = NULL;
        m_size = 0;
    }

    ~ListPtr() {
        clear();
    }

    ListPtr ( const ListPtr &_cpy ) {
        if ( _cpy.m_ptr != NULL ) {
            create ( _cpy.m_size );
            memcpy ( m_ptr, _cpy.m_ptr, sizeof ( T ) * m_size );
        } else {
            m_ptr = NULL;
            m_size = 0;
        }
    }

    void set ( const ListPtr &_cpy ) {
        if ( _cpy.m_ptr != NULL ) {
            create ( _cpy.m_size );
            memcpy ( m_ptr, _cpy.m_ptr, sizeof ( T ) * m_size );
        } else {
            m_ptr = NULL;
            m_size = 0;
        }
    }

    void create ( unsigned _size ) {
        m_ptr = new T[_size];
        m_size = _size;
    }

    void clear ( void ) {
        if ( m_ptr != NULL ) {
            delete [] m_ptr;
            m_ptr = NULL;
            m_size = 0;
        }
    }

    unsigned size() const {
        return m_size;
    }

    inline T & operator[] ( int n ) {
        return m_ptr[n];
    }

    inline T operator[] ( int n ) const {
        return m_ptr[n];
    }

    T* ptr ( void ) {
        return m_ptr;
    }

private:
    T* m_ptr;
    unsigned m_size;
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
    ListPtr<float> m_vertexList;
    ListPtr<float> m_normalList;
    ListPtr<float> m_uv;

    ListPtr<int> m_vertexIndex;
    ListPtr<int> m_normalIndex;
    ListPtr<int> m_textureIndex;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
