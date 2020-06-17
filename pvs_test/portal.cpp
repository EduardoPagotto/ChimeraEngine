

#include "pvs_bsp.h"

struct NODESTACK {
    long Node;
    BYTE JumpBackPoint;
};

//*********************************************************************************************
//                           CALCULATE INITIAL PORTAL
//
// This function builds a large portal on the Node Plane large enough to encompass all the leafs
// in its front and back trees.This portal will then get sent down the tree to get carved into
// possible portals.
//*********************************************************************************************
PORTAL* CalculateInitialPortal(long Node) {

    D3DXVECTOR3 MaxP, MinP, CB, CP;

    MaxP = NodeArray[Node].BoundingBox.BoxMax; // Get this Nodes Bounding Box ranges
    MinP = NodeArray[Node].BoundingBox.BoxMin;

    D3DXVECTOR3 PlaneNormal = PlaneArray[NodeArray[Node].Plane].Normal;
    CB = (MaxP + MinP) / 2;
    float DistanceToPlane = D3DXVec3Dot(&(PlaneArray[NodeArray[Node].Plane].PointOnPlane - CB), &PlaneNormal);
    CP = CB + (PlaneNormal * DistanceToPlane);

    D3DXVECTOR3 A = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    if (fabs(PlaneNormal.y) > fabs(PlaneNormal.z)) {
        if (fabs(PlaneNormal.z) < fabs(PlaneNormal.x)) {
            A.z = 1;
        } else {
            A.x = 1;
        }
    } else {
        if (fabs(PlaneNormal.y) <= fabs(PlaneNormal.x)) {
            A.y = 1;
        } else {
            A.x = 1;
        }
    }

    D3DXVECTOR3 U, V;

    D3DXVec3Cross(&U, &A, &PlaneNormal);
    D3DXVec3Normalize(&U, &U);
    D3DXVec3Cross(&V, &U, &PlaneNormal);
    D3DXVec3Normalize(&V, &V);

    D3DXVECTOR3 BoxHalfLength = (MaxP - CB);
    float Length = D3DXVec3Length(&BoxHalfLength);

    U = U * Length;
    V = V * Length;

    D3DXVECTOR3 P[4];
    P[0] = CP + U - V; // V = UP Vactor
    P[1] = CP + U + V; //    --   +-
    P[2] = CP - U + V; // U = Right Vector
    P[3] = CP - U - V; //      ++ -+

    PORTAL* Portal = new PORTAL;
    ZeroMemory(Portal, sizeof(PORTAL));

    Portal->Normal = PlaneNormal;
    Portal->NumberOfVertices = 4;
    Portal->NumberOfIndices = 6;

    Portal->VertexList = new D3DLVERTEX[Portal->NumberOfVertices];
    Portal->Indices = new WORD[Portal->NumberOfIndices];

    for (int i = 0; i < 4; i++) {
        Portal->VertexList[i].x = P[i].x;
        Portal->VertexList[i].y = P[i].y;
        Portal->VertexList[i].z = P[i].z;
        Portal->VertexList[i].color = D3DCOLOR_RGBA(rand() % 255, rand() % 255, rand() % 255, 240);
        Portal->VertexList[i].specular = 0;
        Portal->VertexList[i].tu = 0;
        Portal->VertexList[i].tv = 0;
    }
    Portal->Indices[0] = 0;
    Portal->Indices[1] = 1;
    Portal->Indices[2] = 3;
    Portal->Indices[3] = 3;
    Portal->Indices[4] = 1;
    Portal->Indices[5] = 2;
    Portal->Next = NULL;
    Portal->Prev = NULL;
    Portal->NumberOfLeafs = 0;
    return Portal;
}

//*****************************************************************************************
//                   BUILD PORTALS
//
// This function creates and manges all the portals.It creates the intial portal and sends
// it down the tree.The returned fragments are checks to see whether they are Valid Portals.
// If they are valid portals they are added to the master portal array.
//
// Note: This function looks a little ugly with a couple of ugly GOTO's.I originally wrote this
// function as a recursive function which was a lot tidier but this function (which recrurses the tree)
// also calls clip portal which recurses the tree so i imagines that on a large data set recursing the tree
// while recursing the tree could possibly cause a stack over flow so changed it to this not so neat
// implementation.
//**************************************************************************************

