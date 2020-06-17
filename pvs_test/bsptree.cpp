#include "pvs_bsp.h"

POLYGON* pTexturePolygonList[25]; // Polygons will be selected from their leaves each frame
                                  // and added to the polygon list of the corresponding texture
                                  // this allows us to batch the polygons to minimize texture changes

//*******************************************************************************
//                      BUILD BSP TREE
//
// No prizes for guessing what this function does
//
//*****************************************************************************

void BuildBspTree(long Node, POLYGON* PolyList) {

    POLYGON* polyTest = NULL;
    POLYGON* FrontList = NULL;
    POLYGON* BackList = NULL;
    POLYGON* NextPolygon = NULL;
    POLYGON* FrontSplit = NULL;
    POLYGON* BackSplit = NULL;
    D3DXVECTOR3 vec1, vec2;
    D3DXVECTOR3 a, b;
    float result;

    NodeArray[Node].Plane = SelectBestSplitter(PolyList);
    polyTest = PolyList;
    // set up dummy bounding boxes for the node so it can be altered
    NodeArray[Node].BoundingBox.BoxMax = D3DXVECTOR3(-40000, -40000, -40000);
    NodeArray[Node].BoundingBox.BoxMin = D3DXVECTOR3(40000, 40000, 40000);

    while (polyTest != NULL) {
        NextPolygon = polyTest->Next; // remember because polytest->Next will be altered

        switch (ClassifyPoly(&PlaneArray[NodeArray[Node].Plane], polyTest)) {
            case CP_ONPLANE:

                a = PlaneArray[NodeArray[Node].Plane].Normal;
                b = polyTest->Normal;
                result = (float)fabs((a.x - b.x) + (a.y - b.y) + (a.z - b.z));
                if (result < 0.1) {
                    polyTest->Next = FrontList;
                    FrontList = polyTest;
                }

                else

                {
                    polyTest->Next = BackList;
                    BackList = polyTest;
                }

                break;

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
                SplitPolygon(polyTest, &PlaneArray[NodeArray[Node].Plane], FrontSplit, BackSplit);
                FrontSplit->BeenUsedAsSplitter = polyTest->BeenUsedAsSplitter;
                BackSplit->BeenUsedAsSplitter = polyTest->BeenUsedAsSplitter;
                FrontSplit->TextureIndex = polyTest->TextureIndex;
                BackSplit->TextureIndex = polyTest->TextureIndex;

                DeletePolygon(polyTest);

                FrontSplit->Next = FrontList;
                FrontList = FrontSplit;
                BackSplit->Next = BackList;
                BackList = BackSplit;
                break;
            default:
                break;

        } // switch
        polyTest = NextPolygon;
    } // end while loop

    int count = 0;
    POLYGON* tempf = FrontList;
    while (tempf != NULL) {
        if (tempf->BeenUsedAsSplitter == false)
            count++;
        tempf = tempf->Next;
    }

    CalculateBox(&NodeArray[Node].BoundingBox, FrontList);
    BOUNDINGBOX LeafBox = NodeArray[Node].BoundingBox;
    CalculateBox(&NodeArray[Node].BoundingBox, BackList);

    if (count == 0) {
        POLYGON* Iterator = FrontList;
        POLYGON* Temp;
        LeafArray[NumberOfLeafs].StartPolygon = NumberOfPolygons;

        while (Iterator != NULL) {
            PolygonArray[NumberOfPolygons] = *Iterator;
            IncreaseNumberOfPolygons();
            Temp = Iterator;
            Iterator = Iterator->Next;
            delete Temp; // we use the normal delete operator here because we dont want to delete the actual vertexLists and index lists
                         // because we are still referenceing them in the Polygon Array
        }
        LeafArray[NumberOfLeafs].EndPolygon = NumberOfPolygons;
        LeafArray[NumberOfLeafs].BoundingBox = LeafBox;
        NodeArray[Node].Front = NumberOfLeafs;
        NodeArray[Node].IsLeaf = 1;
        IncreaseNumberOfLeafs();
    } else {
        NodeArray[Node].IsLeaf = 0;
        NodeArray[Node].Front = NumberOfNodes + 1;
        IncreaseNumberOfNodes();
        BuildBspTree(NumberOfNodes, FrontList);
    }

    if (BackList == NULL) {
        NodeArray[Node].Back = -1;

    } else

    {
        NodeArray[Node].Back = NumberOfNodes + 1;
        IncreaseNumberOfNodes();
        BuildBspTree(NumberOfNodes, BackList);
    }

} // end function

