#include "OpenGLRender.hpp"

namespace Chimera {

OpenGLRenderer::OpenGLRenderer() {
    LightAngle = 22.5f;

    Wireframe = false;
    DisplayShadowMap = false;
    RenderAABB = false;
    VisualizeRenderingOrder = false;
    SortVisibleGeometryNodes = true;
    RenderSlow = false;

    Depth = -1;
}

OpenGLRenderer::~OpenGLRenderer() {}

// FIXME: refazer
bool OpenGLRenderer::Init() {
    // bool Error = false;

    // if (!GLEW_EXT_framebuffer_object) {
    //     ErrorLog.Append("GL_EXT_framebuffer_object not supported!\r\n");
    //     Error = true;
    // }

    // Error |= !Shader.Load("glsl120shader.vs", "glsl120shader.fs");

    // Error |= !terrain.LoadBinary("terrain1.bin");

    // if (Error) {
    //     return false;
    // }

    // Shader.UniformLocations = new GLuint[2];
    // Shader.UniformLocations[0] = glGetUniformLocation(Shader, "ShadowMatrix");
    // Shader.UniformLocations[1] = glGetUniformLocation(Shader, "LightDirection");

    // glUseProgram(Shader);
    // glUniform1i(glGetUniformLocation(Shader, "ShadowMap"), 0);
    // glUniform1i(glGetUniformLocation(Shader, "RotationTexture"), 1);
    // glUniform1f(glGetUniformLocation(Shader, "Scale"), 1.0f / 64.0f);
    // glUniform1f(glGetUniformLocation(Shader, "Radius"), 1.0f / 1024.0f);
    // glUseProgram(0);

    // ShadowMapSize = SHADOW_MAP_SIZE > gl_max_texture_size ? gl_max_texture_size : SHADOW_MAP_SIZE;

    // glGenTextures(1, &ShadowMap);
    // glBindTexture(GL_TEXTURE_2D, ShadowMap);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ShadowMapSize, ShadowMapSize, 0, GL_DEPTH_COMPONENT,
    // GL_FLOAT,
    //              NULL);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // srand(GetTickCount());

    // vec4* RotationTextureData = new vec4[4096];

    // float RandomAngle = 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;

    // for (int i = 0; i < 4096; i++) {
    //     RotationTextureData[i].x = cos(RandomAngle);
    //     RotationTextureData[i].y = sin(RandomAngle);
    //     RotationTextureData[i].z = -RotationTextureData[i].y;
    //     RotationTextureData[i].w = RotationTextureData[i].x;

    //     RotationTextureData[i] *= 0.5f;
    //     RotationTextureData[i] += 0.5f;

    //     RandomAngle += 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;
    // }

    // glGenTextures(1, &RotationTexture);
    // glBindTexture(GL_TEXTURE_2D, RotationTexture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_FLOAT, RotationTextureData);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // delete[] RotationTextureData;

    // glGenFramebuffersEXT(1, &FBO);
    // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
    // glDrawBuffers(0, NULL);
    // glReadBuffer(GL_NONE);
    // glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, ShadowMap, 0);
    // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // RenderShadowMap();

    // float Height = terrain.GetHeight(0.0f, 0.0f);

    // camera.Look(glm::vec3(0.0f, Height + 1.75f, 0.0f), glm::vec3(0.0f, Height + 1.75f, -1.0f));

    return true;
}

// FIXME: Refazer
void OpenGLRenderer::Render() {
    // glViewport(0, 0, Width, Height);

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glMatrixMode(GL_PROJECTION);
    // glLoadMatrixf(&camera.ProjectionMatrix);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadMatrixf(&camera.ViewMatrix);

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // if (!RenderSlow) {
    //     terrain.CheckVisibility(camera.frustum, SortVisibleGeometryNodes);
    // }

    // if (Wireframe) {
    //     glColor3f(0.0f, 0.0f, 0.0f);

    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //     if (RenderSlow) {
    //         terrain.RenderSlow();
    //     } else {
    //         terrain.Render();
    //     }

    //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // }

    // glColor3f(1.0f, 1.0f, 1.0f);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, ShadowMap);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, RotationTexture);

    // glUseProgram(Shader);

    // if (RenderSlow) {
    //     terrain.RenderSlow();
    // } else {
    //     terrain.Render(VisualizeRenderingOrder);
    // }

    // glUseProgram(0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, 0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // if (!RenderSlow && RenderAABB) {
    //     glColor3f(0.0f, 1.0f, 0.0f);

    //     terrain.RenderAABB(Depth);
    // }

    // glDisable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);

    // if (DisplayShadowMap) {
    //     glViewport(16, 16, 256, 256);

    //     glMatrixMode(GL_PROJECTION);
    //     glLoadIdentity();

    //     glMatrixMode(GL_MODELVIEW);
    //     glLoadIdentity();

    //     glColor3f(1.0f, 1.0f, 1.0f);

    //     glEnable(GL_TEXTURE_2D);

    //     glBindTexture(GL_TEXTURE_2D, ShadowMap);

    //     glBegin(GL_QUADS);
    //     glTexCoord2f(0.0f, 0.0f);
    //     glVertex2f(-1.0f, -1.0f);
    //     glTexCoord2f(1.0f, 0.0f);
    //     glVertex2f(1.0f, -1.0f);
    //     glTexCoord2f(1.0f, 1.0f);
    //     glVertex2f(1.0f, 1.0f);
    //     glTexCoord2f(0.0f, 1.0f);
    //     glVertex2f(-1.0f, 1.0f);
    //     glEnd();

    //     glBindTexture(GL_TEXTURE_2D, 0);

    //     glDisable(GL_TEXTURE_2D);
    // }
}

void OpenGLRenderer::Animate(float FrameTime) {}

void OpenGLRenderer::Resize(int Width, int Height) {
    this->Width = Width;
    this->Height = Height;

    camera.SetPerspective(45.0f, (float)Width / (float)Height, 0.125f, 1024.0f);
}

// FIXME: REFAZER
void OpenGLRenderer::Destroy() {
    // Shader.Destroy();

    // terrain.Destroy();

    // glDeleteTextures(1, &ShadowMap);
    // glDeleteTextures(1, &RotationTexture);

    // if (GLEW_EXT_framebuffer_object) {
    //     glDeleteFramebuffersEXT(1, &FBO);
    // }
}

// FIXME: Refazer
void OpenGLRenderer::RenderShadowMap() {
    // glm::vec3 LightPosition =
    //     rotate(glm::vec3((float)terrain.GetSize(), 0.0f, 0.0f), -LightAngle, glm::vec3(0.0f, 1.0f, -1.0f));

    // glm::vec3 LightDirection = normalize(LightPosition);

    // LightViewMatrix = look(LightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // glm::vec3 Min, Max;

    // terrain.GetMinMax(LightViewMatrix, Min, Max);

    // LightProjectionMatrix = ortho(Min.x, Max.x, Min.y, Max.y, -Max.z, -Min.z);

    // ShadowMatrix = BiasMatrix * LightProjectionMatrix * LightViewMatrix;

    // glUseProgram(Shader);
    // glUniformMatrix4fv(Shader.UniformLocations[0], 1, GL_FALSE, &ShadowMatrix);
    // glUniform3fv(Shader.UniformLocations[1], 1, &LightDirection);
    // glUseProgram(0);

    // glViewport(0, 0, ShadowMapSize, ShadowMapSize);

    // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);

    // glClear(GL_DEPTH_BUFFER_BIT);

    // glMatrixMode(GL_PROJECTION);
    // glLoadMatrixf(&LightProjectionMatrix);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadMatrixf(&LightViewMatrix);

    // glEnable(GL_DEPTH_TEST);

    // terrain.RenderSlowToShadowMap();

    // glDisable(GL_DEPTH_TEST);

    // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void OpenGLRenderer::CheckCameraTerrainPosition(glm::vec3& Movement) {
    glm::vec3 CameraPosition = camera.reference + Movement, Min = terrain.GetMin(), Max = terrain.GetMax();

    if (CameraPosition.x < Min.x)
        Movement += glm::vec3(Min.x - CameraPosition.x, 0.0f, 0.0f);
    if (CameraPosition.x > Max.x)
        Movement += glm::vec3(Max.x - CameraPosition.x, 0.0f, 0.0f);
    if (CameraPosition.z < Min.z)
        Movement += glm::vec3(0.0f, 0.0f, Min.z - CameraPosition.z);
    if (CameraPosition.z > Max.z)
        Movement += glm::vec3(0.0f, 0.0f, Max.z - CameraPosition.z);

    CameraPosition = camera.reference + Movement;

    float Height = terrain.GetHeight(CameraPosition.x, CameraPosition.z);

    Movement += glm::vec3(0.0f, Height + 1.75f - camera.reference.y, 0.0f);
}

// FIXME: REFAZER
void OpenGLRenderer::CheckCameraKeys(float FrameTime) {
    // unsigned short Keys = 0x00;

    // if (GetKeyState('W') & 0x80)
    //     Keys |= 0x01;
    // if (GetKeyState('S') & 0x80)
    //     Keys |= 0x02;
    // if (GetKeyState('A') & 0x80)
    //     Keys |= 0x04;
    // if (GetKeyState('D') & 0x80)
    //     Keys |= 0x08;
    // // if(GetKeyState('R') & 0x80) Keys |= 0x10;
    // // if(GetKeyState('F') & 0x80) Keys |= 0x20;

    // if (GetKeyState(VK_SHIFT) & 0x80)
    //     Keys |= 0x40;
    // if (GetKeyState(VK_CONTROL) & 0x80)
    //     Keys |= 0x80;

    // if (Keys & 0x3F) {
    //     glm::vec3 Movement = camera.OnKeys(Keys, FrameTime * 0.5f);

    //     CheckCameraTerrainPosition(Movement);

    //     camera.Move(Movement);
    // }
}

// FIXME: REFAZER
void OpenGLRenderer::OnKeyDown(unsigned short Key) {
    // switch (Key) {
    //     case VK_F1:
    //         Wireframe = !Wireframe;
    //         break;

    //     case VK_F2:
    //         DisplayShadowMap = !DisplayShadowMap;
    //         break;

    //     case VK_F3:
    //         RenderAABB = !RenderAABB;
    //         break;

    //     case VK_F4:
    //         VisualizeRenderingOrder = !VisualizeRenderingOrder;
    //         break;

    //     case VK_F5:
    //         SortVisibleGeometryNodes = !SortVisibleGeometryNodes;
    //         break;

    //     case VK_F6:
    //         RenderSlow = !RenderSlow;
    //         break;

    //     case VK_F7:
    //         terrain.SaveBinary("terrain-saved.bin");
    //         break;

    //     case '1':
    //         if (terrain.LoadBinary("terrain1.bin")) {
    //             glm::vec3 Movement;
    //             CheckCameraTerrainPosition(Movement);
    //             camera.Move(Movement);
    //             RenderShadowMap();
    //         }
    //         break;

    //     case '2':
    //         if (terrain.LoadTexture2D("terrain2.jpg", 32.0f, -16.0f)) {
    //             glm::vec3 Movement;
    //             CheckCameraTerrainPosition(Movement);
    //             camera.Move(Movement);
    //             RenderShadowMap();
    //         }
    //         break;

    //     case '3':
    //         if (terrain.LoadTexture2D("terrain3.jpg", 128.0f, -64.0f)) {
    //             glm::vec3 Movement;
    //             CheckCameraTerrainPosition(Movement);
    //             camera.Move(Movement);
    //             RenderShadowMap();
    //         }
    //         break;

    //     case '4':
    //         if (terrain.LoadTexture2D("terrain4.jpg", 128.0f, -64.0f)) {
    //             glm::vec3 Movement;
    //             CheckCameraTerrainPosition(Movement);
    //             camera.Move(Movement);
    //             RenderShadowMap();
    //         }
    //         break;

    //     case VK_MULTIPLY:
    //         Depth++;
    //         break;

    //     case VK_DIVIDE:
    //         if (Depth > -1)
    //             Depth--;
    //         break;

    //     case VK_ADD:
    //         LightAngle += 3.75f;
    //         RenderShadowMap();
    //         break;

    //     case VK_SUBTRACT:
    //         LightAngle -= 3.75f;
    //         RenderShadowMap();
    //         break;
    // }
}

void OpenGLRenderer::OnLButtonDown(int X, int Y) {
    LastClickedX = X;
    LastClickedY = Y;
}

void OpenGLRenderer::OnLButtonUp(int X, int Y) {
    if (X == LastClickedX && Y == LastClickedY) {}
}

// FIXME: Refazer
void OpenGLRenderer::OnMouseMove(int X, int Y) {
    // if (GetKeyState(VK_RBUTTON) & 0x80) {
    //     camera.OnMouseMove(LastX - X, LastY - Y);
    // }

    // LastX = X;
    // LastY = Y;
}

void OpenGLRenderer::OnMouseWheel(short zDelta) { camera.OnMouseWheel(zDelta); }

void OpenGLRenderer::OnRButtonDown(int X, int Y) {
    LastClickedX = X;
    LastClickedY = Y;
}

void OpenGLRenderer::OnRButtonUp(int X, int Y) {
    if (X == LastClickedX && Y == LastClickedY) {}
}

// ----------------------------------------------------------------------------------------------------------------------------

// CString ModuleDirectory, ErrorLog;

// ----------------------------------------------------------------------------------------------------------------------------

void GetModuleDirectory() {
    // char* moduledirectory = new char[256];
    // GetModuleFileName(GetModuleHandle(NULL), moduledirectory, 256);
    // *(strrchr(moduledirectory, '\\') + 1) = 0;
    // ModuleDirectory = moduledirectory;
    // delete[] moduledirectory;
}

} // namespace Chimera