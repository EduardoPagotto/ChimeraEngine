#ifndef SCENE_ROOT_H_
#define SCENE_ROOT_H_

#include "Node.h"

namespace Chimera {

class SceneRoot : Node
{
public:
	SceneRoot(Node* _pNode, std::string _name);
	virtual ~SceneRoot();

	// Inherited via Node
	virtual void accept(class NodeVisitor * v) override;
};

}

#endif
