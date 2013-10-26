#ifndef NODE_H_
#define NODE_H_

#include <log4cxx/logger.h>

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
    
    friend class SceneMng;
    friend class Loader;
    
    Node (EntityKind _type, std::string _id, std::string _name);
    
    Node (const Node &_node);
    
    ~Node();

    virtual void clone ( Node **ppNode );
    
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

    static Node *findNodeById ( EntityKind type,std::string id );
    static Node *findNodeById ( std::string id );
    static Node *findNodeByName ( EntityKind type,std::string name );
    static Node *findNodeByName ( std::string name );
    
    
protected:
    Node *parent;
    std::list<Node*> listChild;

private:
    std::string name;
    std::string id;
    static std::list<Node*> listNode;
    
    log4cxx::LoggerPtr logger;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
