#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include "glm/glm.hpp"
#include <map>

#include "Node.hpp"
#include "chimera/core/Color.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/Shader.hpp"

namespace Chimera {

enum class LightType {
    DIRECTIONAL = 0,
    POSITIONAL = 1,
};

class Light : public Node {
  public:
    Light(Node* _parent, std::string _name);
    virtual ~Light();
    virtual void init();

    // Inherited via Node
    virtual void accept(class NodeVisitor* v) override;

    void apply(Shader* _pShader);

    inline void setAmbient(const Color& _color) { ambient = _color; }
    inline void setSpecular(const Color& _color) { specular = _color; }
    inline void setDiffuse(const Color& _color) { diffuse = _color; }
    inline void setType(const LightType& _type) { type = _type; }
    inline void setTransform(const glm::mat4& _trans) { transform = _trans; }

    glm::vec3 getPosition() const { return position; }

    void setPositionRotation(const glm::vec3& _posicao, const glm::vec3& _rotation);

  private:
    glm::mat4 transform;
    glm::vec3 position;

    Color ambient;
    Color specular;
    Color diffuse;

    int number;
    LightType type;
};
} // namespace Chimera
#endif
