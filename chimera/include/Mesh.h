#ifndef _MESH_H__
#define _MESH_H__

#include <vector>
#include <string.h>
#include <tinyxml2.h>

#include "Draw.h"
#include <glm/glm.hpp>
#include "VboIndexer.hpp"

namespace Chimera {
    
class Mesh : public Draw
{
public:
    friend class LoaderDae;

    Mesh (Node *_parent, std::string _name );
	Mesh(const Mesh &_cpy);

    virtual ~Mesh();
	virtual void update(DataMsg *dataMsg);
	virtual void init();
    virtual void renderExecute ( bool _texture );
    virtual btVector3 getSizeBox();

    void loadCollada ( tinyxml2::XMLElement* _nNode );

private:
    void setVertexBuffer();   
    int getSource ( tinyxml2::XMLElement* _source, std::vector<float> &_arrayValores );
    void debugDados();
    
    void renterTeste( bool _texture );
    
    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;
    
    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;
    
    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;
        
    //GLfloat* vertices;
    //GLfloat* colors;
    //GLfloat* normals;
    //GLfloat* texturecs;
    
    //GLuint VertexVBOID;
    //GLuint IndexVBOID;

    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
//     std::vector<glm::vec3> vertexVBO;
//     std::vector<glm::vec2> textureVBO;
//     std::vector<glm::vec3> normalVBO;
    
    //GLfloat *pTex;
    //GLfloat *pNorm;  
    
//     GLint *vertexArrayID;
//     GLint vertexbuffer;
//     GLint elementbuffer;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
