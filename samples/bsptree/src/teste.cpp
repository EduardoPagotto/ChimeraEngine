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

BYTE BSPMAP[] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // marca
                 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,  // marca
                 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,  // marca
                 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,  // marca
                 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,  // marca
                 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,  // marca
                 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1,  // marca
                 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 0, 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
                 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
                 0, 1, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,  // marca
                 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,  // marca
                 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,  // marca
                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // marca

void InitPolygons(void) {
    D3DLVERTEX VERTLIST[4][4];
    PolygonList = NULL; // this is a GLOBAL pointer
    POLYGON* child = NULL;
    int direction[4];
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 20; x++) {
            ZeroMemory(direction, sizeof(int) * 4);
            int offset = (y * 20) + x;
            // check what the digit is in the current map location
            if (BSPMAP[offset] != 0) {
                if (BSPMAP[offset] == 2) // North East Wall
                {
                    VERTLIST[0][0] =
                        D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 0);
                    VERTLIST[0][1] =
                        D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 0);
                    VERTLIST[0][2] =
                        D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 1);
                    VERTLIST[0][3] =
                        D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 1);
                    direction[0] = 1;
                }
                if (BSPMAP[offset] == 3) // North West Wall
                {
                    VERTLIST[0][0] =
                        D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 0);
                    VERTLIST[0][1] =
                        D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 0);
                    VERTLIST[0][2] =
                        D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f), RGB_MAKE(255, 255, 255), 0, 1, 1);
                    VERTLIST[0][3] =
                        D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f), RGB_MAKE(255, 255, 255), 0, 0, 1);
                    direction[0] = 1;
                }

                if (BSPMAP[offset] == 1) // Its a Standared wall
                {
                    if (x > 0) {
                        if (BSPMAP[offset - 1] == 0) // if theres nothing to the left add a left facing wall
                        {
                            VERTLIST[0][0] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 0);
                            VERTLIST[0][1] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 0);
                            VERTLIST[0][2] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 1);
                            VERTLIST[0][3] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 1);
                            direction[0] = 1;
                        }
                    }
                    if (x < 19) {
                        if (BSPMAP[offset + 1] == 0) // if there is nothing to the right add a right facing wall
                        {
                            VERTLIST[1][0] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 0);
                            VERTLIST[1][1] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 0);
                            VERTLIST[1][2] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 1);
                            VERTLIST[1][3] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 1);
                            direction[1] = 1;
                        }
                    }
                    if (y > 0) {
                        if (BSPMAP[offset - 20] == 0) // if there is nothing south add a south facing wall
                        {
                            VERTLIST[2][0] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 0);
                            VERTLIST[2][1] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 0);
                            VERTLIST[2][2] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 1);
                            VERTLIST[2][3] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) + 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 1);
                            direction[2] = 1;
                            ;
                        }
                    }
                    if (y < 39) {
                        if (BSPMAP[offset + 20] == 0) // if there is nothing to the north add a north facing wall
                        {
                            VERTLIST[3][0] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 3.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 0);
                            VERTLIST[3][1] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 3.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 0);
                            VERTLIST[3][2] = D3DLVERTEX(D3DVECTOR(x - 9.5f, 0.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 1, 1);
                            VERTLIST[3][3] = D3DLVERTEX(D3DVECTOR(x - 10.5f, 0.0f, (20.0f - y) - 0.5f),
                                                        RGB_MAKE(255, 255, 255), 0, 0, 1);
                            direction[3] = 1;
                            ;
                        }
                    }
                } // end for if offset==1

                // build the polygons

                for (int a = 0; a < 4; a++) {
                    if (direction[a] != 0) {
                        if (PolygonList == NULL) {
                            PolygonList = AddPolygon(NULL, &VERTLIST[a][0], 4);
                            child = PolygonList;
                        } else {
                            child = AddPolygon(child, &VERTLIST[a][0], 4);
                        }
                    } //
                }     ////
            }         // end for if offset!=0
        }
    }
    BSPTreeRootNode = new NODE;
    BuildBspTree(BSPTreeRootNode, PolygonList);
}

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

