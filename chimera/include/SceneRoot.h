#ifndef SCENE_ROOT_H_
#define SCENE_ROOT_H_

#include "Node.h"

#include "Object.h"

#include "State.h"

namespace Chimera {

class SceneRoot : public Node
{
public:
	SceneRoot(Node* _pNode, std::string _name);
	virtual ~SceneRoot();
    virtual void init();
    virtual void draw(Object *pObjeto);
	virtual void accept(class NodeVisitor * v) override;

	State *getState() {
		return pState;
	}

private:
	State *pState;
};

}

#endif
