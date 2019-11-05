#include "OpenGLRender.hpp"

namespace Chimera {

OpenGLRenderer::OpenGLRenderer() {
    //feito
}
OpenGLRenderer::~OpenGLRenderer() {}

bool OpenGLRenderer::Init() {
    //feito em game class
    return true;
}
void OpenGLRenderer::Render() {
// feito
}
void OpenGLRenderer::Animate(float FrameTime) {}
void OpenGLRenderer::Resize(int Width, int Height) {
    // desnecessario
    // this->Width = Width;
    // this->Height = Height;

    // camera.SetPerspective(45.0f, (float)Width / (float)Height, 0.125f, 1024.0f);
}
// FIXME: REFAZER
void OpenGLRenderer::Destroy() {
//feito
}
// FIXME: Refazer
void OpenGLRenderer::RenderShadowMap() {
    //fazendo
}
void OpenGLRenderer::CheckCameraTerrainPosition(glm::vec3& Movement) {
    //feito
}
// FIXME: REFAZER
void OpenGLRenderer::CheckCameraKeys(float FrameTime) {
    //feito
}
// FIXME: REFAZER
void OpenGLRenderer::OnKeyDown(unsigned short Key) {
// feito
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

// FIXME: sequencia de desenho
// void COpenGLView::OnPaint()
// {
// 	static DWORD LastFPSTime = GetTickCount(), LastFrameTime = LastFPSTime, Frame = 0;

// 	PAINTSTRUCT ps;

// 	HDC hDC = BeginPaint(hWnd, &ps);

// 	DWORD Time = GetTickCount();

// 	float FrameTime = (Time - LastFrameTime) * 0.001f;

// 	LastFrameTime = Time;

// 	if(Time - LastFPSTime > 1000)
// 	{
// 		FPS.Set("FPS: %d", Frame);

// 		SetWindowText(hWnd, Title + " - " + Resolution + ", " + MSAA + ", " + ATF + ", " + FPS + " - " +
// Renderer);

// 		LastFPSTime = Time;
// 		Frame = 0;
// 	}
// 	else
// 	{
// 		Frame++;
// 	}

// 	OpenGLRenderer.CheckCameraKeys(FrameTime);

// 	OpenGLRenderer.Render();

// 	OpenGLRenderer.Animate(FrameTime);

// 	SwapBuffers(hDC);

// 	EndPaint(hWnd, &ps);

// 	InvalidateRect(hWnd, NULL, FALSE);
// }

} // namespace Chimera