//***************** Classify a Polygon with Respect to a Plane ******************************
//

int ClassifyPoly(PLANE* Plane, POLYGON* Poly) {
    int Infront = 0;
    int Behind = 0;
    int OnPlane = 0;
    float result;
    D3DXVECTOR3* vec1 = (D3DXVECTOR3*)&Plane->PointOnPlane;

    for (int a = 0; a < Poly->NumberOfVertices; a++) {
        D3DXVECTOR3* vec2 = (D3DXVECTOR3*)&Poly->VertexList[a];
        D3DXVECTOR3 Direction = (*vec1) - (*vec2);
        result = D3DXVec3Dot(&Direction, &Plane->Normal);

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
        return CP_ONPLANE;
    if (Behind == Poly->NumberOfVertices)
        return CP_BACK;
    if (Infront == Poly->NumberOfVertices)
        return CP_FRONT;
    return CP_SPANNING;
}

//***************************************************************************************
//
//           CLASSIFY A POINT WITH RESPECT TO A PLANE
//****************************************************************************************

int ClassifyPoint(D3DXVECTOR3* pos, PLANE* Plane) {
    float result;
    D3DXVECTOR3* vec1 = (D3DXVECTOR3*)&Plane->PointOnPlane;
    D3DXVECTOR3 Direction = (*vec1) - (*pos);
    result = D3DXVec3Dot(&Direction, &Plane->Normal);
    if (result < -0.001)
        return CP_FRONT;
    if (result > 0.001)
        return CP_BACK;

    return CP_ONPLANE;
}

//************************************************************************************
//
//                     RENDER TREE
//
// Simply traverses the tree to find the current leaf the camera in. Once the leaf is found
// we call DrawTree to draw this leafs PVS Set
//
//*************************************************************************************

void RenderTree(D3DXVECTOR3 pos) {

    int Node = 0;
    int leaf = 0;
    while (1) {
        switch (ClassifyPoint(&pos, &PlaneArray[NodeArray[Node].Plane])) {
            case CP_FRONT:
                if (NodeArray[Node].IsLeaf != 0) {
                    leaf = NodeArray[Node].Front;
                    DrawTree(leaf);
                    return;
                } else {
                    Node = NodeArray[Node].Front;
                }
                break;
            case CP_BACK:
                if (NodeArray[Node].Back == -1) {
                    return;
                } else {
                    Node = NodeArray[Node].Back;
                }
                break;
            case CP_ONPLANE:
                if (NodeArray[Node].IsLeaf != 0) {
                    leaf = NodeArray[Node].Front;
                    DrawTree(leaf);
                    return;
                } else {
                    Node = NodeArray[Node].Front;
                }
                break;
            default:
                break;
        }
    } // end while
} // End Function

//******************************************************************************************
//
//                      DRAW TREE
//
// This the function that actually Renders the entire PVS Set.Notice that the PVS set is
// BIT compressed and uses Zero Length Encoding to keep the size of the PVS Set down.
//
//******************************************************************************************

void DrawTree(long leaf) {
    // DRaw the PVS Set
    POLYGON* CurrentPoly;
    int i;
    long PVSOFFSET = LeafArray[leaf].PVSIndex;
    BYTE* PVSPointer = PVSData;
    PVSPointer += PVSOFFSET;
    long currentleaf = 0;

    // Set All the Texture Batch Pointers to NULL
    for (i = 0; i < 25; i++) {
        pTexturePolygonList[i] = NULL;
    }

    while (currentleaf < NumberOfLeafs) {
        if (*PVSPointer != 0) {
            for (i = 0; i < 8; i++) {
                BYTE mask = 1 << i;
                BYTE pvs = *PVSPointer;
                if (pvs & mask) {

                    if (LeafInFrustum(currentleaf) == true || DontFrustumReject == true) {
                        unsigned long start = LeafArray[currentleaf].StartPolygon;
                        unsigned long finish = LeafArray[currentleaf].EndPolygon;
                        unsigned long Count = 0;

                        for (Count = start; Count < finish;
                             Count++) // Collect the polys from this leaf and add them to the correct Texture List
                        {
                            CurrentPoly = &PolygonArray[Count];
                            CurrentPoly->Next = pTexturePolygonList[CurrentPoly->TextureIndex];
                            pTexturePolygonList[CurrentPoly->TextureIndex] = CurrentPoly;
                        }
                    } // end if leaf infrustum

                } // end for if pvsdata
                currentleaf++;
            } // end for i;
            PVSPointer++;
        } // if pvspointer!=0; In other words if this is not a compressed byte

        else

        { // we have hit a zero so read in the next byte and see how long the run of zeros is
            PVSPointer++;
            BYTE RunLength = *PVSPointer;
            PVSPointer++;
            currentleaf += RunLength * 8;
        }

    } // end for while

    // We now have all the polygons in linked lists grouped by texture.So just set each texture and render all the polygons
    // in that textures list.

    for (i = 0; i < NumberOfTextures; i++) {
        if (pTexturePolygonList[i] != NULL) {
            lpDevice->SetTexture(0, lpTextureSurface[i]);
            CurrentPoly = pTexturePolygonList[i];

            while (CurrentPoly != NULL) {
                // lpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX,&CurrentPoly->VertexList[0],CurrentPoly->NumberOfVertices,&CurrentPoly->Indices[0],CurrentPoly->NumberOfIndices,NULL);
                lpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, CurrentPoly->NumberOfVertices, CurrentPoly->NumberOfIndices / 3,
                                                 &CurrentPoly->Indices[0], D3DFMT_INDEX16, &CurrentPoly->VertexList[0], sizeof(D3DLVERTEX));
                CurrentPoly = CurrentPoly->Next;
            }
        }
    }

    lpDevice->SetTexture(0, NULL);

} // End Function

