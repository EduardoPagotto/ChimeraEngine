#ifndef NODE_H_
#define NODE_H_

#include <list>

#include "Entity.h"
#include "DataMsg.h"

namespace Chimera {
namespace Graph {
    
/**
 * Class Node
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Node : public Entity
{
public:

    //friend class Chimera::SceneMng;
    //friend class Chimera::Loader;

    Node ( EntityKind _type, std::string _name );

    Node ( const Node &_node );

    ~Node();

    virtual void clone ( Node **ppNode );

    virtual void update ( DataMsg *dataMsg );

    void addChild ( Node *child );

    Node *findChildByKind ( EntityKind type, int index );

    //static Node *findNodeById ( EntityKind type, std::string id );
    //static Node *findNodeById ( std::string id );
    //static Node *findNodeByName ( EntityKind type, std::string name );
    //static Node *findNodeByName ( std::string name );

	//FIXME mudar para protected
	std::list<Node*> listChild;

protected:
    Node *parent;
    


private:
    static std::list<Node*> listNode;
};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
