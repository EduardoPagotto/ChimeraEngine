#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <list>

#include "Entity.h"
#include "DataMsg.h"

namespace Chimera {
    
/**
 * Class Node
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Node : public Entity
{
public:

    Node (Node *_parent, EntityKind _type, std::string _name );

    virtual ~Node();

    virtual void accept(class NodeVisitor* v) = 0;

    void addChild ( Node *_child );

	void setParent(Node *_node);

	Node *getParent() {
		return parent;
	}

	std::vector<Node*>* getChilds();

	void removeChild(Node* _child);

	const size_t countChilds(const bool &_recursiveCount) const;

	Node* getChildByName(const std::string &_searchName);

    Node *findChildByKind ( EntityKind type, int index );

	void setColor(int col) {
		this->color = &col;
	}

	int getColor() {
		return *(this->color);
	}

	static Node *findNodeByKind(EntityKind _type, int _index);
	static Node *findNodeByName(EntityKind _type, std::string _name);

    //static Node *findNodeById ( EntityKind type, std::string id );
    //static Node *findNodeById ( std::string id );
    //static Node *findNodeByName ( EntityKind type, std::string name );
    //static Node *findNodeByName ( std::string name );

private:
	Node *parent;
	std::vector<Node*> vChild;

	int* color;

    static std::list<Node*> listNode;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
