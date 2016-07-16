#ifndef SCENE_ROOT_H_
#define SCENE_ROOT_H_

#include "Node.h"
#include "State.h"
#include "Group.h"

namespace Chimera {

class SceneRoot : public Node
{
public:
	SceneRoot(Node* _pNode, std::string _name);
	virtual ~SceneRoot();
    virtual void init();
    virtual void draw(Group *_pGroup);
	virtual void accept(class NodeVisitor * v) override;

	State *getState() {
		return pState;
	}

private:
	State *pState;
};

}

#endif
