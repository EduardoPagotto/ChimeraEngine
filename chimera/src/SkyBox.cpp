#include "SkyBox.h"
#include <Object.h>


namespace Chimera {

SkyBox::SkyBox(std::string _id, std::string _name): Node(EntityKind::SKYBOX, _id, _name)
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::update(DataMsg *_dataMsg) {

	if (_dataMsg->getKindOp() == KindOp::START) {

	 //inicialize primeiro os filhos para garantir textura e efeito em material
	 Node::update(_dataMsg);

	 //inicializa objeto local
	 pDraw = (Draw*)findChildByKind(EntityKind::DRAW, 0);

   } else {

	   Node::update(_dataMsg);

   }

}

void SkyBox::render()
{
	if (pDraw != nullptr)
		pDraw->render();
}


}