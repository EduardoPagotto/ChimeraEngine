#ifndef LOADER_H_
#define LOADER_H_

#include <string.h>
#include <stdlib.h>
#include <string>
#include <queue>

#include <tinyxml2.h>

//#include <log4cxx/logger.h>

#include "ExceptionChimera.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Effect.h"
#include "Texture.h"
#include "Material.h"
#include "DrawTriMesh.h"
#include "Object.h"
#include "Constraint.h"

namespace Chimera
{

class Loader
{

public:
    Loader();
    ~Loader();
    Node* loadDAE ( const std::string &_file );

    void setModelDir ( const std::string &_dir ) {
        m_modelDir = _dir;
    }

    void setImageDir ( const std::string &_dir ) {
        m_imageDir = _dir;
    }

private:
    void carregaMatrix ( btTransform *_pTrans, const std::vector<float> &listaMatrix );
    void createNode ( tinyxml2::XMLElement* _nNodeXML, Node *_pNode );
    std::string retornaAtributo ( const std::string &_atributo, tinyxml2::XMLElement* _node );

    void libCam();
    void libLight();
    void libEffect();
    void libTexture();
    void libMaterial();
    void libGeometry();
    Node* libScene();
    void libPhysicsMaterial();
    void libPhysicsModels();
    void libPhysicsScenes();

    void libConstraint();

    std::string m_modelDir;
    std::string m_imageDir;

    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

    std::queue<Node*> listaNodeRemover;
    std::map<std::string, btMaterial*> m_pPhMaterial;

    //log4cxx::LoggerPtr logger;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
