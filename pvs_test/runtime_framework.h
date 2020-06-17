

#include <d3d8.h>
#include <d3dx8.h>
// Used by to return from Classify Point and Classify Poly functions
const int CP_FRONT = 1001;
const int CP_BACK = 1002;
const int CP_ONPLANE = 1003;
const int CP_SPANNING = 1004;

struct D3DLVERTEX {
    float x;
    float y;
    float z;
    D3DCOLOR color;
    D3DCOLOR specular;
    float tu;
    float tv;
};

#define D3DFVF_LVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

//*****************************************************************************************8
//
//                                   STRUCTURES
//
// All the structure used by the compiler and pvs calculator
//*****************************************************************************************
struct PLANE {
    D3DXVECTOR3 PointOnPlane;
    D3DXVECTOR3 Normal;
};

struct PLANE2 {
    float Distance;
    D3DXVECTOR3 Normal;
};

struct POLYGON {
    D3DLVERTEX* VertexList;
    D3DXVECTOR3 Normal;
    WORD NumberOfVertices;
    WORD NumberOfIndices;
    WORD* Indices;
    POLYGON* Next;
    long TextureIndex;
};

struct BOUNDINGBOX {
    D3DXVECTOR3 BoxMin;
    D3DXVECTOR3 BoxMax;
};

struct LEAF {
    long StartPolygon;
    long EndPolygon;
    long PVSIndex;

    BOUNDINGBOX BoundingBox;
};

struct NODE {
    unsigned char IsLeaf;
    unsigned long Plane;
    unsigned long Front;
    signed long Back;
};

//***********************************************************************************
//
//                 GLOBALS (tut tut naughty me)
//
//**************************************************************************************

extern PLANE2 FrustumPlanes[6]; // These hold the View Frustum for Clipping
extern bool DontFrustumReject;

extern D3DXVECTOR3 position;
extern D3DXVECTOR3 look_vector;
extern D3DXVECTOR3 up_vector;
extern D3DXVECTOR3 right_vector;

extern long BytesPerSet; // How many Bytes should be reserved for an Uncompressed LEAF pvs

// These are the Master Arrays that will contain the final tree,Portals and PVS set

extern POLYGON* PolygonArray;
extern NODE* NodeArray;
extern LEAF* LeafArray;
extern PLANE* PlaneArray;
extern BYTE* PVSData;

extern long NumberOfPolygons;
extern long NumberOfNodes;
extern long NumberOfLeafs;
extern long NumberOfPlanes;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern LPDIRECT3D8 lpD3D;          // Used to create the D3DDevice
extern LPDIRECT3DDEVICE8 lpDevice; // Our rendering device

extern LPDIRECT3DTEXTURE8 lpTextureSurface[25];
extern long NumberOfTextures; // Number Of Textures

//********************************************************************************************
//
//                           FUNCTIONS
//
//*******************************************************************************************

// Contained in BSPTREE.CPP

void RenderTree(D3DXVECTOR3 pos);
void DrawTree(long leaf);
bool Get_Intersect(D3DXVECTOR3* linestart, D3DXVECTOR3* lineend, D3DXVECTOR3* vertex, D3DXVECTOR3* normal, D3DXVECTOR3* intersection,
                   float* percentage);
int ClassifyPoint(D3DXVECTOR3* pos, PLANE* Plane);
bool LineOfSight(D3DXVECTOR3* Start, D3DXVECTOR3* End, long Node);
void DeletePolygonArray(void);
void DeletePolygon(POLYGON* Poly);
bool CollideSphere(D3DXVECTOR3* SphereCenter, float SphereRadius, long Node);

extern long MAXNUMBEROFNODES;
extern long MAXNUMBEROFPLANES;
extern long MAXNUMBEROFPOLYGONS;
extern long MAXNUMBEROFLEAFS;

// Contained in Frustum.cpp

void ExtractFrustumPlanes(PLANE2* Planes);
bool LeafInFrustum(long Leaf);
