#ifndef SHAPES_H_
#define SHAPES_H_

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include "Draw.h"

namespace Chimera {  

class DrawBox : public Draw {
public:
	DrawBox(float _lado, float _altura, float _largura);
	~DrawBox();

	virtual btVector3 sizeQuadratic(void);

protected:

	virtual void renderizar();

private:
	float m_lado;
	float m_altura;
	float m_largura;
};

class DrawBoxGrid : public Draw {
public:
	DrawBoxGrid(int _size,int _space);
	~DrawBoxGrid();

	virtual btVector3 sizeQuadratic(void);

protected:
	
	virtual void renderizar();

private:
	int m_size;
	int m_space;
};

class DrawBoxGrid2 : public Draw {
public:
	DrawBoxGrid2();
	~DrawBoxGrid2();

	virtual btVector3 sizeQuadratic(void);

protected:
	
	virtual void renderizar();

private:

};

}
#endif //DESENHO3D_H