#ifndef _MESH_H__
#define _MESH_H__

#include <vector>
#include <string.h>
#include <tinyxml2.h>

#include "Draw.h"

namespace Chimera {
    
template <class T>
class ListPtr
{
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
            delete[] ptr;
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


class Mesh : public Draw
{
public:
    friend class Loader;

    Mesh (Node *_parent, std::string _name );
	Mesh(const Mesh &_cpy);

    virtual ~Mesh();
	virtual void update(DataMsg *dataMsg);
	virtual void init();
    virtual void renderExecute ( bool _texture );
    virtual btVector3 getSizeBox();

    void loadCollada ( tinyxml2::XMLElement* _nNode );

    ListPtr<float> vList;
    ListPtr<int> vIndex;

private:

    int getSource ( tinyxml2::XMLElement* _source, ListPtr<float> &_arrayValores );

    ListPtr<float> nList;
    ListPtr<float> uvList;

    ListPtr<int> nIndex;
    ListPtr<int> tIndex;

};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
