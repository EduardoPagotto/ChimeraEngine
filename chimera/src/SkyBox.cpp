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

	Node::update(_dataMsg);
	
}

}