#ifndef ENTITY_H_
#define ENTITY_H_

#include <cstdint>
#include <string>

namespace Chimera
{

/**
 * Enum EntityKind
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @version %I%, %G%
 */
enum class EntityKind : int8_t
{
    GROUP,
    SOLID,
    TRANSFORM,
    TEXTURE,
    MATERIAL,
    CAMERA,
    LIGHT,
    SKYBOX,
	SCENEROOT,
	CONSTRAINT,
    HUD,
    MESH,
    PARTICLE_SYSTEM,
};

/**
 * Class Entity
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @version %I%, %G%
 */
class Entity
{
public:
    /**
     * Construtor principal Entity
     * @param _kind Tipo
     * @param _id identificador 
     * @param _name nome 
     */
    Entity ( const EntityKind &_kind ,/*const std::string &_id,*/ const std::string &_name )
        : kind ( _kind ), /*id ( _id ),*/ name ( _name ) ,serial ( ++serialMaster ) {}

    /**
     * Construtor de Copia
     * @param _entity entidade a ser copiada
     */
    Entity ( const Entity &_entity ) 
        : kind ( _entity.kind ),/* id(_entity.id + "_cp"),*/ name(_entity.name + "_cp"), serial ( ++serialMaster ) {}

    /**
     * Destrutor 
     */
    virtual ~Entity() {}

    inline unsigned getSerial() const {
        return serial;
    }

    static unsigned getSerialMaster() {
        return ++Entity::serialMaster;
    }

	inline void setKind(const EntityKind &_kind) {
		kind = _kind;
	}

    inline EntityKind getKind() const {
        return kind;
    }

    inline const std::string& getName() const {
        return name;
    }

    inline void setName ( const std::string& name ) {
        this->name = name;
    }

private:
    EntityKind kind;
    unsigned serial;
    static unsigned serialMaster;
    std::string name;
};

//unsigned Entity::serialMaster = 0;

} /* namespace Chimera */
#endif /* BASE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
