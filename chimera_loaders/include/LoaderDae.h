#ifndef _LOADER_DAE__H
#define _LOADER_DAE__H

#include <tinyxml2.h>
#include <string>
#include <tuple>

#include "PhysicsControl.h"
#include "Material.h"
#include "Group.h"
#include "Solid.h"
#include "Draw.h"
#include "Light.h"
#include "Mesh.h"
#include "Camera.h"
#include "CameraSpherical.h"

#include "TextureManager.h"

#include "PhysicMaterial.h"

#include <spdlog/spdlog.h>

namespace Chimera
{

class LoaderDae
{
public:
    LoaderDae (const std::string &_file);
    virtual ~LoaderDae();
	
    Group* getNodes() {
        return pRootNode;
    }
    
private:
    
    void getNodeSceneInFile();
    void getPhysicSceneInfile();
    void getDadosInstancePhysicModel ( tinyxml2::XMLElement* _nPhysicScene );
    
    void carregaNode ( Node *_pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type );

    static Camera *carregaCamera(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nNode, const char* l_url, const char* _id, const char* _name);
    static int libGeometryMap(tinyxml2::XMLElement* _root, std::map<std::string, Draw*> &mapaGeometria);
    static int libTextureMap(tinyxml2::XMLElement* _root, TextureManager *_texManager);
    static tinyxml2::XMLElement* getDadoRigidBody(tinyxml2::XMLElement* _root, const char* _url, const char* _sid );
    static tinyxml2::XMLElement* findSceneLib (tinyxml2::XMLElement* pRoot, const char *rotina, const char* instance, const char* library );


    static std::tuple<Color, LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);

    static void loadMaterial(tinyxml2::XMLElement* root, tinyxml2::XMLElement* _nNode, Material *_pMat);
    static void loadMaterialProfile(tinyxml2::XMLElement* _nNode, Material *_pMat);
    static bool getPhong (const char* _tipoCor, Color &_color, tinyxml2::XMLElement* _nNode);

    static void loadColladaPhysicsModel(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nRigidBody, std::string &_meshName, Solid *_pPhysic);
    static void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, std::string &_meshName, Solid *_pPhysic);

    static void loadPhysicControlCollada ( tinyxml2::XMLElement* _nNode, PhysicsControl *_pPhysicsControl);


    static int getSource ( tinyxml2::XMLElement* _source, std::vector<float> &_arrayValores );
    static void loadMeshCollada (tinyxml2::XMLElement* _nNode, Mesh *_pDraw);

    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;

    std::map<std::string, Solid*> mapaEntidadeFisica;
    std::map<std::string, Draw*> mapaGeometria;
    
    std::map<std::string, PhysicMaterial*> mapaPhysicMaterial;

    Group *pRootNode;
    
    PhysicsControl *pPhysicsControl;
	TextureManager *texManager;
    std::shared_ptr<spdlog::logger> log;
};
}


#endif
