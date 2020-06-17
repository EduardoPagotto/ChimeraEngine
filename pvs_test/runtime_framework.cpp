

#include "runtime_framework.h"

POLYGON* pTexturePolygonList[25]; // Polygons will be selected from their leaves each frame
                                  // and added to the polygon list of the corresponding texture
                                  // this allows us to batch the polygons to minimize texture changes

// These are the Master Arrays that will contain the final tree

POLYGON* PolygonArray;
NODE* NodeArray;
LEAF* LeafArray;
PLANE* PlaneArray;
BYTE* PVSData;

long NumberOfPolygons = 0;
long NumberOfNodes = 0;
long NumberOfLeafs = 0;
long NumberOfPlanes = 0;

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

// This function Extracts the Frustum Planes from a Combined View/Projection matrix.
// NOTE: I do not call GetTransform to get the ViewMatrix from D3D because we may be
// in top down mode.When in top down mode we still want the clip planes to be around
// the first person view so that we can see the clippinf working.

void ExtractFrustumPlanes(PLANE2* Planes) {

    D3DXMATRIX ViewMatrix, ProjMatrix, ViewProj;
    lpDevice->GetTransform(D3DTS_PROJECTION, &ProjMatrix); // get the current projection matrix

    D3DXMatrixIdentity(&ViewMatrix); // make view an identity matrix to start with

    ViewMatrix._11 = right_vector.x;
    ViewMatrix._12 = up_vector.x;
    ViewMatrix._13 = look_vector.x;
    ViewMatrix._21 = right_vector.y;
    ViewMatrix._22 = up_vector.y;
    ViewMatrix._23 = look_vector.y;
    ViewMatrix._31 = right_vector.z;
    ViewMatrix._32 = up_vector.z;
    ViewMatrix._33 = look_vector.z;
    ViewMatrix._41 = -D3DXVec3Dot(&position, &right_vector); // dot product defined in d3dtypes.h
    ViewMatrix._42 = -D3DXVec3Dot(&position, &up_vector);
    ViewMatrix._43 = -D3DXVec3Dot(&position, &look_vector);

    D3DXMatrixMultiply(&ViewProj, &ViewMatrix, &ProjMatrix); // combine View and Projection matrices

    // Left clipping plane
    Planes[0].Normal.x = -(ViewProj._14 + ViewProj._11);
    Planes[0].Normal.y = -(ViewProj._24 + ViewProj._21);
    Planes[0].Normal.z = -(ViewProj._34 + ViewProj._31);
    Planes[0].Distance = -(ViewProj._44 + ViewProj._41);

    // Right clipping plane
    Planes[1].Normal.x = -(ViewProj._14 - ViewProj._11);
    Planes[1].Normal.y = -(ViewProj._24 - ViewProj._21);
    Planes[1].Normal.z = -(ViewProj._34 - ViewProj._31);
    Planes[1].Distance = -(ViewProj._44 - ViewProj._41);

    // Top clipping plane
    Planes[2].Normal.x = -(ViewProj._14 - ViewProj._12);
    Planes[2].Normal.y = -(ViewProj._24 - ViewProj._22);
    Planes[2].Normal.z = -(ViewProj._34 - ViewProj._32);
    Planes[2].Distance = -(ViewProj._44 - ViewProj._42);

    // Bottom clipping plane
    Planes[3].Normal.x = -(ViewProj._14 + ViewProj._12);
    Planes[3].Normal.y = -(ViewProj._24 + ViewProj._22);
    Planes[3].Normal.z = -(ViewProj._34 + ViewProj._32);
    Planes[3].Distance = -(ViewProj._44 + ViewProj._42);

    // Near clipping plane
    Planes[4].Normal.x = -(ViewProj._14 + ViewProj._13);
    Planes[4].Normal.y = -(ViewProj._24 + ViewProj._23);
    Planes[4].Normal.z = -(ViewProj._34 + ViewProj._33);
    Planes[4].Distance = -(ViewProj._44 + ViewProj._43);

    // Far clipping plane
    Planes[5].Normal.x = -(ViewProj._14 - ViewProj._13);
    Planes[5].Normal.y = -(ViewProj._24 - ViewProj._23);
    Planes[5].Normal.z = -(ViewProj._34 - ViewProj._33);
    Planes[5].Distance = -(ViewProj._44 - ViewProj._43);

    for (int i = 0; i < 6; i++) {
        float denom = 1.0f / D3DXVec3Length(&Planes[i].Normal); // Get magnitude of Vector
        Planes[i].Normal.x *= denom;
        Planes[i].Normal.y *= denom;
        Planes[i].Normal.z *= denom;
        Planes[i].Distance *= denom;
    }

} // End Funtion Extract Clip Planes

//               LEAF IN FRUSTUM
//
// Tests if the Bounding Box surrounding the leaf is completely outside the frustum.
//
// NOTE: This check is only done on visible leafs in a Leafs PVS Set to further carve off
// more polygons than just the straight PVS Set.Remember a PVS set does not take into account
// Direction of camera so some of the PVS Set will not be visible.

bool LeafInFrustum(long Leaf) {
    D3DXVECTOR3 bMax = LeafArray[Leaf].BoundingBox.BoxMax;
    D3DXVECTOR3 bMin = LeafArray[Leaf].BoundingBox.BoxMin;
    D3DXVECTOR3 NearPoint, FarPoint;
    PLANE2* Plane = FrustumPlanes;
    for (int i = 0; i < 6; i++) {

        if (Plane->Normal.x > 0.0f) {
            if (Plane->Normal.y > 0.0f) {
                if (Plane->Normal.z > 0.0f) {
                    NearPoint.x = bMin.x;
                    NearPoint.y = bMin.y;
                    NearPoint.z = bMin.z;
                } else {
                    NearPoint.x = bMin.x;
                    NearPoint.y = bMin.y;
                    NearPoint.z = bMax.z;
                }
            } else {
                if (Plane->Normal.z > 0.0f) {
                    NearPoint.x = bMin.x;
                    NearPoint.y = bMax.y;
                    NearPoint.z = bMin.z;
                } else {
                    NearPoint.x = bMin.x;
                    NearPoint.y = bMax.y;
                    NearPoint.z = bMax.z;
                }
            }
        } else {
            if (Plane->Normal.y > 0.0f) {
                if (Plane->Normal.z > 0.0f) {
                    NearPoint.x = bMax.x;
                    NearPoint.y = bMin.y;
                    NearPoint.z = bMin.z;
                } else {
                    NearPoint.x = bMax.x;
                    NearPoint.y = bMin.y;
                    NearPoint.z = bMax.z;
                }
            } else {
                if (Plane->Normal.z > 0.0f) {
                    NearPoint.x = bMax.x;
                    NearPoint.y = bMax.y;
                    NearPoint.z = bMin.z;
                } else {
                    NearPoint.x = bMax.x;
                    NearPoint.y = bMax.y;
                    NearPoint.z = bMax.z;
                }
            }
        }

        // near extreme point is outside, and thus
        // the AABB is Totally outside the polyhedron

        if (D3DXVec3Dot(&Plane->Normal, &NearPoint) + Plane->Distance > 0)
            return false;
        Plane++;
    }

    return true;
}

//**************************************************************************
void DeletePolygonArray(void) {
    for (long i = 0; i < NumberOfPolygons; i++) {
        delete PolygonArray[i].VertexList;
        delete PolygonArray[i].Indices;
    }
    free(PolygonArray);
}
