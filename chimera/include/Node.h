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
    Node ( EntityType _type);
    
    Node (const Node &_node);
    
    ~Node();

    virtual void update ( DataMsg *dataMsg );

    void addChild ( Node *child );

    inline const std::string& getName() const {
        return name;
    }
    
    inline void setName ( const std::string& name ) {
        this->name = name;
    }

    inline const std::string& getId() const {
        return id;
    }

    inline void setId ( const std::string& name ) {
        this->id = name;
    }

    static Node *findObjById ( EntityType type,std::string id );
    static Node *findObjById ( std::string id );
    static Node *findObjByName ( EntityType type,std::string name );
    static Node *findObjByName ( std::string name );

protected:
    Node *parent;
    std::list<Node*> listChild;

private:
    std::string name;
    std::string id;
    static std::list<Node*> listNode;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
