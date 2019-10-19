#ifndef __CHIMERA_COORD__HPP
#define __CHIMERA_COORD__HPP

#include "Node.hpp"
#include "glm/glm.hpp"

namespace Chimera {

/**
 * @brief Clase de coordenada puramente Virtual
 *
 */
class Coord : public Node {
  public:
    /**
     * @brief Construct a new Coord object
     * @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
     *
     * @param _parent : Pai ou nullptr se nao o tiver
     * @param _entity : tipo de entidade
     * @param _name : Nome da coordenada
     */
    Coord(Node* _parent, EntityKind _entity, std::string _name) : Node(_parent, _entity, _name) {}

    /**
     * @brief Destroy the Coord object
     *
     */
    virtual ~Coord() {}

    /**
     * @brief Get the Position object
     *
     * @return glm::vec3
     */
    virtual glm::vec3 getPosition() = 0;

    /**
     * @brief Set the Position object
     *
     * @param _pos
     */
    virtual void setPosition(const glm::vec3& _pos) = 0;

    /**
     * @brief Get the Rotation object
     *
     * @return glm::vec3
     */
    virtual glm::vec3 getRotation() = 0;

    /**
     * @brief Set the Rotation object
     *
     * @param _rot
     */
    virtual void setRotation(const glm::vec3& _rot) = 0;

    /**
     * @brief Get the Matrix object
     *
     * @return glm::mat4
     */
    virtual glm::mat4 getMatrix() = 0;

    /**
     * @brief Set the Matrix object
     *
     * @param _trans
     */
    virtual void setMatrix(const glm::mat4& _trans) = 0;
};
} // namespace Chimera

#endif