BSPTreeRootNode = new NODE;
BuildBspTree(BSPTreeRootNode, PolygonList);

int ClassifyPoint(D3DVECTOR* pos, POLYGON* Plane) {
    float result;
    D3DVECTOR* vec1 = (D3DVECTOR*)&Plane->VertexList[0];
    D3DVECTOR Direction = (*vec1) - (*pos);
    result = DotProduct(Direction, Plane->Normal);
    if (result < -0.001)
        return CP_FRONT;
    if (result > 0.001)
        return CP_BACK;
    return CP_ONPLANE;
}

int ClassifyPoly(POLYGON* Plane, POLYGON* Poly) {
    int Infront = 0;
    int Behind = 0;
    int OnPlane = 0;
    float result;
    D3DVECTOR* vec1 = (D3DVECTOR*)&Plane->VertexList[0];
    for (int a = 0; aNumberOfVertices; a++) {
        D3DVECTOR* vec2 = (D3DVECTOR*)&Poly->VertexList[a];
        D3DVECTOR Direction = (*vec1) - (*vec2);
        result = DotProduct(Direction, Plane->Normal);
        if (result > 0.001) {
            Behind++;
        } else if (result < -0.001) {
            Infront++;
        } else {
            OnPlane++;
            Infront++;
            Behind++;
        }
    }
    if (OnPlane == Poly->NumberOfVertices)
        return CP_FRONT; // this would nomrally be CP_ONPLANE
    if (Behind == Poly->NumberOfVertices)
        return CP_BACK;
    if (Infront == Poly->NumberOfVertices)
        return CP_FRONT;
    return CP_SPANNING;
}

void SplitPolygon(POLYGON* Poly, POLYGON* Plane, POLYGON* FrontSplit, POLYGON* BackSplit);

POLYGON* SelectBestSplitter(POLYGON* PolyList) {
    POLYGON* Splitter = PolyList;
    POLYGON* CurrentPoly = NULL;
    unsigned long BestScore = 100000; // just set to a very high value to begin
    POLYGON* SelectedPoly = NULL;
    while (Splitter != NULL) {
        CurrentPoly = PolyList;
        unsigned long score, splits, backfaces, frontfaces;
        score = splits = backfaces = frontfaces = 0;
        while (CurrentPoly != NULL) {
            if (CurrentPoly != Splitter) {
                int result = ClassifyPoly(Splitter, CurrentPoly);
                switch (result) {
                    case CP_ONPLANE:
                        break;
                    case CP_FRONT:
                        frontfaces++;
                        break;
                    case CP_BACK:
                        backfaces++;
                        break;
                    case CP_SPANNING:
                        splits++;
                        break;
                    default:
                        break;
                }
            }
            CurrentPoly = CurrentPoly->Next;
        } // end while current poly
        score = abs(frontfaces - backfaces) + (splits * 8);
        if (score < BestScore) {
            BestScore = score;
            SelectedPoly = Splitter;
        }
        Splitter = Splitter->Next;
    } // end while splitter == null
    return SelectedPoly;
}

