
#define NAME "Solid Leaf BSP/ PVS TUTORIAL"
#define TITLE "BSP  Tutorial"

#define WIN32_LEAN_AND_MEAN

#include "runtime_framework.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>

#define VIEWMODE_FIRSTPERSON 0
#define VIEWMODE_TOPDOWN 1
#define SCREENWIDTH 640
#define SCREENHEIGHT 480

LPD3DXFONT MainFont;

int LEFTKEY = 0, RIGHTKEY = 0, UPKEY = 0, DOWNKEY = 0;
;

// These vars are flags to signal that rendering is being done and the View MOde to render in and whether the app
// is about to quit

int rendering = 1;
int ViewMode = VIEWMODE_TOPDOWN;
int quit = 0;

// These hold the Clip Planes for the View Frustum and a flag to enable/disable Fristum Culling
PLANE2 FrustumPlanes[6];
bool DontFrustumReject = true;

// The following describe the Position,Orientation and current rotation arounf the Y Axis to be performed
float Yaw = 0.0;
D3DXVECTOR3 look_vector(-1, 0, 0);
D3DXVECTOR3 up_vector(0, 1, 0);
D3DXVECTOR3 right_vector(0, 0, 1);
D3DXVECTOR3 position(33, 3, -75);

// These contain the Textures for the Level
LPDIRECT3DTEXTURE8 lpTextureSurface[25];
long NumberOfTextures = 25;

LRESULT CALLBACK WindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam);
BOOL init_ddraw(HWND hWnd);
void InitPolygons(void);
void renderframe(void);
void UpdateViewpos(void);
void LoadTextures(void);
void SetupFont(void);
void LoadBSPTree(char* filename);
void sleep(clock_t wait);

HWND hwnd;
HINSTANCE hInst;
LPDIRECT3D8 lpD3D = NULL;          // Used to create the D3DDevice
LPDIRECT3DDEVICE8 lpDevice = NULL; // Our rendering device

long PVSCompressedSize = 0; // This contains the size of the entire PVS Set AFTER compression so
                            // the PVS array can be resized

// *****************************************************************************************
//
//                        Init Polygons
//
// This function is the mother of nearly all init routines.It loads the Scene,compiles the leaf
// bsp tree, generates the portals and calculates the pvs set.And then makes the portals a nice
// pretty color so i can see them.
//*******************************************************************************************

void InitPolygons(void) {
    SetupFont();

    LoadBSPTree("demolevel.bsp");
    LoadTextures();

} // END FUNCTION

//****************************SET UP MAIN WINDOW**************************************

static BOOL doInit(HINSTANCE hInstance, int nCmdShow) {

    WNDCLASS wc;

    // Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(DWORD);
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "my window";

    if (!RegisterClass(&wc)) {
        return FALSE;
    }

    // Get dimensions of display

    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a window and display

    HWND hWnd;
    hWnd = CreateWindow("my window",           // class
                        "BSP me baby",         // caption
                        WS_VISIBLE | WS_POPUP, // style
                        0,                     // left
                        0,                     // top
                        ScreenWidth,           // width
                        ScreenHeight,          // height
                        NULL,                  // parent window
                        NULL,                  // menu
                        hInstance,             // instance
                        NULL);                 // parms
    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // initialze DirectDraw
    init_ddraw(hWnd);
    hwnd = hWnd;
    InitPolygons();

    return TRUE;

} /* doInit */

