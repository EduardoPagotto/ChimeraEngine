#include "chimera/core/gl/DrawLine.hpp"

namespace ce {

// ref: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11517

void DrawLine::create(std::shared_ptr<Shader> shader, const uint32_t& sizeBuffer) noexcept {

    this->shader = shader;
    pVao = new VertexArray();
    pVbo = new VertexBuffer(BufferType::STREAM);

    pVao->push(pVbo);

    pVao->bind();
    pVbo->bind();

    BufferLayout layout;
    layout.Push<float>(3, false); // point
    layout.Push<float>(3, false); // color

    pVbo->setLayout(layout);
    pVbo->setData(nullptr, sizeBuffer);
}

void DrawLine::destroy() noexcept {
    delete pVao;
    pVao = nullptr;
    shader = nullptr;
    points.clear();
}

void DrawLine::render(MapUniform& uniformsQueue) noexcept {
    glUseProgram(shader->getID());

    for (const auto& kv : uniformsQueue)
        shader->setUniformU(kv.first.c_str(), kv.second);

    pVao->bind();
    pVbo->bind();

    pVbo->setSubData(&points[0], 0, points.size()); // load tata dynamic

    glDrawArrays(GL_LINES, 0, points.size());

    pVbo->unbind();
    pVao->unbind();

    glUseProgram(0);

    points.clear();
}

void DrawLine::addAABB(const AABB& aabb, const glm::vec3& color) noexcept {
    const std::vector<glm::vec3>& v = aabb.getAllVertex();
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
} // namespace ce
