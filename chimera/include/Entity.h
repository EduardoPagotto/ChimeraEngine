#ifndef ENTITY_H_
#define ENTITY_H_

#include <cstdint>
#include <string>
#include <list>

namespace Chimera
{

/**
 * Enum EntityKind
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class EntityKind : int8_t
{
    NODE,
    TEXTURE,
    EFFECT,
    MATERIAL,
    CAMERA,
    LIGHT,
    OBJECT,
    PARTICLE_EMITTER,
    PHYSICS,
    DRAW,
    CONSTRAINT,
    SKYBOX
};

/**
 * Class Entity
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
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
    Entity ( const EntityKind &_kind ,const std::string &_id, const std::string &_name )
        : kind ( _kind ), id ( _id ), name ( _name ) ,serial ( ++serialMaster ), status ( true ) {}

    /**
     * Construtor de Copia
     * @param _entity entidade a ser copiada
     */
    Entity ( const Entity &_entity ) 
        : kind ( _entity.kind ), id(_entity.id + "_cp"), name(_entity.name + "_cp"), serial ( ++serialMaster ), status ( _entity.status ) {}

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

    inline EntityKind getKind() const {
        return kind;
    }

    void virtual setStatus ( bool _status ) {
        status = _status;
    }

    bool virtual getStatus() const {
        return status;
    }

    inline const std::string& getName() const {
        return name;
    }

    inline void setName ( const std::string& name ) {
        this->name = name;
    }

    inline const std::string& getId() const {
        return id;
    }

    inline void setId ( const std::string& name ) {
        this->id = name;
    }

    /**
     * Retorna uma string com o nome do tipo
     * @param _kind tipo
     * @return nome do tipo
     */
    static std::string getNameKindNode ( EntityKind _kind );

private:
    bool status;
    EntityKind kind;
    unsigned serial;
    static unsigned serialMaster;
    std::string name;
    std::string id;
};

//unsigned Entity::serialMaster = 0;

} /* namespace Chimera */
#endif /* BASE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
