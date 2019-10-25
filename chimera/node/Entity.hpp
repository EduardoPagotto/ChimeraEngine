// #ifndef __CHIMERA_ENTITY__HPP
// #define __CHIMERA_ENTITY__HPP

// #include <cstdint>
// #include <string>

// namespace Chimera {

// /**
//  * @brief Enum EntityKind
//  *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
//  *  @since 20130925
//  *  @version %I%, %G%
//  */
// enum class EntityKind : int8_t { GROUP, CAMERA, LIGHT, SKYBOX, SHADOWMAP, HUD, MESH, PARTICLE_SYSTEM };

// /**
//  * @brief Class Entity
//  *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
//  *  @since 20130925
//  *  @version %I%, %G%
//  */
// class Entity {
//   public:
//     /**
//      * Construtor principal Entity
//      * @param _kind Tipo
//      * @param _id identificador
//      * @param _name nome
//      */
//     Entity(const EntityKind& _kind, const std::string& _name) : kind(_kind), name(_name), serial(++serialMaster) {}

//     /**
//      * Construtor de Copia
//      * @param _entity entidade a ser copiada
//      */
//     Entity(const Entity& _entity) : kind(_entity.kind), name(_entity.name + "_cp"), serial(++serialMaster) {}

//     /**
//      * Destrutor
//      */
//     virtual ~Entity() {}

//   private:
//     NodeKind kind;
//     unsigned serial;
//     static unsigned serialMaster;
//     std::string name;
// };

// // unsigned Entity::serialMaster = 0;

// } // namespace Chimera
// #endif