//**********************************************************************************************8
//
//                          GET INTERSECT
//
// Returns the intersection point between a RAY and a PLANE
//
//**********************************************************************************************

bool Get_Intersect(D3DXVECTOR3* linestart, D3DXVECTOR3* lineend, D3DXVECTOR3* vertex, D3DXVECTOR3* normal, D3DXVECTOR3* intersection,
                   float* percentage)

{
    D3DXVECTOR3 direction, L1;
    float linelength, dist_from_plane;

    direction.x = lineend->x - linestart->x;
    direction.y = lineend->y - linestart->y;
    direction.z = lineend->z - linestart->z;

    linelength = D3DXVec3Dot(&direction, normal);
    if (fabsf(linelength) < 0.0001) {
        return false;
    }

    L1.x = vertex->x - linestart->x;
    L1.y = vertex->y - linestart->y;
    L1.z = vertex->z - linestart->z;

    dist_from_plane = D3DXVec3Dot(&L1, normal);

    *percentage = dist_from_plane / linelength; // How far from Linestart , intersection is as a percentage of 0 to 1

    if (*percentage < 0.0f) // The plane is behind the start of the line
    {
        return false;
    }

    else

        if (*percentage > 1.0f) // The line does not reach the plane
    {
        return false;
    }

    else

        intersection->x = linestart->x + direction.x * (*percentage); // add the percentage of the line to line start
    intersection->y = linestart->y + direction.y * (*percentage);
    intersection->z = linestart->z + direction.z * (*percentage);
    return true;
}

//**************************************************************************************
//
//                             SPLIT POLYGON
//
// This function is used to do ALL the clipping and Splitting of portals and polygons.It
// takes a polygon POLY and a Plane PLANE and splits the polygon into to two seperate polygons.
// PLease nore that FRONT and BACK MUST be valid pointers to empty Polygon structures as this
// function does NOT allocate the memory for them.The reason for this is that this function is
// used in so many cases and some of them required the Front and Back already be initialized.
// When used for clipping to a plane, i call this function and then simple discard the front or back
// depending on your needs.

//**************************************************************************************

