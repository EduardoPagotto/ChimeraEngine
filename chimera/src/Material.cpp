#include "Material.h"

namespace Chimera {

	Material::Material(std::string _id, std::string _name) : Node(EntityKind::MATERIAL, _id, _name) {
		pEffect = nullptr;
		pTextura = nullptr;
	}

	Material::Material(const Material &_cpy) : Node(_cpy) {
	}

	Material::~Material() {
	}

	void Material::clone(Node **ppNode) {
		*ppNode = new Material(*this);
		Node::clone(ppNode);
	}

	void Material::update(DataMsg *dataMsg) {
		Node::update(dataMsg);
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
