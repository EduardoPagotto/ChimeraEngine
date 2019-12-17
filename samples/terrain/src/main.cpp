#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
//#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/render/HeightMap.hpp"
#include "chimera/render/LoadHeightMap.hpp"
//#include "chimera/render/LoadObj.hpp"

int main(int argn, char** argv) {

    using namespace Chimera;

    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        CanvasGL* video = new CanvasGL("HeightMap", 800, 600);

        Shader* pShader = new Shader("mesh-default", shadeLoadProg("default", "./chimera/shaders/MeshFullShadow.vert",
                                                                   "./chimera/shaders/MeshFullShadow.frag"));

        NodeGroup* group1 = new NodeGroup(nullptr, "modelos");
        group1->setShader(pShader);

        // Propriedades da camera
        NodeCamera* pCam = new NodeCamera(group1, "Observador-01");
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

        // Propriedades da luz
        NodeLight* pLight = new NodeLight(group1, "Luz1");
        pLight->data.setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        pLight->data.setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
        pLight->data.setPosition(glm::vec3(80, 100, 150));

        Material* pMat1 = new Material();
        pMat1->setAmbient(glm::vec4(0.5f, 0.5f, 0.31f, 1.0f));
        pMat1->setDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        pMat1->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        pMat1->setShine(32.0f);
        pMat1->addTexture(new TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png"));

        // HeightMap
        NodeMesh* pMesh = new NodeMesh(group1, "terra");
        pMesh->setTransform(new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0))));
        pMesh->setMaterial(pMat1);
        LoadHeightMap* pHeight = new LoadHeightMap();
        pHeight->getMesh("./data/terrain/heightmap_8x8.png", pMesh->meshData);

        HeightMap* pHM = new HeightMap(pHeight->getWidth(), pHeight->getHeight(), 2, 2);
        pHM->split(pMesh->meshData);

        pMesh->meshData.changeSize(100.0, pMat1->hasTexture());

        Game* game = new Game(video, group1);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        delete pControle;
        delete game;
        delete video;

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fali 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}