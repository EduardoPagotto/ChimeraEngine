#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include <tinyxml2.h>

#include "Node.hpp"
#include "chimera/core/Color.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "glm/glm.hpp"
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

    Color getAmbient() { return ambient; }

    Color getSpecular() { return specular; }

    Color getDiffuse() { return diffuse; }

    void setAmbient(Color _color) {
        ambient = _color;
        map_params[ModeMaterial::AMBIENT] = &ambient;
    }

    void setSpecular(Color _color) {
        specular = _color;
        map_params[ModeMaterial::SPECULAR] = &specular;
    }

    void setDiffuse(Color _color) {
        diffuse = _color;
        map_params[ModeMaterial::DIFFUSE] = &diffuse;
    }

    void setType(LightType _type) { type = _type; }

    void setTransform(const glm::mat4& _trans) { transform = _trans; }

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

    std::map<ModeMaterial, void*> map_params;
};
} // namespace Chimera
#endif
