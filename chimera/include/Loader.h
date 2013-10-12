#ifndef LOADER_H_
#define LOADER_H_

#include <string.h>
#include <stdlib.h>

#include <string>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "ExceptionChimera.h"

#include "SceneMng.h"
#include "PhysicWorld.h"
#include "Mesh.h"

namespace Chimera {

class Loader {
public:
    Loader ();
    ~Loader();
    bool exec ( const char *_file );
    
    SceneMng *getSceneMng() {
        return m_pScene;
    }
    
    void setModelDir(const std::string &_dir) {
        m_modelDir = _dir;
    }
    
    void setImageDir(const std::string &_dir) {
        m_imageDir = _dir;
    }
    
private:

    void createScene();
    
    std::string getValProp (const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode );
    std::string getAttribute(const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode );
    
    xmlNodePtr findNode ( const char* _nomeNode, xmlNodePtr _nodePos );
    
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

    SceneMng *m_pScene;
    PhysicWorld *m_physicWorld;

    xmlDocPtr m_doc;
    xmlNodePtr m_root;

    unsigned m_numNodes;
    
    std::string m_modelDir;
    std::string m_imageDir;
    
    log4cxx::LoggerPtr logger;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
