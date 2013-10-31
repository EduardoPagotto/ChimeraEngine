#ifndef LOADER_H_
#define LOADER_H_

#include <string.h>
#include <stdlib.h>

#include <string>

#include <queue>

//#include <libxml/xmlmemory.h>
//#include <libxml/parser.h>

#include <tinyxml2.h>

#include <log4cxx/logger.h>

#include "ExceptionChimera.h"

//#include "SceneMng.h"
//#include "PhysicsControl.h"
// #include "DrawTriMesh.h"
// #include "DrawBox.h"
// #include "DrawGrid.h"
// #include "Material.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Effect.h"
#include "Texture.h"
#include "Material.h"
#include "DrawTriMesh.h"
#include "Object.h"

namespace Chimera {

class Loader {

public:
    Loader ();
    ~Loader();
    Node* loadDAE ( const std::string &_file );

    void setModelDir ( const std::string &_dir ) {
        m_modelDir = _dir;
    }

    void setImageDir ( const std::string &_dir ) {
        m_imageDir = _dir;
    }

private:

    std::string m_modelDir;
    std::string m_imageDir;

    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

     void carregaMatrix(btTransform *_pTrans, const std::vector<float> &listaMatrix);
//     std::string getAtributoXML (const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode );
//     std::string getValueFromProp(const std::string &tipoNomeNode, const std::string &chave, xmlNodePtr _xmlNode );
//
//     static xmlNodePtr findNode ( const char* _nomeNode, xmlNodePtr _nodePos );
//
//     Color getPhong ( xmlNodePtr _nPos,const char* _name );
//
//     int getSource ( xmlNodePtr _nSource, ListPtr<float> &_arrayValores );
//
    void createNode (tinyxml2::XMLElement* _nNodeXML, Node *_pNode );
//
    std::string retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node );

    
    void libCam ();
    void libLight ();
    void libEffect ();
    void libTexture ();
    void libMaterial ();
    void libGeometry ();
    Node* libScene ();
    void libPhysicsMaterial();
    void libPhysicsModels ();
    void libPhysicsScenes ();
//
//     xmlDocPtr m_doc;
//     xmlNodePtr m_root;
//
//     unsigned m_numNodes;
//

//
    std::queue<Node*> listaNodeRemover;
//
     std::map<std::string, btMaterial*> m_pPhMaterial;

    log4cxx::LoggerPtr logger;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
