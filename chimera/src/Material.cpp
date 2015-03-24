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

		if (dataMsg->getKindOp() == KindOp::START) {

			pEffect = (Effect*)findChildByKind(EntityKind::EFFECT, 0);
			if (pEffect == nullptr) {
				createDefaultEffect();
			}

			pTextura = (Texture*)findChildByKind(EntityKind::TEXTURE, 0);

			Node::update(dataMsg);

		}
	}

    void Material::end()
    {
        if (pTextura != nullptr) {
            pTextura->end();
        }
    }

	void Material::createDefaultEffect() {

		pEffect = new Effect("effect_interno", "effect_interno");
		pEffect->setDiffuse(Color(0.6f, 0.6f, 0.6f));
		pEffect->setEmissive(Color(0.1f, 0.1f, 0.1f));
		pEffect->setAmbient(Color(0.1f, 0.1f, 0.1f));
		pEffect->setSpecular(Color(0.5f, 0.5f, 0.5f));
		pEffect->setShininess(0.5);

		//Node::addChild(pEffect);//preciso ??
	}

	void Material::begin() {

		pEffect->apply();

		if (pTextura != nullptr) {
			pTextura->begin();
		}
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