void BuildBspTree(NODE* CurrentNode, POLYGON* PolyList) {
    POLYGON* polyTest = NULL;
    POLYGON* FrontList = NULL;
    POLYGON* BackList = NULL;
    POLYGON* NextPolygon = NULL;
    POLYGON* FrontSplit = NULL;
    POLYGON* BackSplit = NULL;
    D3DVECTOR vec1, vec2;
    CurrentNode->Splitter = SelectBestSplitter(PolyList);
    polyTest = PolyList;

    while (polyTest != NULL) {
        NextPolygon = polyTest->Next; // remember because polytest-> Next will be altered
        if (polyTest != CurrentNode->Splitter) {
            switch (ClassifyPoly(CurrentNode->Splitter, polyTest)) {

                case CP_FRONT:
                    polyTest->Next = FrontList;
                    FrontList = polyTest;
                    break;

                case CP_BACK:
                    polyTest->Next = BackList;
                    BackList = polyTest;
                    break;

                case CP_SPANNING:
                    FrontSplit = new POLYGON;
                    BackSplit = new POLYGON;
                    ZeroMemory(FrontSplit, sizeof(POLYGON));
                    ZeroMemory(BackSplit, sizeof(POLYGON));
                    SplitPolygon(polyTest, CurrentNode->Splitter, FrontSplit, BackSplit);
                    delete polyTest;
                    FrontSplit->Next = FrontList;
                    FrontList = FrontSplit;
                    BackSplit->Next = BackList;
                    BackList = BackSplit;
                    break;
                default:
                    break;
            }
        } // end if polypoint!=CurrentNodesplitter
        polyTest = NextPolygon;
    } // end while loop

    if (FrontList == NULL) {
        NODE* leafnode = new NODE;
        ZeroMemory(leafnode, sizeof(leafnode));
        leafnode->IsLeaf = true;
        leafnode->IsSolid = false;
        CurrentNode->Front = leafnode;
    } else {
        NODE* newnode = new NODE;
        ZeroMemory(newnode, sizeof(newnode));
        newnode->IsLeaf = false;
        CurrentNode->Front = newnode;
        BuildBspTree(newnode, FrontList);
    }
    if (BackList == NULL) {
        NODE* leafnode = new NODE;
        ZeroMemory(leafnode, sizeof(leafnode));
        leafnode->IsLeaf = true;
        leafnode->IsSolid = true;
        CurrentNode->Back = leafnode;
        ;
    } else {
        NODE* newnode = new NODE;
        ZeroMemory(newnode, sizeof(newnode));
        newnode->IsLeaf = false;
        CurrentNode->Back = newnode;
        BuildBspTree(newnode, BackList);
    }
} // end function

bool Get_Intersect(D3DVECTOR* linestart, D3DVECTOR* lineend, D3DVECTOR* vertex, D3DVECTOR* normal,
                   D3DVECTOR* intersection, float* percentage) {
    D3DVECTOR direction, L1;
    float linelength, dist_from_plane;

    direction.x = lineend->x - linestart->x;
    direction.y = lineend->y - linestart->y;
    direction.z = lineend->z - linestart->z;

    linelength = DotProduct(direction, *normal);

    if (fabsf(linelength) < 0.0001) {
        return false;
    }

    L1.x = vertex->x - linestart->x;
    L1.y = vertex->y - linestart->y;
    L1.z = vertex->z - linestart->z;

    dist_from_plane = DotProduct(L1, *normal);
    *percentage = dist_from_plane / linelength;

    if (*percentage < 0.0f) {
        return false;
    } else if (*percentage > 1.0f) {
        return false;
    }

    intersection->x = linestart->x + direction.x * (*percentage);
    intersection->y = linestart->y + direction.y * (*percentage);
    intersection->z = linestart->z + direction.z * (*percentage);
    return true;
}

