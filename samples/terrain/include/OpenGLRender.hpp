#ifndef __GAME_TERRAIN_OPENGL_RENDER__HPP
#define __GAME_TERRAIN_OPENGL_RENDER__HPP

#include "Camera.hpp"
#include "Terrain.hpp"
#include "chimera/render/Shader.hpp"

namespace Chimera {

class OpenGLRenderer {
  public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    bool Init();
    void Render();
    void Animate(float FrameTime);
    void Resize(int Width, int Height);
    void Destroy();

    void CheckCameraKeys(float FrameTime);

    void OnKeyDown(unsigned short Key);
    void OnLButtonDown(int X, int Y);
    void OnLButtonUp(int X, int Y);
    void OnMouseMove(int X, int Y);
    void OnMouseWheel(short zDelta);
    void OnRButtonDown(int X, int Y);
    void OnRButtonUp(int X, int Y);

  private:
    void RenderShadowMap();
    void CheckCameraTerrainPosition(glm::vec3& Movement);
    int LastX, LastY, LastClickedX, LastClickedY;
    int Width, Height;
    Camera camera;
    // CShaderProgram Shader;
    Terrain terrain;
    float LightAngle;
    glm::mat4 LightViewMatrix, LightProjectionMatrix, ShadowMatrix;
    int ShadowMapSize;
    GLuint ShadowMap, RotationTexture, FBO;
    bool Wireframe, DisplayShadowMap, RenderAABB, VisualizeRenderingOrder, SortVisibleGeometryNodes, RenderSlow;
    int Depth;
};

} // namespace Chimera
#endif