void SplitPolygon(POLYGON* Poly, PLANE* Plane, POLYGON* FrontSplit, POLYGON* BackSplit) {

    D3DLVERTEX FrontList[40], BackList[40], FirstVertex;
    D3DXVECTOR3 PlaneNormal, IntersectPoint, PointOnPlane, PointA, PointB;
    WORD FrontCounter = 0, BackCounter = 0, loop = 0, CurrentVertex = 0;
    float percent; // used for return storage of percentage

    // Find any vertex on the plane to use later in plane intersect routine
    PointOnPlane = Plane->PointOnPlane;

    // first we find out if the first vertex belongs in front or back list
    FirstVertex = Poly->VertexList[0];

    switch (ClassifyPoint((D3DXVECTOR3*)&FirstVertex, Plane)) {
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
            PostQuitMessage(0);
            ;
    }

    for (loop = 1; loop < Poly->NumberOfVertices + 1; loop++) {

        if (loop == Poly->NumberOfVertices) {
            CurrentVertex = 0;
        } else {
            CurrentVertex = loop;
        }
        PointA = *(D3DXVECTOR3*)&Poly->VertexList[loop - 1];
        PointB = *(D3DXVECTOR3*)&Poly->VertexList[CurrentVertex];

        PlaneNormal = Plane->Normal;

        int Result = ClassifyPoint(&PointB, Plane);
        if (Result == CP_ONPLANE) {
            BackList[BackCounter++] = Poly->VertexList[CurrentVertex];
            FrontList[FrontCounter++] = Poly->VertexList[CurrentVertex];
        } else {
            if (Get_Intersect(&PointA, &PointB, &PointOnPlane, &PlaneNormal, &IntersectPoint, &percent) == true) {

                // create new vertex and calculate new texture coordinate
                float deltax, deltay, texx, texy;
                deltax = Poly->VertexList[CurrentVertex].tu - Poly->VertexList[loop - 1].tu;
                deltay = Poly->VertexList[CurrentVertex].tv - Poly->VertexList[loop - 1].tv;
                texx = Poly->VertexList[loop - 1].tu + (deltax * percent);
                texy = Poly->VertexList[loop - 1].tv + (deltay * percent);
                D3DLVERTEX copy; //=D3DLVERTEX(IntersectPoint,D3DCOLOR_ARGB(255,255,255,255),0,texx,texy);
                copy.x = IntersectPoint.x;
                copy.y = IntersectPoint.y;
                copy.z = IntersectPoint.z;
                copy.color = D3DCOLOR_ARGB(255, 255, 255, 255);
                copy.specular = 0;
                copy.tu = texx;
                copy.tv = texy;

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

            } // end get intersect

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
        } // end for not onplane
    }     // end for loop

    // OK THEN LETS BUILD THESE TWO POLYGONAL BAD BOYS

    FrontSplit->NumberOfVertices = 0;
    BackSplit->NumberOfVertices = 0;

    // Reserve Memory for Front and Back Vertex Lists
    FrontSplit->VertexList = new D3DLVERTEX[FrontCounter];
    BackSplit->VertexList = new D3DLVERTEX[BackCounter];

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

    // Reserve Memory for Front and Back Index Lists

    FrontSplit->Indices = new WORD[FrontSplit->NumberOfIndices];
    BackSplit->Indices = new WORD[BackSplit->NumberOfIndices];

    // Fill in the Indices
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
    FrontSplit->Normal = Poly->Normal;
    BackSplit->Normal = Poly->Normal;
}

//***************************************************************************************
//
//                               ADD POLYGON
//
// Takes any convex Polygon and breaks in into multiple Indexed Triangle Lists and adds the
// polygon to a Linked list that will be sent to the BSP Compiler.
//
//****************************************************************************************

POLYGON* AddPolygon(POLYGON* Parent, D3DLVERTEX* Vertices, WORD NOV) {
    int loop;
    POLYGON* Child = new POLYGON;
    Child->NumberOfVertices = NOV;
    Child->NumberOfIndices = (NOV - 2) * 3;
    Child->Next = NULL;
    Child->BeenUsedAsSplitter = false;

    // Reserve space for Vertex and Index Lists
    Child->VertexList = new D3DLVERTEX[Child->NumberOfVertices];
    Child->Indices = new WORD[Child->NumberOfIndices];

    for (loop = 0; loop < NOV; loop++) {
        Child->VertexList[loop] = Vertices[loop];
    }

    // calculate indices
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
    }

    // generate polygon normal
    D3DXVECTOR3* vec0 = (D3DXVECTOR3*)&Child->VertexList[0];
    D3DXVECTOR3* vec1 = (D3DXVECTOR3*)&Child->VertexList[1];
    D3DXVECTOR3* vec2 = (D3DXVECTOR3*)&Child->VertexList[Child->NumberOfVertices - 1]; // the last vert

    D3DXVECTOR3 edge1 = (*vec1) - (*vec0);
    D3DXVECTOR3 edge2 = (*vec2) - (*vec0);
    D3DXVec3Cross(&Child->Normal, &edge1, &edge2);
    D3DXVec3Normalize(&Child->Normal, &Child->Normal);

    if (Parent != NULL) {
        Parent->Next = Child;
    }

    return Child;
}

