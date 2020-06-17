
#include "pvs_bsp.h"
#include <malloc.h>
#include <stdlib.h>

long MAXNUMBEROFNODES = 100;
long MAXNUMBEROFPOLYGONS = 100;
long MAXNUMBEROFPLANES = 100;
long MAXNUMBEROFLEAFS = 100;
long MAXNUMBEROFPORTALS = 100;

// These are the Master Arrays that will contain the final tree

POLYGON* PolygonArray;
NODE* NodeArray;
LEAF* LeafArray;
PLANE* PlaneArray;
PORTAL** PortalArray;
BYTE* PVSData;

long NumberOfPolygons = 0;
long NumberOfNodes = 0;
long NumberOfLeafs = 0;
long NumberOfPlanes = 0;
long NumberOfPortals = 0;

long BytesPerSet; // will hold how many bytes are needed to hold a PVS for one leaf

//   **********************MEMORY ALLOCATION FUNCTIONS ************************************

void ReserveInitialMemoryForArrays() {
    NodeArray = (NODE*)malloc(MAXNUMBEROFNODES * sizeof(NODE));
    PolygonArray = (POLYGON*)malloc(MAXNUMBEROFPOLYGONS * sizeof(POLYGON));
    PlaneArray = (PLANE*)malloc(MAXNUMBEROFPLANES * sizeof(PLANE));
    LeafArray = (LEAF*)malloc(MAXNUMBEROFLEAFS * sizeof(LEAF));
    PortalArray = (PORTAL**)malloc(MAXNUMBEROFPORTALS * sizeof(PORTAL*));

    ZeroMemory(NodeArray, sizeof(NODE) * MAXNUMBEROFNODES);
    ZeroMemory(LeafArray, sizeof(LEAF) * MAXNUMBEROFLEAFS);
    ZeroMemory(PlaneArray, sizeof(PLANE) * MAXNUMBEROFPLANES);
    ZeroMemory(PolygonArray, sizeof(POLYGON) * MAXNUMBEROFPOLYGONS);
    ZeroMemory(PortalArray, sizeof(PORTAL*) * MAXNUMBEROFPORTALS);
}

//************************************************************************

void IncreaseNumberOfNodes(void) {
    NumberOfNodes++;
    if (NumberOfNodes == MAXNUMBEROFNODES) {
        MAXNUMBEROFNODES += 100;
        NodeArray = (NODE*)realloc(NodeArray, MAXNUMBEROFNODES * sizeof(NODE));
    }
}

//************************************************************************

void IncreaseNumberOfPolygons(void) {
    NumberOfPolygons++;
    if (NumberOfPolygons == MAXNUMBEROFPOLYGONS) {
        MAXNUMBEROFPOLYGONS += 100;
        PolygonArray = (POLYGON*)realloc(PolygonArray, MAXNUMBEROFPOLYGONS * sizeof(POLYGON));
    }
}

//************************************************************************

void IncreaseNumberOfPlanes(void) {
    NumberOfPlanes++;
    if (NumberOfPlanes == MAXNUMBEROFPLANES) {
        MAXNUMBEROFPLANES += 100;
        PlaneArray = (PLANE*)realloc(PlaneArray, MAXNUMBEROFPLANES * sizeof(PLANE));
    }
}

//************************************************************************

void IncreaseNumberOfLeafs(void) {
    NumberOfLeafs++;
    if (NumberOfLeafs == MAXNUMBEROFLEAFS) {
        MAXNUMBEROFLEAFS += 100;
        LeafArray = (LEAF*)realloc(LeafArray, MAXNUMBEROFLEAFS * sizeof(LEAF));
    }
}

//************************************************************************

void IncreaseNumberOfPortals(void) {
    NumberOfPortals++;
    if (NumberOfPortals == MAXNUMBEROFPORTALS) {
        MAXNUMBEROFPORTALS += 100;
        PortalArray = (PORTAL**)realloc(PortalArray, MAXNUMBEROFPORTALS * sizeof(PORTAL*));
    }
}

//*************************************************************************

void DeletePortalArray(void) {
    for (int a = 0; a < NumberOfPortals; a++) {
        DeletePortal(PortalArray[a]);
    }
    free(PortalArray);
}

//**************************************************************************
void DeletePolygonArray(void) {
    for (long i = 0; i < NumberOfPolygons; i++) {
        delete PolygonArray[i].VertexList;
        delete PolygonArray[i].Indices;
    }
    free(PolygonArray);
}

//*************************************************************************

void DeletePolygon(POLYGON* Poly) {
    delete[] Poly->VertexList;
    delete[] Poly->Indices;
    delete Poly;
}

void DeletePortal(PORTAL* Portal) {
    delete[] Portal->VertexList;
    delete[] Portal->Indices;
    delete Portal;
}

//*************************************************************************
