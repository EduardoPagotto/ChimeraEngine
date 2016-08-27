#ifndef _MESH_H__
#define _MESH_H__

#include <vector>
#include <string.h>
#include <tinyxml2.h>

#include "Draw.h"
#include <glm/glm.hpp>
#include "VboIndexer.hpp"

namespace Chimera {

//Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((void*)(i))

class Mesh : public Draw
{
public:
    friend class LoaderDae;

    Mesh (Node *_parent, std::string _name );
	Mesh(const Mesh &_cpy);

    virtual ~Mesh();
	virtual void init();

	// Inherited via Node
	void accept(class NodeVisitor* v) override;

	// Inherited via Draw
    virtual void render (const glm::mat4 &_projection, const glm::mat4 &_view, const glm::mat4 &_model) override;
    virtual glm::vec3 getSizeBox();

    void loadCollada ( tinyxml2::XMLElement* _nNode );

    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;

    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;

    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;

private:
    void setVertexBuffer();
    int getSource ( tinyxml2::XMLElement* _source, std::vector<float> &_arrayValores );

	GLuint VAO;
    GLuint VertexVBOID;
    GLuint IndexVBOID;

    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