void BuildPortals(void) {

    long stackpointer = 0;
    NODESTACK* NodeStack = new NODESTACK[NumberOfNodes + 1];
    int portalindex;
    NodeStack[stackpointer].Node = 0; // root node
    NodeStack[stackpointer].JumpBackPoint = 0;

START:

    PORTAL* InitialPortal = CalculateInitialPortal(NodeStack[stackpointer].Node);
    PORTAL* PortalList = ClipPortal(0, InitialPortal);
    PORTAL* Iterator = PortalList; // Step through the Portal List

    while (Iterator != NULL) // BEGIN WHILE LOOPto REMOVE Portals NOT in 2 leafs
    {
        if (Iterator->NumberOfLeafs != 2) {
            PORTAL* temp = Iterator->Next;
            RemovePortalFromList(Iterator);
            Iterator = temp;
        } else // add this portal
        {
            if (CheckDuplicatePortal(Iterator, &portalindex) == true) {
                PORTAL* temp = Iterator->Next;
                RemovePortalFromList(Iterator);
                Iterator = temp;
            } else {
                // clip to the leafs here

                PortalArray[portalindex] = Iterator;
                if (portalindex == NumberOfPortals) {
                    for (int a = 0; a < Iterator->NumberOfLeafs; a++) {
                        long Index = Iterator->LeafOwnerArray[a];
                        LeafArray[Index].PortalIndexList[LeafArray[Index].NumberOfPortals] = NumberOfPortals;
                        LeafArray[Index].NumberOfPortals++;
                    }
                    IncreaseNumberOfPortals();
                }

                Iterator = Iterator->Next;

            } // if not a duplicate portal

        } // end else

    } // END WHILE LOOP

    if (NodeArray[NodeStack[stackpointer].Node].IsLeaf == 0) {
        NodeStack[stackpointer + 1].Node = NodeArray[NodeStack[stackpointer].Node].Front;
        NodeStack[stackpointer + 1].JumpBackPoint = 1;
        stackpointer++;
        goto START;
    }

BACK:
    if (NodeArray[NodeStack[stackpointer].Node].Back != -1) {
        NodeStack[stackpointer + 1].Node = NodeArray[NodeStack[stackpointer].Node].Back;
        NodeStack[stackpointer + 1].JumpBackPoint = 2;
        stackpointer++;
        goto START;
    };

END:

    stackpointer--;
    if (stackpointer > -1) {
        if (NodeStack[stackpointer + 1].JumpBackPoint == 1)
            goto BACK;
        else if (NodeStack[stackpointer + 1].JumpBackPoint == 2)
            goto END;
    }
    delete[] NodeStack;
}

//*******************************************************************************************
//                        CLIP PORTAL
//
// This recursive function repeatedly clips the current portal to the tree until it ends up in
// a leaf at which point it is retured
//*******************************************************************************************

