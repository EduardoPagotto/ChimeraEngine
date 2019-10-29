#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include "Node.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/Shader.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <map>

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

    inline void setAmbient(const glm::vec4& _color) { ambient = _color; }
    inline void setSpecular(const glm::vec4& _color) { specular = _color; }
    inline void setDiffuse(const glm::vec4& _color) { diffuse = _color; }
    inline void setType(const LightType& _type) { type = _type; }

    inline void setTransform(const glm::mat4& _trans) { transform = _trans; }

    inline glm::vec3 getPosition() const { return glm::vec3(transform[3]); }
    inline void setPosition(const glm::vec3& _pos) { transform = glm::translate(transform, _pos); }

    inline void setRotation(const glm::vec3& _rotation) {
        transform = glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
    }

  private:
    int number;
    LightType type;
    glm::mat4 transform;
    glm::vec4 ambient;
    glm::vec4 specular;
    glm::vec4 diffuse;
};
} // namespace Chimera
#endif
