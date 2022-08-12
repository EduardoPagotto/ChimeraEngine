#include "chimera/render/3d/Renderer3dLines.hpp"
#include "chimera/core/space/AABB.hpp"

namespace Chimera {

Renderer3dLines::Renderer3dLines() {

    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";
    ShaderManager::load("DrawLine", shadeData, shader);
}

Renderer3dLines::~Renderer3dLines() {}

void Renderer3dLines::create(const uint32_t& sizeBuffer) {

    pVao = new VertexArray();
    pVbo = new VertexBuffer(BufferType::STREAM);

    pVao->push(pVbo);

    pVao->bind();
    pVbo->bind();

    BufferLayout layout;
    layout.Push<float>(3, false); // point
    layout.Push<float>(3, false); // color

    pVbo->setLayout(layout); // FIXME: no debug verificar tamanho no setData final!!! (stride interfere!!!)
    pVbo->setData(nullptr, sizeBuffer);
}

void Renderer3dLines::destroy() {
    if (pVao != nullptr) {
        delete pVao;
        pVao = nullptr;
    }
}

void Renderer3dLines::addAABB(const AABB& aabb, const glm::vec3& color) {

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

void Renderer3dLines::begin(Camera* camera, ViewProjection* vpo, Octree* octree) {

    this->camera = camera;
    this->vpo = vpo;
    // this->octree = octree;
    frustum.set(vpo->getViewProjectionInverse());
    // debug data
    // totIBO = 0;
    // totFaces = 0;
}

void Renderer3dLines::end() {}

bool Renderer3dLines::submit(const RenderCommand& command, IRenderable3d* renderable) {
    // Transformation model matrix AABB to know if in frustrum Camera
    Renderable3D* r = (Renderable3D*)renderable;
    const AABB& aabb = r->getAABB();
    AABB nova = aabb.transformation(command.transform);

    // adicione apenas o que esta no clip-space
    if (nova.visible(frustum) == true) {
        this->command = command;
        this->addAABB(nova, glm::vec3(1.0, 1.0, 1.0)); // FIXME: ver se e esta cor mesmo
        return true;
    }

    return false;
}

void Renderer3dLines::flush() {

    Shader activeShader;
    pVao->bind();
    pVbo->bind();

    activeShader = command.shader;
    glUseProgram(activeShader.getID());

    // generic bind in each draw call camera, light, etc
    for (const auto& kv : uniformsQueue)
        activeShader.setUniformU(kv.first.c_str(), kv.second);

    // bind dos uniforms from model
    for (const auto& kv : command.uniforms)
        activeShader.setUniformU(kv.first.c_str(), kv.second);

    pVbo->setSubData(&points[0], 0, points.size()); // TODO: conferir todos os tamanho, esta tudo muito solto ainda!!!
    glDrawArrays(GL_LINES, 0, points.size());       // /2 ?? *2

    pVbo->unbind();
    pVao->unbind();

    // Limpa buffer de uniforms ao terminar todos os draws calls
    uniformsQueue.clear();
    points.clear();
}
} // namespace Chimera