PORTAL* ClipPortal(long Node, PORTAL* Portal) {
    PORTAL *PortalList = NULL, *FrontPortalList = NULL, *BackPortalList = NULL, *Iterator = NULL, *FrontSplit = NULL, *BackSplit = NULL;

    switch (ClassifyPoly(&PlaneArray[NodeArray[Node].Plane], (POLYGON*)Portal)) {
        case CP_ONPLANE: // The Portal has to be sent down Both sides of the tree and tracked.Send it down front first
                         // but DO NOT delete any bits that end up in solid space, just ignore them.

            if (NodeArray[Node].IsLeaf != 0) // this is a Leaf Node
            {                                // The Front is a Leaf

                Portal->LeafOwnerArray[Portal->NumberOfLeafs] = NodeArray[Node].Front;
                Portal->NumberOfLeafs++;
                Portal->Next = NULL;
                Portal->Prev = NULL;
                FrontPortalList = Portal;
            } else {
                // Send the Portal Down the Front List and get returned a list of PortalFragments that survived the Front Tree
                FrontPortalList = ClipPortal(NodeArray[Node].Front, Portal);
            }

            // Then send each fragment down the back tree.
            if (FrontPortalList == NULL)
                return NULL;
            ;

            if (NodeArray[Node].Back == -1)
                return FrontPortalList;

            // Loop through each front list fragment and send it down the back tree

            while (FrontPortalList != NULL) {
                PORTAL* tempnext = FrontPortalList->Next;
                BackPortalList = NULL;
                BackPortalList = ClipPortal(NodeArray[Node].Back, FrontPortalList); // fragment this fragment into the back tree

                // now set to the end of the returned back grafment listto get the last fragment
                if (BackPortalList != NULL) {

                    Iterator = BackPortalList;
                    while (Iterator->Next != NULL) {
                        Iterator = Iterator->Next;
                    }

                    // attach the last fragment to the first fragment from a previos loop.

                    Iterator->Next = PortalList;
                    if (PortalList != NULL) {
                        PortalList->Prev = Iterator;
                    }
                    PortalList = BackPortalList; // portal List now points at the current complete list of fragment collected from each loop

                } //**************

                FrontPortalList = tempnext;
            }
            return PortalList;
            break;

        case CP_FRONT: // Either send it down the front tree or add it to the portal list because it has come out
                       // Empty Space

            if (NodeArray[Node].IsLeaf == 0) {
                PortalList = ClipPortal(NodeArray[Node].Front, Portal);
                return PortalList;
            } else // The Front Node is a Empty Leaf so Add it to the Portal List
            {

                Portal->LeafOwnerArray[Portal->NumberOfLeafs] = NodeArray[Node].Front;
                Portal->NumberOfLeafs++;
                Portal->Next = NULL;
                Portal->Prev = NULL;
                return Portal;
            }

            break;

        case CP_BACK: // either asend it downthe back tree or Delete it if no back tree exists
                      // because it means it has come out in solid space

            if (NodeArray[Node].Back != -1) {
                PortalList = ClipPortal(NodeArray[Node].Back, Portal);
                return PortalList;
            } else {
                DeletePortal(Portal);
                return NULL;
            }
            break;

        case CP_SPANNING:

            FrontSplit = new PORTAL;
            BackSplit = new PORTAL;
            ZeroMemory(FrontSplit, sizeof(PORTAL));
            ZeroMemory(BackSplit, sizeof(PORTAL));

            SplitPortal(Portal, &PlaneArray[NodeArray[Node].Plane], FrontSplit, BackSplit);
            DeletePortal(Portal);

            if (NodeArray[Node].IsLeaf == 0) // There is another Front NODE
            {
                FrontPortalList = ClipPortal(NodeArray[Node].Front, FrontSplit);
            } else // Its about to get pushed into a Leaf
            {

                FrontSplit->LeafOwnerArray[FrontSplit->NumberOfLeafs] = NodeArray[Node].Front;
                FrontSplit->NumberOfLeafs++;
                FrontSplit->Prev = NULL;
                FrontSplit->Next = NULL;
                FrontPortalList = FrontSplit;
            }

            if (NodeArray[Node].Back != -1) // the backsplit is in solid space
            {
                BackPortalList = ClipPortal(NodeArray[Node].Back, BackSplit);
            } else // Its a NOde so puchit down the back tree
            {
                DeletePortal(BackSplit);
            }

            if (FrontPortalList != NULL) // Find the End of the list and attach it to Front Back List
            {
                Iterator = FrontPortalList;
                while (Iterator->Next != NULL) {
                    Iterator = Iterator->Next;
                }
                if (BackPortalList != NULL) {
                    Iterator->Next = BackPortalList;
                    BackPortalList->Prev = Iterator;
                }
                return FrontPortalList;
            } // there is something in the front list

            else ////There is nothing in the front list
            {
                if (BackPortalList != NULL)
                    return BackPortalList;
                return NULL;
            }

            return NULL;
            break;

        default:
            return NULL;
            break;

    } // switch

    return NULL;
}

//****************************************************************************************************************
//
//                          SPLIT PORTAL (uses split polygon is really just a convenience function
//
//*************************************************************************************************************

void SplitPortal(PORTAL* Portal, PLANE* Plane, PORTAL* FrontSplit, PORTAL* BackSplit) {
    SplitPolygon((POLYGON*)Portal, Plane, (POLYGON*)FrontSplit, (POLYGON*)BackSplit);
    FrontSplit->NumberOfLeafs = Portal->NumberOfLeafs;
    BackSplit->NumberOfLeafs = Portal->NumberOfLeafs;
    memcpy(FrontSplit->LeafOwnerArray, Portal->LeafOwnerArray, sizeof(long) * Portal->NumberOfLeafs);
    memcpy(BackSplit->LeafOwnerArray, Portal->LeafOwnerArray, sizeof(long) * Portal->NumberOfLeafs);
}

// ****************************************************************************
//                        DELETE PORTAL FROM LIST
//
// Removes a portal from the linked list of portals.This is used to remove a portal
// fragment from the fragment array when it fails the test of either not being in two leafs
// or the portal already in the array is larger.

void RemovePortalFromList(PORTAL* RemovePortal) {
    PORTAL* temp = RemovePortal;
    PORTAL *PrevPortal, *NextPortal;

    if (RemovePortal->Prev != NULL) {
        PrevPortal = RemovePortal->Prev;
        if (RemovePortal->Next != NULL) {
            PrevPortal->Next = RemovePortal->Next;
        } else {
            PrevPortal->Next = NULL;
        }
    } // if there is a prev

    if (RemovePortal->Next != NULL) {
        NextPortal = RemovePortal->Next;
        if (RemovePortal->Prev != NULL) {
            NextPortal->Prev = RemovePortal->Prev;
        } else {
            NextPortal->Prev = NULL;
        }
    }
    DeletePortal(temp);
}

