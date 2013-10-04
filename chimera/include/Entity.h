#ifndef ENTITY_H_
#define ENTITY_H_

#include <cstdint>
#include <string>
#include <list>

namespace Chimera {

/**
 * Enum EntityType
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
enum class EntityType : int8_t {
    BASE=0,
    ANY,
    NODE,
    TRANSFORM,
    CAMERA,
    LIGHT,
    OBJECT,
    EFFECT,
    ENGINE
};

/**
 * Class Entity
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Entity {
public:
    Entity ( EntityType type ) : type ( type ) , serial ( ++serialMaster ), mode(0), status(true), maxMode(2) {}
    Entity ( const Entity &entity ) : type ( entity.type ), serial ( ++serialMaster ), mode(entity.mode), status(entity.status), maxMode(entity.maxMode) {}

    virtual ~Entity() {}
    
    inline unsigned getSerial() const {
        return serial;
    }

    static unsigned getSerialMaster() {
        return ++Entity::serialMaster;
    }

    inline EntityType getType() const {
        return type;
    }

    void virtual setStatus(bool _status) { 
        status = _status; 
    }

    bool virtual getStatus() const { 
        return status; 
    }
    
    void virtual setMode(int _valor) {
        if (_valor<=maxMode) 
            mode = _valor;
        else
            mode = maxMode;
    }
    
    int virtual getMode() { 
        return mode; 
    }    

private:
    bool status;
    int mode;
    int maxMode;
    EntityType type;
    unsigned serial;
    static unsigned serialMaster;

};

//unsigned Entity::serialMaster = 0;

} /* namespace Chimera */
#endif /* BASE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