//****************************************************************************************
//
//                              SELECT BEST SPLITTER
//
// Chooses the next polygon in the list to be used as the Splitting Node of the BSP.Note you
// can changes the Splitting heuristic from Splits*2 to other values depending on whether
// you need fewer splits or a more balanced tree.
//
//***********************************************************************

long SelectBestSplitter(POLYGON* PolyList) {
    POLYGON* Splitter = PolyList;
    POLYGON* CurrentPoly = NULL;
    unsigned long BestScore = 1000000;
    POLYGON* SelectedPoly = NULL;

    while (Splitter != NULL) {
        if (Splitter->BeenUsedAsSplitter != true) {

            PLANE SplittersPlane;
            SplittersPlane.Normal = Splitter->Normal;
            SplittersPlane.PointOnPlane = *(D3DXVECTOR3*)&Splitter->VertexList[0];
            CurrentPoly = PolyList;
            unsigned long score, splits, backfaces, frontfaces;
            score = splits = backfaces = frontfaces = 0;

            while (CurrentPoly != NULL) {

                int result = ClassifyPoly(&SplittersPlane, CurrentPoly);
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
                } // switch

                CurrentPoly = CurrentPoly->Next;
            } // end while current poly

            score = abs(frontfaces - backfaces) + (splits * 3);
            if (score < BestScore) {
                BestScore = score;
                SelectedPoly = Splitter;
            }

        } // end if this splitter has not been used yet
        Splitter = Splitter->Next;
    } // end while splitter == null

    SelectedPoly->BeenUsedAsSplitter = true;

    PlaneArray[NumberOfPlanes].PointOnPlane = *((D3DXVECTOR3*)&SelectedPoly->VertexList[0]);
    PlaneArray[NumberOfPlanes].Normal = SelectedPoly->Normal;
    IncreaseNumberOfPlanes();
    return (NumberOfPlanes - 1);
}

// calculates a Bounding Box from the List of Polys passed in
//============================================================

void CalculateBox(BOUNDINGBOX* Box, POLYGON* Polylist) {
    WORD i;
    POLYGON* PolyPointer = Polylist;
    while (PolyPointer != NULL) {

        for (i = 0; i < PolyPointer->NumberOfVertices; i++) {
            // check Minimum Bounds
            if (PolyPointer->VertexList[i].x < Box->BoxMin.x)
                Box->BoxMin.x = PolyPointer->VertexList[i].x;
            if (PolyPointer->VertexList[i].y < Box->BoxMin.y)
                Box->BoxMin.y = PolyPointer->VertexList[i].y;
            if (PolyPointer->VertexList[i].z < Box->BoxMin.z)
                Box->BoxMin.z = PolyPointer->VertexList[i].z;
            // check Maximum Bounds
            if (PolyPointer->VertexList[i].x > Box->BoxMax.x)
                Box->BoxMax.x = PolyPointer->VertexList[i].x;
            if (PolyPointer->VertexList[i].y > Box->BoxMax.y)
                Box->BoxMax.y = PolyPointer->VertexList[i].y;
            if (PolyPointer->VertexList[i].z > Box->BoxMax.z)
                Box->BoxMax.z = PolyPointer->VertexList[i].z;
        }

        PolyPointer = PolyPointer->Next;
    }
}

//**********************************************************************************************
//                          LINE OF SIGHT
//
// Checks if there is a clear Line of sight between two points be recursing the tree and making sure
// that none of the line segmenbts end up in solid space.
//
//***************************************************************************************

