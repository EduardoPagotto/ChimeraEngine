// ref https://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html
// ref novo: https://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm

struct POLYGON {
    D3DLVERTEX VertexList[10];
    D3DVECTOR Normal;
    WORD NumberOfVertices;
    WORD NumberOfIndices;
    WORD Indices[30];
    POLYGON* Next;
};

struct NODE {
    POLYGON* Splitter;
    NODE* Front;
    NODE* Back;
    BOOL IsLeaf;
    BOOL IsSolid;
};

// BYTE BSPMAP[] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // marca
//                  0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,  // marca
//                  0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,  // marca
//                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,  // marca
//                  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,  // marca
//                  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,  // marca
//                  0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1,  // marca
//                  0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  0, 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
//                  0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
//                  0, 1, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
//                  1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,  // marca
//                  1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
//                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // marca

// void InitPolygons(void) {
//     D3DLVERTEX VERTLIST[4][4];
//     PolygonList = NULL; // this is a GLOBAL pointer
//     POLYGON* child = NULL;
//     int direction[4];
//     for (int y = 0; y < 40; y++) {
//         for (int x = 0; x < 20; x++) {
//             ZeroMemory(direction, sizeof(int) * 4);
//             int offset = (y * 20) + x;
//             // check what the digit is in the current map location
//             if (BSPMAP[offset] != 0) {
//                 if (BSPMAP[offset] == 2) // North East Wall
//                 {
//                     VERTLIST[0][0] =
//                         D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 0);
//                     VERTLIST[0][1] =
//                         D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 0);
//                     VERTLIST[0][2] =
//                         D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 1);
//                     VERTLIST[0][3] =
//                         D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 1);
//                     direction[0] = 1;
//                 }
//                 if (BSPMAP[offset] == 3) // North West Wall
//                 {
//                     VERTLIST[0][0] =
//                         D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 0);
//                     VERTLIST[0][1] =
//                         D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 0);
//                     VERTLIST[0][2] =
//                         D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 1);
//                     VERTLIST[0][3] =
//                         D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 1);
//                     direction[0] = 1;
//                 }

//                 if (BSPMAP[offset] == 1) // Its a Standared wall
//                 {
//                     if (x > 0) {
//                         if (BSPMAP[offset - 1] == 0) // if theres nothing to the left add a left facing wall
//                         {
//                             VERTLIST[0][0] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 0);
//                             VERTLIST[0][1] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 0);
//                             VERTLIST[0][2] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 1);
//                             VERTLIST[0][3] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 1);
//                             direction[0] = 1;
//                         }
//                     }
//                     if (x < 19) {
//                         if (BSPMAP[offset + 1] == 0) // if there is nothing to the right add a right facing wall
//                         {
//                             VERTLIST[1][0] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 0);
//                             VERTLIST[1][1] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 0);
//                             VERTLIST[1][2] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 1);
//                             VERTLIST[1][3] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 1);
//                             direction[1] = 1;
//                         }
//                     }
//                     if (y > 0) {
//                         if (BSPMAP[offset - 20] == 0) // if there is nothing south add a south facing wall
//                         {
//                             VERTLIST[2][0] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 0);
//                             VERTLIST[2][1] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 0);
//                             VERTLIST[2][2] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 1);
//                             VERTLIST[2][3] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 1);
//                             direction[2] = 1;
//                             ;
//                         }
//                     }
//                     if (y < 39) {
//                         if (BSPMAP[offset + 20] == 0) // if there is nothing to the north add a north facing wall
//                         {
//                             VERTLIST[3][0] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 0);
//                             VERTLIST[3][1] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 0);
//                             VERTLIST[3][2] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 1, 1);
//                             VERTLIST[3][3] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f),
//                                                         RGB_MAKE(255, 255, 255), 0, 0, 1);
//                             direction[3] = 1;
//                             ;
//                         }
//                     }
//                 } // end for if offset==1

//                 // build the polygons

//                 for (int a = 0; a < 4; a++) {
//                     if (direction[a] != 0) {
//                         if (PolygonList == NULL) {
//                             PolygonList = AddPolygon(NULL, &VERTLIST[a][0], 4);
//                             child = PolygonList;
//                         } else {
//                             child = AddPolygon(child, &VERTLIST[a][0], 4);
//                         }
//                     } //
//                 }     ////
//             }         // end for if offset!=0
//         }
//     }
//     BSPTreeRootNode = new NODE;
//     BuildBspTree(BSPTreeRootNode, PolygonList);
// }

POLYGON* AddPolygon(POLYGON* Parent, D3DLVERTEX* Vertices, WORD NOV) {
    int loop;
    POLYGON* Child = new POLYGON;
    Child->NumberOfVertices = NOV;
    Child->NumberOfIndices = (NOV - 2) * 3;
    Child->Next = NULL;
    for (loop = 0; loop < NOV; loop++) {
        Child->VertexList[loop] = Vertices[loop];
    } // calculate indices
    WORD v0, v1, v2;
    for (loop = 0; loop < Child->NumberOfIndices / 3; loop++) {
        if (loop == 0) {
            v0 = 0;
            v1 = 1;
            v2 = 2;
        } else {
            v1 = v2;
            v2++;
        }
        Child->Indices[loop * 3] = v0;
        Child->Indices[(loop * 3) + 1] = v1;
        Child->Indices[(loop * 3) + 2] = v2;
    } // generate polygon normal
    D3DVECTOR* vec0 = (D3DVECTOR*)&Child->VertexList[0];
    D3DVECTOR* vec1 = (D3DVECTOR*)&Child->VertexList[1];
    D3DVECTOR* vec2 = (D3DVECTOR*)&Child->VertexList[Child->NumberOfVertices - 1]; // the last vert
    D3DVECTOR edge1 = (*vec1) - (*vec0);
    D3DVECTOR edge2 = (*vec2) - (*vec0);
    Child->Normal = CrossProduct(edge1, edge2);
    Child->Normal = Normalize(Child->Normal);
    if (Parent != NULL) {
        Parent->Next = Child;
    }
    return Child;
}