//****************************** WIN MAIN ******************************************

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    MSG msg;
    hInst = hInstance;
    hPrevInstance = hPrevInstance;

    float time_elapsed;     // time since previous frame
    float time_scale;       // scaling factor for time
    LONGLONG cur_time;      // current time
    LONGLONG perf_cnt;      // performance timer frequency
    BOOL perf_flag = FALSE; // flag determining which timer to use
    LONGLONG last_time = 0;

    if (!doInit(hInst, nCmdShow)) {
        return FALSE;
    }

    if (QueryPerformanceFrequency((LARGE_INTEGER*)&perf_cnt)) {

        perf_flag = TRUE;
        QueryPerformanceCounter((LARGE_INTEGER*)&last_time);
        time_scale = 1.0f / perf_cnt;

    } else {

        // no performance counter, read in using timeGetTime

        last_time = timeGetTime();
        time_scale = 0.001f;
    }

    // Now we're ready to recieve and process Windows messages.

    BOOL bGotMsg;

    while (quit != 1) {
        while (bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (rendering == 1) {

            if (perf_flag)

                QueryPerformanceCounter((LARGE_INTEGER*)&cur_time);
            else
                cur_time = timeGetTime();

            // calculate elapsed time

            time_elapsed = (cur_time - last_time) * time_scale; // time in seconds

            // save frame time

            last_time = cur_time;

            if (LEFTKEY == 1) {
                Yaw = -2.05f * time_elapsed;
            }
            if (RIGHTKEY == 1) {
                Yaw = +2.05f * time_elapsed;
            }

            if (DOWNKEY == 1) {
                D3DXVECTOR3 NewPos = position - (25.4f * look_vector) * time_elapsed;
                D3DXVECTOR3 NewPos2 = position - (65.4f * look_vector) * time_elapsed;

                if (CollideSphere(&NewPos2, 0.5, 0) == false) {
                    position = NewPos;
                }
            }

            if (UPKEY == 1) {

                D3DXVECTOR3 NewPos = position + (25.4f * look_vector) * time_elapsed;
                D3DXVECTOR3 NewPos2 = position + (65.4f * look_vector) * time_elapsed;

                if (CollideSphere(&NewPos2, 0.5, 0) == false)

                {
                    position = NewPos;
                }
            }

            renderframe();
        }
    }
    // return final message

    return msg.wParam;
}

//***************************************************************************************
//
//                             WINDOW PROC
//***************************************************************************************

LRESULT CALLBACK WindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam) {
    int i;
    switch (uMsg) // ropey keyboard handling but please forgive me
    {

        case WM_DESTROY:

            rendering = 0;

            DeletePolygonArray(); // deallocated memory.Just freeing the Polygon would not otherwise free up the
                                  // vertex or index arrays.There must be deleted first to avoid memory leakage.
            free(NodeArray);      // Free up Arrays
            free(LeafArray);
            free(PlaneArray);
            free(PVSData);

            for (i = 0; i < NumberOfTextures; i++) // Release Texture Maps
            {
                lpTextureSurface[i]->Release();
            }

            lpDevice->Release(); // Release all DX interfaces in order of creation
            lpD3D->Release();
            PostQuitMessage(0);
            break;

        case WM_MOUSEMOVE:
            SetCursor(NULL);
            break;

        case WM_KEYDOWN:
            switch (wParam) {

                case VK_F1:
                    if (ViewMode == VIEWMODE_FIRSTPERSON) {
                        ViewMode = VIEWMODE_TOPDOWN;
                    } else {
                        ViewMode = VIEWMODE_FIRSTPERSON;
                    }
                    break;

                case VK_F2:
                    if (DontFrustumReject == true) {
                        DontFrustumReject = false;
                    } else {
                        DontFrustumReject = true;
                    }
                    break;

                case VK_LEFT:
                    LEFTKEY = 1;

                    break;

                case VK_RIGHT:

                    RIGHTKEY = 1;
                    break;

                case VK_UP:

                    UPKEY = 1;

                    break;

                case VK_DOWN:

                    DOWNKEY = 1;

                    break;
            }
            break;

        case WM_KEYUP:
            switch (wParam) {

                case VK_LEFT:
                    LEFTKEY = 0;
                    break;

                case VK_RIGHT:
                    RIGHTKEY = 0;
                    break;

                case VK_UP:
                    UPKEY = 0;

                    break;
                case VK_DOWN:
                    DOWNKEY = 0;

                    break;

                case VK_ESCAPE:
                    // exit the program on escape
                    quit = 1;

                    DestroyWindow(hWnd);
                    break;
            }
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0L;
}

//*********************************************************************************
//
//             SETUP DIRECT DRAW and DIRECT3D ENVIRONMENT
//
//*******************************************************************************

BOOL init_ddraw(HWND hWnd) {
    D3DPRESENT_PARAMETERS d3dpp; // set up intialization parameters
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = FALSE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dpp.BackBufferWidth = SCREENWIDTH;
    d3dpp.BackBufferHeight = SCREENHEIGHT;
    d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    lpD3D = Direct3DCreate8(D3D_SDK_VERSION);

    if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpDevice))) {
        d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
        if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpDevice))) {
            return false;
        }
    }

    // Set projection matrix
    D3DXMATRIX proj_m;
    D3DXMatrixPerspectiveFovLH(&proj_m, 1.0f, 0.77777f, 0.3f, 500.0f);
    lpDevice->SetTransform(D3DTS_PROJECTION, &proj_m);
    lpDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));

    lpDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    lpDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    lpDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

    float fBias = -1.5;
    lpDevice->SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD)(&fBias)));

    lpDevice->SetVertexShader(D3DFVF_LVERTEX);

    lpDevice->SetRenderState(D3DRS_LIGHTING, false); // we are using prelit vertices
    lpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    lpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    lpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    lpDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

    return TRUE;
}

