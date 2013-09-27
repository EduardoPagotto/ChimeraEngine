#ifndef LOADER_H_
#define LOADER_H_

#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "Scene.h"
#include "PhysicWorld.h"

namespace Chimera {

class Loader {
public:
    Loader ( Scene *_pScene );
    ~Loader();
    bool exec ( const char *_file );
private:

    xmlNodePtr findNode ( const char* _nomeNode, xmlNodePtr _nodePos );
    void setIdentity ( Node *_pNode, xmlNodePtr _xmlNode );

    void loadArrayF ( const char *_val, std::vector<float> &_arrayF );
    void loadArrayI ( const char *_val, std::vector<int> &_arrayI );

    Color getPhong ( xmlNodePtr _nPos,const char* _name );

    int getSource ( xmlNodePtr _nSource, ListPtr<float> &_arrayValores );

    void createNode ( xmlNodePtr _nodeXML, Node *_pNode );

    int libCam ( void );
    int libLight ( void );
    int libEffect ( void );
    int libMaterial ( void );

    int libGeometry ( void );
    int libImage ( void );

    int libScene ( void );

    int libPhysicsModels ( void );
    int libPhysicsScenes ( void );
    int libPhysicsMaterial ( void );

    Scene *m_pScene;
    PhysicWorld *m_physicWorld;

    xmlDocPtr m_doc;
    xmlNodePtr m_root;

    unsigned m_numNodes;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
