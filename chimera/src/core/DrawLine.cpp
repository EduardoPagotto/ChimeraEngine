#include "chimera/core/DrawLine.hpp"

namespace Chimera {

// ref: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11517

DrawLine::DrawLine() {

    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";
    ShaderManager::load("DrawLine", shadeData, shader);
}

DrawLine::~DrawLine() {}

void DrawLine::init() {

    BufferLayout b;
    b.Push<float>(3, false);
    b.Push<float>(3, false);

    vbo = new VertexBuffer(BufferType::STATIC);
    vao.bind();
    vbo->bind();

    vao.push(vbo);

    vbo->setLayout(b);
    vbo->setData(&points[0], points.size());
    vbo->unbind();
}

void DrawLine::render(const glm::mat4& projection, const glm::mat4& view) {
    glUseProgram(shader.getID());

    shader.setUniformU("projection", Chimera::UValue(projection)); // renderer.getCamera()->getProjection()
    shader.setUniformU("view", Chimera::UValue(view));             // vpo->getView()

    vao.bind();
    glDrawArrays(GL_LINES, 0, points.size() * 2); // 2 ??
    vao.unbind();
    glUseProgram(0);
}

void DrawLine::addAABB(const AABB& aabb, const glm::vec3& color) {

    const glm::vec3* v = aabb.getAllVertex();

    add(v[0], v[1], color);
    add(v[2], v[3], color);
    add(v[4], v[5], color);
    add(v[6], v[7], color);

    add(v[0], v[2], color);
    add(v[1], v[3], color);
    add(v[4], v[6], color);
    add(v[5], v[7], color);

    add(v[0], v[4], color);
    add(v[1], v[5], color);
    add(v[2], v[6], color);
    add(v[3], v[7], color);
}

} // namespace Chimera
