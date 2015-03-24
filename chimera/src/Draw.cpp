#include "Draw.h"

namespace Chimera {

	Draw::Draw(DrawType _type, std::string _id, std::string _name) : Node(EntityKind::DRAW, _id, _name), type(_type) {
		pMaterial = nullptr;
	}

	Draw::Draw(const Draw &_draw) : Node(_draw), type(_draw.type) {
		pMaterial = _draw.pMaterial;
	}

	Draw::~Draw() {

	}

	void Draw::init() {

		pMaterial = (Material*)findChildByKind(EntityKind::MATERIAL, 0);
		if (pMaterial == nullptr) {
			pMaterial = new Material(getId() + "_Material_defaultCreate", getName() + "_Material_default_create");
			pMaterial->createDefaultEffect();
		}

	}

	void Draw::render() {

		pMaterial->begin();
		renderExecute();
		pMaterial->end();

	}

	void Draw::update(DataMsg *dataMsg) {

		if (dataMsg->getKindOp() == KindOp::START) {

		//inicialize primeiro os filhos para garantir textura e efeito em material
		Node::update(dataMsg);

		//inicializa Mesh local
		init();

	} else if (dataMsg->getKindOp() == KindOp::DRAW3D) {

		//             glPushMatrix();
		//
		//             if (pPhysic != nullptr) {
		//                 Object *pSource = (Object *)_dataMsg->getParam();
		//                 pPhysic->ajusteMatrix(pSource->pPhysic);
		//             }
		//
		//             render();
		//
		//             Node::update(_dataMsg);
		//
		//             glPopMatrix();
		}
		else
		Node::update(dataMsg);

	}

}