bool LineOfSight(D3DXVECTOR3* Start, D3DXVECTOR3* End, long Node) {
    float temp;
    D3DXVECTOR3 intersection;

    NODE* CurrNode = &NodeArray[Node];
    PLANE* Plane = &PlaneArray[CurrNode->Plane];

    int PointA = ClassifyPoint(Start, Plane);
    int PointB = ClassifyPoint(End, Plane);

    //***************** IF BOTH POINTS ARE ON_PLANE

    if (PointA == CP_ONPLANE && PointB == CP_ONPLANE) {
        if (CurrNode->IsLeaf == 0) // this is not a leaf so recurse
        {
            return LineOfSight(Start, End, CurrNode->Front);
        } else {
            return true; // This is a front leaf.Front Leafs are always empty so this is empty space
        }
    }

    if (PointA == CP_FRONT && PointB == CP_BACK) {
        if (CurrNode->Back == -1)
            return false;

        Get_Intersect(Start, End, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp);

        if (CurrNode->IsLeaf == 0) {
            return LineOfSight(Start, &intersection, CurrNode->Front) && LineOfSight(End, &intersection, CurrNode->Back);
        } else {
            return true && LineOfSight(End, &intersection, CurrNode->Back);
        }
    }

    if (PointA == CP_BACK && PointB == CP_FRONT) {
        if (CurrNode->Back == -1)
            return false;
        Get_Intersect(Start, End, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp);

        if (CurrNode->IsLeaf == 0) {
            return LineOfSight(End, &intersection, CurrNode->Front) && LineOfSight(Start, &intersection, CurrNode->Back);
        } else {
            return true && LineOfSight(Start, &intersection, CurrNode->Back);
        }
    }

    // if we get here one of the points is on the plane
    if (PointA == CP_FRONT || PointB == CP_FRONT) {
        if (CurrNode->IsLeaf == 0) {
            return LineOfSight(Start, End, CurrNode->Front);
        } else {
            return true;
        }

    }

    else

    {
        if (CurrNode->Back == -1) {
            return false;
        } else {
            return LineOfSight(Start, End, CurrNode->Back);
        }
    }

    return true;
}

//*******************************************************************************
//                    COLLIDE SPHERE

//*******************************************************************************

bool CollideSphere(D3DXVECTOR3* SphereCenter, float SphereRadius, long Node) {
    float temp;

    int PointA;
    D3DXVECTOR3 intersection, SphereRadiusEnd;
    PLANE* Plane = &PlaneArray[NodeArray[Node].Plane];
    NODE* CurrNode = &NodeArray[Node];

    PointA = ClassifyPoint(SphereCenter, Plane);

    if (PointA == CP_BACK) {
        SphereRadiusEnd = *SphereCenter + (Plane->Normal * SphereRadius);
    } else {
        SphereRadiusEnd = *SphereCenter + (-Plane->Normal * SphereRadius);
    }
    int PointB = ClassifyPoint(&SphereRadiusEnd, Plane);

    //***************** IF BOTH POINTS ARE ON_PLANE

    if (PointA == CP_ONPLANE && PointB == CP_ONPLANE) {
        if (CurrNode->IsLeaf == 0) // this is not a leaf so recurse
        {
            return CollideSphere(SphereCenter, SphereRadius, CurrNode->Front);
        } else {
            return false; // This is a front leaf.Front Leafs are always empty so this is empty space
        }
    }

    // Spanning front to back

    if (PointA == CP_FRONT && PointB == CP_BACK) {
        if (CurrNode->Back == -1)
            return true;

        Get_Intersect(SphereCenter, &SphereRadiusEnd, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp);

        if (CurrNode->IsLeaf == 0) {
            return CollideSphere(SphereCenter, D3DXVec3Length(&(intersection - *SphereCenter)), CurrNode->Front) ||
                   CollideSphere(&SphereRadiusEnd, D3DXVec3Length(&(intersection - SphereRadiusEnd)), CurrNode->Back);
        } else {
            return CollideSphere(&SphereRadiusEnd, D3DXVec3Length(&(intersection - SphereRadiusEnd)), CurrNode->Back);
        }
    }

    // spanning Back to front

    if (PointA == CP_BACK && PointB == CP_FRONT) {
        if (CurrNode->Back == -1)
            return true;
        Get_Intersect(&SphereRadiusEnd, SphereCenter, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp);

        if (CurrNode->IsLeaf == 0) {
            return CollideSphere(&SphereRadiusEnd, D3DXVec3Length(&(intersection - SphereRadiusEnd)), CurrNode->Front) ||
                   CollideSphere(SphereCenter, D3DXVec3Length(&(intersection - *SphereCenter)), CurrNode->Back);
        } else {
            return CollideSphere(SphereCenter, D3DXVec3Length(&(intersection - *SphereCenter)), CurrNode->Back);
        }
    }

    // if we get here one of the points is on the plane
    if (PointA == CP_FRONT || PointB == CP_FRONT) {
        if (CurrNode->IsLeaf == 0) {
            return CollideSphere(SphereCenter, SphereRadius, CurrNode->Front);
        } else {
            return false;
        }

    }

    else

    {
        if (CurrNode->Back == -1) {
            return true;
        } else {
            return CollideSphere(SphereCenter, SphereRadius, CurrNode->Back);
        }
    }

    return false;
}