//*******************************  CHECK DUPLICATE PORTALS ***********************************************88
//
// This is used to check the current portal with the portal already sharing the same leafs in
// tree. It is only accepted if the Portal is Larger than the one that is ready occupying this
// place in the Master Portal Array
//**************************************************************************************

bool CheckDuplicatePortal(PORTAL* CheckPortal, int* index) {
    long CheckPortalLeaf1 = CheckPortal->LeafOwnerArray[0];
    long CheckPortalLeaf2 = CheckPortal->LeafOwnerArray[1];
    long PALeaf1 = 0;
    long PALeaf2 = 0;
    for (long i = 0; i < NumberOfPortals; i++) {
        PALeaf1 = PortalArray[i]->LeafOwnerArray[0];
        PALeaf2 = PortalArray[i]->LeafOwnerArray[1];

        if ((CheckPortalLeaf1 == PALeaf1 && CheckPortalLeaf2 == PALeaf2) || (CheckPortalLeaf1 == PALeaf2 && CheckPortalLeaf2 == PALeaf1)) {
            D3DXVECTOR3 Max1, Min1, Max2, Min2;

            GetPolygonBounds((POLYGON*)CheckPortal, &Min1, &Max1);
            GetPolygonBounds((POLYGON*)PortalArray[i], &Min2, &Max2);

            float NewSize = D3DXVec3Length(&(Max1 - Min1)); // Measure the Lengths of the vector to
                                                            // see which is bigger
            float OldSize = D3DXVec3Length(&(Max2 - Min2));

            if (fabs(NewSize) > fabs(OldSize)) {
                PORTAL* temp = PortalArray[i];
                DeletePortal(temp);
                *index = i;
                return false;
            } else {
                return true; // This portal is already in the array
            }
        }
    }
    *index = NumberOfPortals;
    return false; // This portal was not found inthe array
}

//**************************************************************************************
//         GET POLYGON BOUNDS
//
//   Simply returns the Min/Max bounds of a polygon or portal
//************************************************************************************

void GetPolygonBounds(POLYGON* Poly, D3DXVECTOR3* Min, D3DXVECTOR3* Max) {
    float XMax = -99999, XMin = 99999;
    float YMax = -99999, YMin = 99999;
    float ZMax = -99999, ZMin = 99999;

    for (WORD i = 0; i < Poly->NumberOfVertices; i++) {
        D3DXVECTOR3 P = *((D3DXVECTOR3*)&Poly->VertexList[i]);

        // Get X Bounds
        if (P.x > XMax) {
            XMax = P.x;
        } else if (P.x < XMin) {
            XMin = P.x;
        }
        // Get Y Bounds
        if (P.y > YMax) {
            YMax = P.y;
        } else if (P.y < YMin) {
            YMin = P.y;
        }
        // Get Z Bounds
        if (P.z > ZMax) {
            ZMax = P.z;
        } else if (P.z < ZMin) {
            ZMin = P.z;
        }
    } // End For i

    *Min = D3DXVECTOR3(XMin, YMin, ZMin);
    *Max = D3DXVECTOR3(XMax, YMax, ZMax);
}

//********************************GetPortalPlane*************************************************
//
// Convenience function to create a Plane structure from a portal. This saves me havinf to
// fill out a PLANE structure manually everytime i want to use a portals plane
//
//*******************************************************************************************

PLANE GetPortalPlane(PORTAL* Portal) {
    PLANE plane;
    plane.Normal = Portal->Normal;
    plane.PointOnPlane = *(D3DXVECTOR3*)&Portal->VertexList[0];
    return plane;
}

//*********************************************************************************************
//
//                        DRAW PORTALS
//
// Can be called if you want to see where the portals have been created.
//
//*********************************************************************************************

void DrawPortals() {

    lpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    for (int index = 0; index < NumberOfPortals; index++) {

        // lpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX,&PortalArray[index]->VertexList[0],PortalArray[index]->NumberOfVertices,&PortalArray[index]->Indices[0],PortalArray[index]->NumberOfIndices,NULL);
        lpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, PortalArray[index]->NumberOfVertices,
                                         PortalArray[index]->NumberOfIndices / 3, &PortalArray[index]->Indices[0], D3DFMT_INDEX16,
                                         &PortalArray[index]->VertexList[0], sizeof(D3DLVERTEX));
    }
    lpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
