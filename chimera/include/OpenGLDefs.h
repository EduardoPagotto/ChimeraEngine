#ifndef _OPEN_GL_DEFS__H_
#define _OPEN_GL_DEFS__H_

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

namespace Chimera {
    
enum class FaceMaterial{
    FRONT=GL_FRONT,
    BACK=GL_BACK,
    FRONT_BACK=GL_FRONT_AND_BACK
}; 

enum class ModeMaterial {
    AMBIENT=GL_AMBIENT,
    DIFFUSE=GL_DIFFUSE,
    EMISSION=GL_EMISSION,
    SPECULAR=GL_SPECULAR,
    SHININESS=GL_SHININESS,
    AMBIENT_AND_DIFFUSE=GL_AMBIENT_AND_DIFFUSE
};

enum LightNum {
    LIGHT0=GL_LIGHT0,
    LIGHT1=GL_LIGHT1,
    LIGHT2=GL_LIGHT2,
    LIGHT3=GL_LIGHT3,
    LIGHT4=GL_LIGHT4,
    LIGHT5=GL_LIGHT5,
    LIGHT6=GL_LIGHT6,
    LIGHT7=GL_LIGHT7,
    LIGHTING=GL_LIGHTING
};

enum PolygonMode {
    FILL=GL_FILL,
    WIREFRAME=GL_LINE,
    POINTS=GL_POINT
};

enum CullFace {
    CULL_FACE=GL_CULL_FACE
};

enum ColorMaterial {
    COLOR_MATERIAL=GL_COLOR_MATERIAL
};

enum ClientState {
    COLOR_ARRAY=GL_COLOR_ARRAY, 
    EDGE_FLAG_ARRAY=GL_EDGE_FLAG_ARRAY, 
    FOG_COORD_ARRAY=GL_FOG_COORD_ARRAY, 
    INDEX_ARRAY=GL_INDEX_ARRAY, 
    NORMAL_ARRAY=GL_NORMAL_ARRAY, 
    SECONDARY_COLOR_ARRAY=GL_SECONDARY_COLOR_ARRAY, 
    TEXTURE_COORD_ARRAY=GL_TEXTURE_COORD_ARRAY, 
    VERTEX_ARRAY=GL_VERTEX_ARRAY
};
}
#endif
