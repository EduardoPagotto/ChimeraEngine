

#define NAME "Solid Leaf BSP/ PVS TUTORIAL"
#define TITLE "BSP  Tutorial"

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>

#include "pvs_bsp.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <mmsystem.h>
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

POLYGON* PolygonList; // This will contain the Polygons to be FED into the BSP compiler.The Polygons are removed from this
                      // list as they are added to the PolygonArray.

LRESULT CALLBACK WindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam);
BOOL init_ddraw(HWND hWnd);
void InitPolygons(void);
void renderframe(void);
void UpdateViewpos(void);
POLYGON* LoadMWM(char* filename);
void LoadTextures(void);
void SetupFont(void);
void SaveBSPTree(char* filename);
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
    ReserveInitialMemoryForArrays();

    PolygonList = NULL;
    PolygonList = LoadMWM("demolevel.mwm");

    LoadTextures();

    BuildBspTree(0, PolygonList);

    BuildPortals();

    BytesPerSet = (NumberOfLeafs + 7) >> 3;               // enough space for a LEAFS uncompressed PVS Set
    PVSData = (BYTE*)malloc(NumberOfLeafs * BytesPerSet); // Reserve enought for all leafs
    ZeroMemory(PVSData, NumberOfLeafs * BytesPerSet);     // Set to zero for safety
    PVSCompressedSize = CalculatePVS();                   // This function returns back the actual size of the Compressed
                                                          // PVS data array

    PVSData = (BYTE*)realloc(PVSData, PVSCompressedSize); // so resize the array so we dont waste any memory.(just to be nice)
    // SaveBSPTree("demolevel.bsp");//Unremark this Line to save in my BSP Format

    // for now just make the portals a nice pretty color so we can see them
    // this is purely for cosmetic reasons
    for (long i = 0; i < NumberOfPortals; i++) {
        for (int o = 0; o < PortalArray[i]->NumberOfVertices; o++) {
            D3DCOLOR col = D3DCOLOR_RGBA(0, 100, 0, 255);
            PortalArray[i]->VertexList[o].color = col;
        }
    }

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
                Yaw = -3.05f * time_elapsed;
            }
            if (RIGHTKEY == 1) {
                Yaw = +3.05f * time_elapsed;
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
            DeletePortalArray();  // Special Case array deletion which also deletes each structures dynamic
            DeletePolygonArray(); // allocated memory.Just freeing the Polygon would not otherwise free up the
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
