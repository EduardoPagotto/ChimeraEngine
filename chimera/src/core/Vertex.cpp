#include "chimera/core/Vertex.hpp"
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

#define BUFFER_OFFSET(i) ((void*)(i))

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2) { return fabs(v1 - v2) < 0.01f; }

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, unsigned int& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (unsigned int i = 0; i < out_vertex.size(); i++) {

        if (is_near(in_vertex.position.x, out_vertex[i].position.x) &&
            is_near(in_vertex.position.y, out_vertex[i].position.y) &&
            is_near(in_vertex.position.z, out_vertex[i].position.z) &&

            is_near(in_vertex.normal.x, out_vertex[i].normal.x) &&
            is_near(in_vertex.normal.y, out_vertex[i].normal.y) &&
            is_near(in_vertex.normal.z, out_vertex[i].normal.z) &&

            is_near(in_vertex.texture.x, out_vertex[i].texture.x) &&
            is_near(in_vertex.texture.y, out_vertex[i].texture.y) //&&

            //             is_near(in_vertex.color.x, out_vertex[i].color.x) &&
            //             is_near(in_vertex.color.y, out_vertex[i].color.y) &&
            //             is_near(in_vertex.color.z, out_vertex[i].color.z) &&
            //             is_near(in_vertex.color.w, out_vertex[i].color.w)
        ) {
            result = i;
            return true;
        }
    }
    return false;
}

void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
                   std::vector<unsigned int>& out_indices) {

    // percorrer todos os vertices
    for (unsigned int i = 0; i < inData.size(); i++) {

        // Procura por similar
        unsigned int index;
        if (getSimilarVertexIndex(inData[i], outData, index) == true) {

            // se entrotar usar apenas o indice
            out_indices.push_back(index);

        } else {

            // se nao adiciona a lista de novo vertex
            outData.push_back(inData[i]);
            out_indices.push_back((unsigned int)outData.size() - 1);
        }
    }
}

//---------- VertexRenderDynamic

VertexRenderDynamic::VertexRenderDynamic() {}

VertexRenderDynamic::~VertexRenderDynamic() {
    // FIXME: remover da memoria o VAO/VBO
}

void VertexRenderDynamic::render(std::vector<VertexDataFull>& vVertice) {

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int tot = vVertice.size() * sizeof(Chimera::VertexDataFull);
    // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tot, &vVertice[0]);

    glDrawArrays(GL_TRIANGLES, 0, vVertice.size()); //?? https://www.youtube.com/watch?v=S_xUgzFMIso

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexRenderDynamic::create(const unsigned int& max) {

    // refs: ver Particle.cpp in node sub-projetc
    // http://www.songho.ca/opengl/gl_vbo.html#create
    // https://www.khronos.org/opengl/wiki/Vertex_Rendering

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max, nullptr, GL_STREAM_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    GLuint positionID = 0;
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(positionID);

    // normal attribute
    GLuint normalID = 1;
    glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(normalID);

    // color attribute
    GLuint colorID = 2;
    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(colorID);

    // texture coord attribute
    GLuint uvID = 3;
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataFull), BUFFER_OFFSET(36));
    glEnableVertexAttribArray(uvID);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(positionID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(colorID);
    glDisableVertexAttribArray(uvID);
}

//---------- VertexRenderStatic

VertexRenderStatic::VertexRenderStatic() {
    VertexVBOID = 0;
    IndexVBOID = 0;
    VAO = 0;
}
VertexRenderStatic::~VertexRenderStatic() {
    glDeleteBuffers(1, &VertexVBOID);
    glDeleteBuffers(1, &IndexVBOID);
}

void VertexRenderStatic::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexIBO.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}

// void VertexRenderStatic::setVertexBuffer() {
void VertexRenderStatic::create(std::vector<VertexData>& vertexDataIn) {
    indexVBO_slow(vertexDataIn, vertexData, indexIBO);

    unsigned int sizeBufferVertex = vertexData.size() * sizeof(VertexData);
    unsigned int sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);

    // Buffer de vertice
    glGenBuffers(1, &VertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &vertexData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Buffer de indice
    glGenBuffers(1, &IndexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // cria o VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vincula VBO
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

    // Vertice
    GLuint positionID = 0; // glGetAttribLocation(shader.getIdProgram(), "position");
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(positionID);

    // Normal
    GLuint normalID = 1; // glGetAttribLocation(shader.getIdProgram(), "normal");
    glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(normalID);

    // Texture
    GLuint uvID = 2; // glGetAttribLocation(shader.getIdProgram(), "uv1");
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
    glEnableVertexAttribArray(uvID);

    // vincula IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

    // limpa dados
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(positionID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(uvID);
}

} // namespace Chimera