#ifndef SCENE_ROOT_H_
#define SCENE_ROOT_H_

#include "Node.h"

#include "Object.h"

namespace Chimera {

class SceneRoot : public Node
{
public:
	SceneRoot(Node* _pNode, std::string _name);
	virtual ~SceneRoot();

    virtual void initScene();
    virtual void draw(Object *pObjeto);
    
	// Inherited via Node
	virtual void accept(class NodeVisitor * v) override;
};

}

#endif