// ****************************************************************************************
//                          RENDER FRAME
//
// This cute little function renders every thing
//
//****************************************************************************************

void renderframe(void) {

    UpdateViewpos();
    ExtractFrustumPlanes(FrustumPlanes);

    lpDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    lpDevice->BeginScene();
    D3DVECTOR LookAtPos = position;

    RenderTree(LookAtPos);

    // print test onfo on the screen

    RECT rect;
    rect.left = 400;
    rect.right = 640;
    rect.top = 0;
    rect.bottom = 150;

    if (DontFrustumReject == true) {
        MainFont->DrawTextA("Press F2 to Enable Frustum Rejection", 36, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(155, 255, 255));
    } else {
        MainFont->DrawTextA("Press F2 to Disable Frustum Rejection", 37, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(155, 255, 255));
    }

    rect.left = 0;
    rect.right = 640;
    rect.top = 0;
    rect.bottom = 150;

    if (ViewMode == VIEWMODE_TOPDOWN) // Draw a Cross Hair at center of the Screen to represent Player
    {
        MainFont->DrawTextA("  Press F1 for First Person View", 32, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
        rect.left = (SCREENWIDTH / 2) - 5;
        rect.top = (SCREENHEIGHT / 2) - 5;
        MainFont->DrawTextA("+", 1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
    } else {
        MainFont->DrawTextA("  Press F1 for Top Down (PVS) View", 34, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
    }

    lpDevice->EndScene();
    lpDevice->Present(NULL, NULL, NULL, NULL);
}

// *****************************************************************************************
//
//                         UpdateViewPos
//
// Called each frame to update the view matix.
//******************************************************************************************

void UpdateViewpos() {
    D3DXMATRIX PitchMatrix, YawMatrix; // Well need a Matrix for pitch and another for yaw

    D3DXMatrixRotationAxis(&YawMatrix, &up_vector, Yaw);

    D3DXVec3TransformCoord(&look_vector, &look_vector, &YawMatrix);
    D3DXVec3TransformCoord(&right_vector, &right_vector, &YawMatrix);

    D3DXVec3Normalize(&look_vector, &look_vector);
    D3DXVec3Cross(&right_vector, &up_vector, &look_vector);
    D3DXVec3Normalize(&right_vector, &right_vector);
    D3DXVec3Cross(&up_vector, &look_vector, &right_vector);
    ;
    D3DXVec3Normalize(&up_vector, &up_vector);

    D3DXMATRIX view;
    D3DXMatrixIdentity(&view);

    if (ViewMode == VIEWMODE_FIRSTPERSON) {
        view._11 = right_vector.x;
        view._12 = up_vector.x;
        view._13 = look_vector.x;
        view._21 = right_vector.y;
        view._22 = up_vector.y;
        view._23 = look_vector.y;
        view._31 = right_vector.z;
        view._32 = up_vector.z;
        view._33 = look_vector.z;
        view._41 = -D3DXVec3Dot(&position, &right_vector); // dot product defined in d3dtypes.h
        view._42 = -D3DXVec3Dot(&position, &up_vector);
        view._43 = -D3DXVec3Dot(&position, &look_vector);
    } else {
        D3DXVECTOR3 TopDown = D3DXVECTOR3(position.x, 150, position.z);

        view._11 = right_vector.x;
        view._12 = look_vector.x;
        view._13 = 0.0f; //
        view._21 = right_vector.y;
        view._22 = look_vector.y;
        view._23 = -1.0f; //
        view._31 = right_vector.z;
        view._32 = look_vector.z;
        view._33 = 0.0f;                                  //
        view._41 = -D3DXVec3Dot(&TopDown, &right_vector); // dot product defined in d3dtypes.h
        view._42 = -D3DXVec3Dot(&TopDown, &look_vector);
        view._43 = -D3DXVec3Dot(&TopDown, &D3DXVECTOR3(0, -1, 0));
    }
    lpDevice->SetTransform(D3DTS_VIEW, &view);
    Yaw = 0.0;
}

//**************************************************************************
//
//              Loads 25 textures for the level
//
//*************************************************************************

void LoadTextures(void) {
    // load in textures

    D3DXCreateTextureFromFileA(lpDevice, "checkered_floor1.jpg", &lpTextureSurface[0]);
    D3DXCreateTextureFromFileA(lpDevice, "brick2.jpg", &lpTextureSurface[1]);
    D3DXCreateTextureFromFileA(lpDevice, "metalrustyfloor1.jpg", &lpTextureSurface[2]);

    D3DXCreateTextureFromFileA(lpDevice, "brick3.jpg", &lpTextureSurface[3]);
    D3DXCreateTextureFromFileA(lpDevice, "curvyfloor.jpg", &lpTextureSurface[4]);
    D3DXCreateTextureFromFileA(lpDevice, "doomfloor.jpg", &lpTextureSurface[5]);
    D3DXCreateTextureFromFileA(lpDevice, "crate.jpg", &lpTextureSurface[6]);
    D3DXCreateTextureFromFileA(lpDevice, "stones1.jpg", &lpTextureSurface[7]);
    D3DXCreateTextureFromFileA(lpDevice, "wood1.jpg", &lpTextureSurface[8]);
    D3DXCreateTextureFromFileA(lpDevice, "wood2.jpg", &lpTextureSurface[9]);
    D3DXCreateTextureFromFileA(lpDevice, "celtic.jpg", &lpTextureSurface[10]);
    D3DXCreateTextureFromFileA(lpDevice, "celtic1.jpg", &lpTextureSurface[11]);
    D3DXCreateTextureFromFileA(lpDevice, "rock1.jpg", &lpTextureSurface[12]);
    D3DXCreateTextureFromFileA(lpDevice, "oldmetalriveted.jpg", &lpTextureSurface[13]);
    D3DXCreateTextureFromFileA(lpDevice, "stone2.jpg", &lpTextureSurface[14]);
    D3DXCreateTextureFromFileA(lpDevice, "brick1.jpg", &lpTextureSurface[15]);
    D3DXCreateTextureFromFileA(lpDevice, "concrete1.jpg", &lpTextureSurface[16]);
    D3DXCreateTextureFromFileA(lpDevice, "brickz2.jpg", &lpTextureSurface[17]);
    D3DXCreateTextureFromFileA(lpDevice, "construct2.jpg", &lpTextureSurface[18]);
    D3DXCreateTextureFromFileA(lpDevice, "construct2c.jpg", &lpTextureSurface[19]);
    D3DXCreateTextureFromFileA(lpDevice, "doomgrey1.jpg", &lpTextureSurface[20]);
    D3DXCreateTextureFromFileA(lpDevice, "doomgrey2.jpg", &lpTextureSurface[21]);
    D3DXCreateTextureFromFileA(lpDevice, "granitefloor.jpg", &lpTextureSurface[22]);
    D3DXCreateTextureFromFileA(lpDevice, "stained_glass1.jpg", &lpTextureSurface[23]);
    D3DXCreateTextureFromFileA(lpDevice, "stained_glass2.jpg", &lpTextureSurface[24]);
}

//*****************************************************************************************
//                                    LOAD BSP
//
//
//*****************************************************************************************
void LoadBSPTree(char* filename) {
    FILE* stream;
    long a;
    stream = fopen(filename, "rb");

    // Read Node Array  array.We can Read the array as one block
    fread(&NumberOfNodes, sizeof(long), 1, stream);
    NodeArray = (NODE*)malloc(NumberOfNodes * sizeof(NODE));
    NODE* n = NodeArray;
    for (a = 0; a < NumberOfNodes; a++) {
        fread(&n->IsLeaf, sizeof(unsigned char), 1, stream);
        fread(&n->Plane, sizeof(unsigned long), 1, stream);
        fread(&n->Front, sizeof(unsigned long), 1, stream);
        fread(&n->Back, sizeof(signed long), 1, stream);
        n++;
    }

    // Write the Plane Array
    fread(&NumberOfPlanes, sizeof(long), 1, stream);
    PlaneArray = (PLANE*)malloc(NumberOfPlanes * sizeof(PLANE));
    fread(PlaneArray, sizeof(PLANE), NumberOfPlanes, stream);

    // Write Leaf Array.This also has some reduntant Run Time fields so we shall remove
    // these when writing.
    fread(&NumberOfLeafs, sizeof(long), 1, stream);
    LeafArray = (LEAF*)malloc(NumberOfLeafs * sizeof(LEAF));
    LEAF* l = LeafArray;
    for (a = 0; a < NumberOfLeafs; a++) {
        fread(&l->StartPolygon, sizeof(long), 1, stream);
        fread(&l->EndPolygon, sizeof(long), 1, stream);
        fread(&l->PVSIndex, sizeof(long), 1, stream);
        fread(&l->BoundingBox, sizeof(BOUNDINGBOX), 1, stream);
        l++;
    }

    // WritePolygonArray.There are fields in this structure that were used for sompiling and are
    // not needed at runtime."BeenUsedAsSplitter' is not needed and neither is the 'Next' Pointer.
    fread(&NumberOfPolygons, sizeof(long), 1, stream);
    PolygonArray = (POLYGON*)malloc(NumberOfPolygons * sizeof(POLYGON));
    POLYGON* p = PolygonArray;
    for (a = 0; a < NumberOfPolygons; a++) {
        fread(&p->NumberOfVertices, sizeof(WORD), 1, stream);
        p->VertexList = new D3DLVERTEX[p->NumberOfVertices];
        fread(p->VertexList, sizeof(D3DLVERTEX), p->NumberOfVertices, stream);
        fread(&p->NumberOfIndices, sizeof(WORD), 1, stream);
        p->Indices = new WORD[p->NumberOfIndices];
        fread(p->Indices, sizeof(WORD), p->NumberOfIndices, stream);
        fread(&p->Normal, sizeof(D3DXVECTOR3), 1, stream);
        fread(&p->TextureIndex, sizeof(DWORD), 1, stream);
        p++;
    }

    // Now all we have to do is write the PVS Data itself
    fread(&PVSCompressedSize, sizeof(long), 1, stream);
    PVSData = (BYTE*)malloc(PVSCompressedSize * sizeof(BYTE));
    fread(PVSData, sizeof(BYTE), PVSCompressedSize, stream);

    // All done
    fclose(stream);
}

//***************************************************************************************
void SetupFont(void) {

    LOGFONT logFont;
    ZeroMemory(&logFont, sizeof(LOGFONT));
    strcpy(logFont.lfFaceName, "arial\n");
    HDC TempDC = GetDC(hwnd);
    logFont.lfHeight = -MulDiv(10, GetDeviceCaps(TempDC, LOGPIXELSY), 72);
    logFont.lfWidth = 6;
    ReleaseDC(hwnd, TempDC);

    D3DXCreateFontIndirect(lpDevice, &logFont, &MainFont);
    RECT rect, rect2;
    rect.left = 0;
    rect.right = SCREENWIDTH;
    rect.top = 0;
    rect.bottom = SCREENHEIGHT;
    rect2.left = 0;
    rect2.right = SCREENWIDTH;
    rect2.top = 80;
    rect2.bottom = 500;

    LPDIRECT3DSURFACE8 titlescreen, backbuffer;
    lpDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
    lpDevice->CreateImageSurface(640, 480, D3DFMT_R5G6B5, &titlescreen);
    D3DXLoadSurfaceFromFileA(titlescreen, NULL, NULL, "title.jpg", NULL, D3DX_FILTER_NONE, 0, NULL);

    lpDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    lpDevice->CopyRects(titlescreen, NULL, 0, backbuffer, NULL);

    titlescreen->Release();
    backbuffer->Release();
    lpDevice->Present(NULL, NULL, NULL, NULL);

    sleep(3500);
}

void sleep(clock_t wait) {
    clock_t goal;
    goal = wait + clock();
    while (goal > clock())
        ;
}
