#ifndef ENTITY_H_
#define ENTITY_H_

#include <cstdint>
#include <string>
#include <list>

namespace Chimera {

	/**
	 * Enum EntityKind
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	enum class EntityKind : int8_t {
		NODE,
		TEXTURE,
		EFFECT,
		MATERIAL,
		CAMERA,
		LIGHT,
		OBJECT,
		PHYSICS,
		DRAW,
		CONSTRAINT
	};

	/**
	 * Class Entity
	 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
	 *  @since 20130925
	 */
	class Entity {
	public:
		Entity(EntityKind _kind) : kind(_kind), serial(++serialMaster), status(true) {}

		Entity(const Entity &_entity) : kind(_entity.kind), serial(++serialMaster), status(_entity.status){}

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

		void virtual setStatus(bool _status) {
			status = _status;
		}

		bool virtual getStatus() const {
			return status;
		}

		static std::string getNameKindNode(EntityKind _kind);

	private:
		bool status;
		EntityKind kind;
		unsigned serial;
		static unsigned serialMaster;

	};

	//unsigned Entity::serialMaster = 0;

} /* namespace Chimera */
#endif /* BASE_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
