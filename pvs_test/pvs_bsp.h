
//#include <d3d8.h>
//#include <d3dx8.h>
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

struct CLIPPLANES // Holds the Clip Planes for the Anti-Penumbra
{
    WORD NumberOfPlanes;
    PLANE* Planes;
};

// Holds a Convex polygon as a Indexed Tri List
struct POLYGON {
    D3DLVERTEX* VertexList;
    D3DXVECTOR3 Normal;
    WORD NumberOfVertices;
    WORD NumberOfIndices;
    WORD* Indices;
    POLYGON* Next;
    bool BeenUsedAsSplitter;
    long TextureIndex;
};
// Basically the same as a polygon but with extra data.Can be safely cast
struct PORTAL // to a polygon for all splitting/clipping and classify functions
{
    D3DLVERTEX* VertexList;
    D3DXVECTOR3 Normal;
    WORD NumberOfVertices;
    WORD NumberOfIndices;
    WORD* Indices;
    PORTAL* Next;
    PORTAL* Prev;
    BYTE NumberOfLeafs;
    long LeafOwnerArray[4];
};

struct BOUNDINGBOX      // Holds the AABB of the BSP Nodes and Leafs.These are used to build
{                       // initial portals at each node and to perform frustum rejection on
    D3DXVECTOR3 BoxMin; // leafs
    D3DXVECTOR3 BoxMax;
};

// Contains where the start and end polygon in the PolygonArray.I initial allow
struct LEAF {                 // each leaf to have a maximum of 50 portals which is way to many but lets not
    long StartPolygon;        // take any chances.This memory is only wasteful during PVS Computing and would
    long EndPolygon;          // not be saved to a bsp file for runtime rendering.(Unless you wanted the
    long PortalIndexList[50]; // portal information)
    long NumberOfPortals;
    long PVSIndex; // Because the PVS is compressed this if the offset into the PVSData
                   // array that this leafs PVS Set begins
    BOUNDINGBOX BoundingBox;
};

struct NODE {
    unsigned char IsLeaf;
    unsigned long Plane;
    unsigned long Front;
    signed long Back;
    BOUNDINGBOX BoundingBox;
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
extern PORTAL** PortalArray;
extern BYTE* PVSData;

extern long NumberOfPolygons;
extern long NumberOfNodes;
extern long NumberOfLeafs;
extern long NumberOfPlanes;
extern long NumberOfPortals;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern LPDIRECT3D8 lpD3D;          // Used to create the D3DDevice
extern LPDIRECT3DDEVICE8 lpDevice; // Our rendering device

extern POLYGON* PolygonList; // This will contain the Master Linked List of polygons fed into
                             // the "Solid Leaf BSP Compiler	"

// extern LPDIRECTDRAWSURFACE7		lpTextureSurface[25];    //Surfaces for the textures
extern LPDIRECT3DTEXTURE8 lpTextureSurface[25];
extern long NumberOfTextures; // Number Of Textures

//********************************************************************************************
//
//                           FUNCTIONS
//
//*******************************************************************************************

// Contained in BSPTREE.CPP

void BuildBspTree(long Node, POLYGON* PolyList);
void RenderTree(D3DXVECTOR3 pos);
void DrawTree(long leaf);
long SelectBestSplitter(POLYGON* PolyList);
bool Get_Intersect(D3DXVECTOR3* linestart, D3DXVECTOR3* lineend, D3DXVECTOR3* vertex, D3DXVECTOR3* normal, D3DXVECTOR3* intersection,
                   float* percentage);

int ClassifyPoly(PLANE* plane, POLYGON* Poly);
int ClassifyPoint(D3DXVECTOR3* pos, PLANE* Plane);

POLYGON* AddPolygon(POLYGON* Parent, D3DLVERTEX* Vertices, WORD NumberOfVerts);
void SplitPolygon(POLYGON* Poly, PLANE* Plane, POLYGON* FrontSplit, POLYGON* BackSplit);
void CalculateBox(BOUNDINGBOX* Box, POLYGON* Polylist);
bool LineOfSight(D3DXVECTOR3* Start, D3DXVECTOR3* End, long Node);
bool CollideSphere(D3DXVECTOR3* SphereCenter, float SphereRadius, long Node);
// Contained in PORTAL.CPP

PORTAL* CalculateInitialPortal(long Node);
void BuildPortals();
PORTAL* ClipPortal(long Node, PORTAL* Portal);
void SplitPortal(PORTAL* Portal, PLANE* Plane, PORTAL* FrontSplit, PORTAL* BackSplit);

void DrawPortals();

bool CheckDuplicatePortal(PORTAL* CheckPortal, int* index);
void RemovePortalFromList(PORTAL* RemovePortal);
void GetPolygonBounds(POLYGON* Poly, D3DXVECTOR3* Min, D3DXVECTOR3* Max);

// Contained in PVS.CPP

long CalculatePVS();
void RecursePVS(long SourceLeaf, PORTAL* SrcPortal, PORTAL* TargetPortal, long TargetLeaf, BYTE* LeafPVS);
void SetPVSBit(BYTE* VisArray, long DestLeaf);
void CompressPVSDataArray(void);
long CompressLeafSet(BYTE* VisArray, long WritePos);

PLANE GetPortalPlane(PORTAL* Portal);
PORTAL* ClipToAntiPenumbra(PORTAL* SourcePortal, PORTAL* TargetPortal, PORTAL* GeneratorPortal);

// contained in MemAlloc.cpp
void ReserveInitialMemoryForArrays(void);

void IncreaseNumberOfPolygons(void);
void IncreaseNumberOfPlanes(void);
void IncreaseNumberOfNodes(void);
void IncreaseNumberOfPortals(void);
void IncreaseNumberOfLeafs(void);
void DeletePolygonArray(void);
void DeletePortalArray(void);
void DeletePortal(PORTAL* Portal);
void DeletePolygon(POLYGON* Poly);

extern long MAXNUMBEROFNODES;
extern long MAXNUMBEROFPLANES;
extern long MAXNUMBEROFPOLYGONS;
extern long MAXNUMBEROFPORTALS;
extern long MAXNUMBEROFLEAFS;

// Contained in Frustum.cpp

void ExtractFrustumPlanes(PLANE2* Planes);
bool LeafInFrustum(long Leaf);
