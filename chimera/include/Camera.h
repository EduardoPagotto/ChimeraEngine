#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <tinyxml2.h>

#include "Node.h"

#include "Shader.h"

namespace Chimera {

enum class CameraType
{
    Base,
    Spherical,
    Land,
    Air,
    Ovr
};

class Camera : public Node
{
public:
    Camera (Node* _pNode, CameraType _type, std::string _name );

    virtual ~Camera();

	virtual void init() {}

	glm::mat4 getViewMatrix(void);

	// Inherited via Node
	virtual void accept(class NodeVisitor* v) override;

    void setType ( CameraType _type ) {
        type = _type;
    }

    void setNear ( float _near ) {
        this->nearDistance = _near;
    }

    void setFar ( float _far ) {
        this->farDistance = _far;
    }

    void setFov ( float _fov ) {
        this->fov = _fov;
    }

    float getNear() const {
        return nearDistance;
    }

    float getFar() const {
        return farDistance;
    }

    float getFov() const {
        return fov;
    }

    bool isPerspective() {
        return perspective;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    void setPerspective ( bool _perspective ) {
        perspective = _perspective; //TODO implementar isto para ortogonal (false);
    }

	void setTransform(const glm::mat4 &_trans);
    void setPositionRotation ( const glm::vec3 &_posicao, const glm::vec3 &_rotation );

	void apply();

    void loadCollada ( tinyxml2::XMLElement* _nNode );

protected:
    CameraType type;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 direction;

    //glm::mat4 transform;

private:
    float nearDistance;
    float farDistance;
    float fov;
    bool perspective;

	Shader *shader;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