void SplitPolygon(POLYGON* Poly, POLYGON* Plane, POLYGON* FrontSplit, POLYGON* BackSplit) {
    D3DLVERTEX FrontList[20], BackList[20], FirstVertex;
    D3DVECTOR PlaneNormal, IntersectPoint, PointOnPlane, PointA, PointB;
    WORD FrontCounter = 0, BackCounter = 0, loop = 0, CurrentVertex = 0;
    float percent;

    // Find any vertex on the plane to use later in plane intersect routine
    PointOnPlane = *(D3DVECTOR*)&Plane->VertexList[0];

    // first we find out if the first vertex belongs in front or back list
    FirstVertex = Poly->VertexList[0];
    switch (ClassifyPoint((D3DVECTOR*)&FirstVertex, Plane)) {
        case CP_FRONT:
            FrontList[FrontCounter++] = FirstVertex;
            break;
        case CP_BACK:
            BackList[BackCounter++] = FirstVertex;
            break;
        case CP_ONPLANE:
            BackList[BackCounter++] = FirstVertex;
            FrontList[FrontCounter++] = FirstVertex;
            break;
        default:
            break;
    }

    for (loop = 1; loop < Poly->NumberOfVertices + 1; loop++) {
        if (loop == Poly->NumberOfVertices) {
            CurrentVertex = 0;
        } else {
            CurrentVertex = loop;
        }
        PointA = *(D3DVECTOR*)&Poly->VertexList[loop - 1];
        PointB = *(D3DVECTOR*)&Poly->VertexList[CurrentVertex];

        PlaneNormal = Plane->Normal;
        int Result = ClassifyPoint(&PointB, Plane);
        if (Result == CP_ONPLANE) {
            BackList[BackCounter++] = Poly->VertexList[CurrentVertex];
            FrontList[FrontCounter++] = Poly->VertexList[CurrentVertex];
        } else {

            if (Get_Intersect(&PointA, &PointB, &PointOnPlane, &PlaneNormal, &IntersectPoint, &percent) == true) {

                float deltax, deltay, texx, texy;
                deltax = Poly->VertexList[CurrentVertex].tu - Poly->VertexList[loop - 1].tu;
                deltay = Poly->VertexList[CurrentVertex].tv - Poly->VertexList[loop - 1].tv;
                texx = Poly->VertexList[loop - 1].tu + (deltax * percent);
                texy = Poly->VertexList[loop - 1].tv + (deltay * percent);
                D3DLVERTEX copy = D3DLVERTEX(IntersectPoint, RGB_MAKE(255, 255, 255), 0, texx, texy);

                if (Result == CP_FRONT) {
                    BackList[BackCounter++] = copy;
                    FrontList[FrontCounter++] = copy;
                    if (CurrentVertex != 0) {
                        FrontList[FrontCounter++] = Poly->VertexList[CurrentVertex];
                    }
                }

                if (Result == CP_BACK) {
                    FrontList[FrontCounter++] = copy;
                    BackList[BackCounter++] = copy;
                    if (CurrentVertex != 0) {
                        BackList[BackCounter++] = Poly->VertexList[CurrentVertex];
                    }
                }

            } // end if intersection (get intersect==true)

            else {
                if (Result == CP_FRONT) {
                    if (CurrentVertex != 0) {
                        FrontList[FrontCounter++] = Poly->VertexList[CurrentVertex];
                    }
                }

                if (Result == CP_BACK) {
                    if (CurrentVertex != 0) {
                        BackList[BackCounter++] = Poly->VertexList[CurrentVertex];
                    }
                }
            }
        }
    } // end loop through each edge

    // OK THEN LETS BUILD THESE TWO POLYGONAL BAD BOYS

    FrontSplit->NumberOfVertices = 0;
    BackSplit->NumberOfVertices = 0;

    for (loop = 0; loop < FrontCounter; loop++) {
        FrontSplit->NumberOfVertices++;
        FrontSplit->VertexList[loop] = FrontList[loop];
    }

    for (loop = 0; loop < BackCounter; loop++) {
        BackSplit->NumberOfVertices++;
        BackSplit->VertexList[loop] = BackList[loop];
    }

    BackSplit->NumberOfIndices = (BackSplit->NumberOfVertices - 2) * 3;
    FrontSplit->NumberOfIndices = (FrontSplit->NumberOfVertices - 2) * 3;

    // Fill in the Indices Array

    WORD v0, v1, v2;
    for (loop = 0; loop < FrontSplit->NumberOfIndices / 3; loop++) {
        if (loop == 0) {
            v0 = 0;
            v1 = 1;
            v2 = 2;
        } else {
            v1 = v2;
            v2++;
        }
        FrontSplit->Indices[loop * 3] = v0;
        FrontSplit->Indices[(loop * 3) + 1] = v1;
        FrontSplit->Indices[(loop * 3) + 2] = v2;
    }

    for (loop = 0; loop < BackSplit->NumberOfIndices / 3; loop++) {
        if (loop == 0) {
            v0 = 0;
            v1 = 1;
            v2 = 2;
        } else {
            v1 = v2;
            v2++;
        }
        BackSplit->Indices[loop * 3] = v0;
        BackSplit->Indices[(loop * 3) + 1] = v1;
        BackSplit->Indices[(loop * 3) + 2] = v2;
    }

    // calculate polygon Normals

    D3DVECTOR edge1, edge2;
    edge1 = *(D3DVECTOR*)&FrontSplit->VertexList[FrontSplit->Indices[1]] -
            *(D3DVECTOR*)&FrontSplit->VertexList[FrontSplit->Indices[0]];

    edge2 = *(D3DVECTOR*)&FrontSplit->VertexList[FrontSplit->Indices[FrontSplit->NumberOfIndices - 1]] -
            *(D3DVECTOR*)&FrontSplit->VertexList[FrontSplit->Indices[0]];

    FrontSplit->Normal = CrossProduct(edge1, edge2);
    FrontSplit->Normal = Normalize(FrontSplit->Normal);

    edge1 = *(D3DVECTOR*)&BackSplit->VertexList[BackSplit->Indices[1]] -
            *(D3DVECTOR*)&BackSplit->VertexList[BackSplit->Indices[0]];

    edge2 = *(D3DVECTOR*)&BackSplit->VertexList[BackSplit->Indices[BackSplit->NumberOfIndices - 1]] -
            *(D3DVECTOR*)&BackSplit->VertexList[BackSplit->Indices[0]];

    BackSplit->Normal = CrossProduct(edge1, edge2);
    BackSplit->Normal = Normalize(BackSplit->Normal);
}

