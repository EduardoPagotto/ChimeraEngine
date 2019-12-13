#include "Game.hpp"
//#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/render/LoadHeightMap.hpp"
#include "chimera/render/LoadObj.hpp"

#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

Chimera::CanvasGL* video;
Chimera::NodeGroup* group1;
Chimera::NodeCamera* pCam;
Chimera::NodeLight* pLight;

std::map<std::string, Chimera::Shader*> mapa;

void createScreemAndLoadShaders() {

    using namespace Chimera;

    std::string config_file = "./samples/stereoscopic/appteste.yaml";
    SDL_Log("Carregar arquivo: %s", config_file.c_str());

    YAML::Node config = YAML::LoadFile(config_file);
    YAML::Node screen = config["screen"];
    YAML::Node canvas = screen["canvas"];

    // Controlador de video
    video = new CanvasHmd(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());
    // video = new CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

    // Carga dos shaders
    YAML::Node shaders = config["shaders"];

    // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
    SDL_Log("Shaders identificados: %d", (int)shaders.size());
    for (std::size_t i = 0; i < shaders.size(); i++) {
        YAML::Node shader_item = shaders[i];

        std::string nome = shader_item["name"].as<std::string>();
        std::string vertex = shader_item["vertex"].as<std::string>();
        std::string fragm = shader_item["fragment"].as<std::string>();
        Chimera::Shader* pShader =
            new Chimera::Shader(nome.c_str(), Chimera::shadeLoadProg(nome.c_str(), vertex.c_str(), fragm.c_str()));

        mapa[pShader->getCurrentProgram()] = pShader;
    }
}

void loadCameraData() {
    pCam = new Chimera::NodeCamera(group1, "Observador-01");
    pCam->createTrackBall();
    pCam->createTrackHead();
    pCam->getTrackBall()->setMax(1000.0);
    pCam->getTrackBall()->setMin(0.5);
    pCam->getViewPoint()->far = 10000.0;
    pCam->getViewPoint()->near = 0.1;
    pCam->getViewPoint()->fov = 45.0;
    pCam->getViewPoint()->position = glm::vec3(0, 0, 300);
    pCam->getViewPoint()->front = glm::vec3(0, 0, 0);
    pCam->getViewPoint()->up = glm::vec3(0, 1, 0);
    pCam->init();
}

void loadLightData() {
    pLight = new Chimera::NodeLight(group1, "Luz1");
    pLight->data.setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    pLight->data.setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    pLight->data.setPosition(glm::vec3(80, 100, 150));
}

void createMeshTexturizade(std::string name, std::string file, float scale, glm::vec3 _position) {

    using namespace Chimera;

    Material* pMap = new Material();
    NodeMesh* pMesh = new NodeMesh(group1, name);

    LoaderObj loader(file);
    loader.getMesh(pMesh->meshData);
    loader.getMaterial(*pMap);

    pMesh->meshData.changeSize(scale, pMap->hasTexture());
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(pMap);
}

void createMesh(std::string name, std::string file, float scale, glm::vec3 _position, Chimera::Material* _pMap) {

    using namespace Chimera;

    NodeMesh* pMesh = new NodeMesh(group1, name);
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(_pMap);

    LoaderObj loader(file);
    loader.getMesh(pMesh->meshData);

    pMesh->meshData.changeSize(scale, _pMap->hasTexture());
}

void createTerrain(std::string name, std::string file, float scale, glm::vec3 _position, Chimera::Material* _pMap) {

    using namespace Chimera;

    NodeMesh* pMesh = new NodeMesh(group1, name);
    pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), _position)));
    pMesh->setMaterial(_pMap);

    LoadHeightMap height((char*)file.c_str());
    height.getMesh2(pMesh->meshData);

    pMesh->meshData.changeSize(scale, _pMap->hasTexture());
}

int main(int argn, char** argv) {

    using namespace Chimera;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("appTest Iniciado");

    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %d: %s", i, argv[i]);
    }

    try {
        createScreemAndLoadShaders();

        group1 = new NodeGroup(nullptr, "modelos");
        group1->setShader(mapa["mesh-default"]);

        // Propriedades da camera
        loadCameraData();

        // Propriedades da luz
        loadLightData();

        createMeshTexturizade("Cubo-01", "./data/models/cubo_textura_simples.obj", 5.0, glm::vec3(0.0, 0.0, 0.0));
        createMeshTexturizade("Cubo-02", "./data/models/cubo2.obj", 10.0, glm::vec3(150.0, 0.0, 0.0));

        // Material Cubo sem textura
        Material* pMat = new Material();
        pMat->setAmbient(glm::vec4(0.5f, 0.5f, 0.31f, 1.0f));
        pMat->setDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        pMat->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        pMat->setShine(32.0f);
        pMat->addTexture(new TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid1.png"));

        createMesh("Teste1", "./data/models/tela01.obj", 20.0, glm::vec3(0.0, 0.0, 150.0), pMat);
        createMesh("Teste2", "./data/models/tela01.obj", 20.0, glm::vec3(0.0, 0.0, -150.0), pMat);

        Material* pMat1 = new Material();
        pMat1->setAmbient(glm::vec4(0.5f, 0.5f, 0.31f, 1.0f));
        pMat1->setDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        pMat1->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        // pMat1->setShine(32.0f);
        pMat1->addTexture(new TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png"));

        // createTerrain("terra", "./data/terrain/terrain1.bin", 1.0, glm::vec3(150.0, 0.0, 0.0), pMat1);
        createTerrain("terra", "./data/terrain/heightmap2.tga", 1.0, glm::vec3(150.0, 0.0, 0.0), pMat1);

        // Wrapper do game
        Game* game = new Game(video, group1);

        // Controle do fluxo de programa
        FlowControl* pControle = new FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

        mapa.clear();

        SDL_Log("appTest finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fail 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
