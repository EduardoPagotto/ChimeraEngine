#ifndef NODE_H_
#define NODE_H_

#include "Entity.h"
#include "DataMsg.h"

namespace Chimera {

/**
 * Class Node
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Node : public Entity {
public:
    Node ( EntityType type, std::string name );
    ~Node();

    virtual void update ( DataMsg *dataMsg );

    void addChild ( Node *child );

protected:
    Node *parent;
    std::list<Node*> lChild;

private:

};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