void WalkBspTree(NODE* Node, D3DVECTOR* pos) {
    if (Node->IsLeaf == true)
        return;
    int result = ClassifyPoint(pos, Node->Splitter);
    if (result == CP_FRONT) {
        if (Node->Back != NULL)
            WalkBspTree(Node->Back, pos);
        lpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, &Node->Splitter->VertexList[0],
                                       Node->Splitter->NumberOfVertices, &Node->Splitter->Indices[0],
                                       Node->Splitter->NumberOfIndices, NULL);
        if (Node->Front != NULL)
            WalkBspTree(Node->Front, pos);
        return;
    } // this happens if we are at back or on plane
    if (Node->Front != NULL)
        WalkBspTree(Node->Front, pos);
    if (Node->Back != NULL)
        WalkBspTree(Node->Back, pos);
    return;
}

bool LineOfSight(D3DVECTOR* Start, D3DVECTOR* End, NODE* Node) {
    float temp;
    D3DVECTOR intersection;
    if (Node->IsLeaf == true) {
        return !Node->IsSolid;
    }

    int PointA = ClassifyPoint(Start, Node->Splitter);
    int PointB = ClassifyPoint(End, Node->Splitter);

    if (PointA == CP_ONPLANE && PointB == CP_ONPLANE) {
        return LineOfSight(Start, End, Node->Front);
    }

    if (PointA == CP_FRONT && PointB == CP_BACK) {
        Get_Intersect(Start, End, (D3DVECTOR*)&Node->Splitter->VertexList[0], &Node->Splitter->Normal, &intersection,
                      &temp);
        return LineOfSight(Start, &intersection, Node->Front) && LineOfSight(End, &intersection, Node->Back);
    }

    if (PointA == CP_BACK && PointB == CP_FRONT) {
        Get_Intersect(Start, End, (D3DVECTOR*)&Node->Splitter->VertexList[0], &Node->Splitter->Normal, &intersection,
                      &temp);
        return LineOfSight(End, &intersection, Node->Front) && LineOfSight(Start, &intersection, Node->Back);
    }

    // if we get here one of the points is on the plane
    if (PointA == CP_FRONT || PointB == CP_FRONT) {
        return LineOfSight(Start, End, Node->Front);
    } else {
        return LineOfSight(Start, End, Node->Back);
    }
    return true;
}