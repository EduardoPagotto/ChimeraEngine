#ifndef __GAME_TESTE_TERRAIN__HPP
#define __GAME_TESTE_TERRAIN__HPP

#include "chimera/core/Application.hpp"
#include "chimera/core/Camera.hpp"
#include "chimera/render/3d/RenderableChunk.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    glm::mat4 model;
    Chimera::Scene activeScene;
    Chimera::Camera* camera;
    Chimera::Shader* pShader;
    Chimera::Light* pLight;
    Chimera::Material* pMaterial;
    Chimera::RenderableChunk* pHeightMap;
    Chimera::Renderer3d render3d;
};

#endif