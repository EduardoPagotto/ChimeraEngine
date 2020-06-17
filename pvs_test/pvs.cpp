

#include "pvs_bsp.h"
//*******************************************************************************************************************
//*******************************************************************************************************************8

//                          PVS FUNCTIONS PVS FUNCTIONS

//*******************************************************************************************************************
//*******************************************************************************************************************8

long CalculatePVS() {

    BYTE* LeafPVS = new BYTE[BytesPerSet];
    long PVSMasterWritePointer = 0;
    for (long Leaf = 0; Leaf < NumberOfLeafs; Leaf++) {
        LeafArray[Leaf].PVSIndex = PVSMasterWritePointer;
        ZeroMemory(LeafPVS, BytesPerSet);
        SetPVSBit(LeafPVS, Leaf);

        for (long SPI = 0; SPI < LeafArray[Leaf].NumberOfPortals; SPI++) {

            PORTAL* SourcePortal = PortalArray[LeafArray[Leaf].PortalIndexList[SPI]];
            long TargetLeaf = SourcePortal->LeafOwnerArray[0];
            if (TargetLeaf == Leaf) {
                TargetLeaf = SourcePortal->LeafOwnerArray[1];
            }
            SetPVSBit(LeafPVS, TargetLeaf);
            for (long DPI = 0; DPI < LeafArray[TargetLeaf].NumberOfPortals; DPI++) {
                PORTAL* TargetPortal = PortalArray[LeafArray[TargetLeaf].PortalIndexList[DPI]];
                if (SourcePortal != TargetPortal && ClassifyPoly(&GetPortalPlane(SourcePortal), (POLYGON*)TargetPortal) != CP_ONPLANE) {
                    //
                    RecursePVS(Leaf, SourcePortal, TargetPortal, TargetLeaf, LeafPVS);

                } // End for If Source != Dest
            }     // End for DPI
        }         // End for SPI
        PVSMasterWritePointer += CompressLeafSet(LeafPVS, PVSMasterWritePointer);
    } // End for Leaf

    delete[] LeafPVS;
    return PVSMasterWritePointer;
} // End Function

//////////////*******************************************************************************************************
//////////////                                RECURSE PVS
//////////////*******************************************************************************************************

void RecursePVS(long SourceLeaf, PORTAL* SrcPortal, PORTAL* TargetPortal, long TargetLeaf, BYTE* LeafPVS) {

    long GeneratorLeaf = TargetPortal->LeafOwnerArray[0];
    if (GeneratorLeaf == TargetLeaf)
        GeneratorLeaf = TargetPortal->LeafOwnerArray[1];

    SetPVSBit(LeafPVS, GeneratorLeaf);

    // This next bit finds out which side of the Source portal the the Source Leaf is on, front or back
    D3DXVECTOR3 SourceLeafCenter = (LeafArray[SourceLeaf].BoundingBox.BoxMax + LeafArray[SourceLeaf].BoundingBox.BoxMin) / 2;
    D3DXVECTOR3 TargetLeafCenter = (LeafArray[TargetLeaf].BoundingBox.BoxMax + LeafArray[TargetLeaf].BoundingBox.BoxMin) / 2;
    int SourceLeafLocation = ClassifyPoint(&SourceLeafCenter, &GetPortalPlane(SrcPortal));
    int TargetLeafLocation = ClassifyPoint(&TargetLeafCenter, &GetPortalPlane(TargetPortal));

    for (long GPI = 0; GPI < LeafArray[GeneratorLeaf].NumberOfPortals; GPI++) {

        // If the GeneratePortal == The Target Portal then ignore
        if (PortalArray[LeafArray[GeneratorLeaf].PortalIndexList[GPI]] == TargetPortal) { /*delete GeneratorPortal,delete SourcePortal;*/
            continue;
        }

        // We Make Copies of both the Source And Destination Portals because these will need to be clipped and we dont want to clip
        // the ACTUAL portals in the Master Portal Array because we will need this for other Leafs to be calculated
        PORTAL* SourcePortal = new PORTAL;
        *SourcePortal = *SrcPortal;
        PORTAL* GeneratorPortal = new PORTAL;
        *GeneratorPortal = *PortalArray[LeafArray[GeneratorLeaf].PortalIndexList[GPI]];

        // now we check and eliminate all portals that cant possibly be seen.

        int GeneratorLocation = ClassifyPoly(&GetPortalPlane(SourcePortal), (POLYGON*)GeneratorPortal);
        // If the is on the same plane as the Source portal or on the Same side of Source portal as the source leaf ignore it
        if (GeneratorLocation == CP_ONPLANE || GeneratorLocation == SourceLeafLocation) {
            delete GeneratorPortal;
            delete SourcePortal;
            continue;
        }

        // if the Generator Portal is on the same side of the Target Portal as the Target Leaf
        GeneratorLocation = ClassifyPoly(&GetPortalPlane(TargetPortal), (POLYGON*)GeneratorPortal);
        if (GeneratorLocation == CP_ONPLANE || GeneratorLocation == TargetLeafLocation) {
            delete GeneratorPortal;
            delete SourcePortal;
            continue;
        }

        GeneratorPortal = ClipToAntiPenumbra(SourcePortal, TargetPortal, GeneratorPortal);

        if (GeneratorPortal == NULL) {
            if (SourcePortal)
                delete SourcePortal;
            continue;
            continue;
        }

        SourcePortal = ClipToAntiPenumbra(GeneratorPortal, TargetPortal, SourcePortal);

        if (SourcePortal == NULL) {
            if (GeneratorPortal)
                delete GeneratorPortal;
            continue;
        }

        RecursePVS(SourceLeaf, SourcePortal, GeneratorPortal, GeneratorLeaf, LeafPVS);
        delete GeneratorPortal;
        delete SourcePortal;
    }
}

////////////////******************************************************************************************************
////////////////                          CLIP TO ANTI-PENUMBRA
////////////////******************************************************************************************************

PORTAL* ClipToAntiPenumbra(PORTAL* SourcePortal, PORTAL* TargetPortal, PORTAL* GeneratorPortal) {
    D3DXVECTOR3 EdgeVector1, EdgeVector2, Normal;
    int PortalLocation;
    PORTAL *FrontSplit, *BackSplit, *TempSource, *TempTarget;

    CLIPPLANES ClipPlanes;         // Create a ClipPlane set
    ClipPlanes.NumberOfPlanes = 0; // Set Current Number to zero because we have not added any planes yet
    ClipPlanes.Planes =
        new PLANE[SourcePortal->NumberOfVertices * TargetPortal->NumberOfVertices * 2]; // max possible planes that could be stored
    PLANE TempPlane;
    int NextVertex = 0;

    for (int loop = 0; loop < 2; loop++) // We loop though twice swapping Target and Source to shave off just that little bit extra
    {
        if (loop == 0) {
            TempSource = SourcePortal;
            TempTarget = TargetPortal;
        } else {
            TempSource = TargetPortal;
            TempTarget = SourcePortal;
        }
        for (int SV = 0; SV < TempSource->NumberOfVertices; SV++) {
            // find out whether the Source Portal Point is behind the Dest portal because we will have to flip the normal of all planes

            PortalLocation = ClassifyPoint((D3DXVECTOR3*)&TempSource->VertexList[SV], &GetPortalPlane(TempTarget));
            if (PortalLocation == CP_ONPLANE)
                continue;

            for (int TP = 0; TP < TempTarget->NumberOfVertices; TP++) {
                if (TP == TempTarget->NumberOfVertices - 1) {
                    NextVertex = 0;
                } else {
                    NextVertex = TP + 1;
                }

                // Create the Temp Plane using the edges from Source Portal Point to Both Dest Portal points
                EdgeVector1 = *((D3DXVECTOR3*)&TempSource->VertexList[SV]) - *((D3DXVECTOR3*)&TempTarget->VertexList[TP]);
                EdgeVector2 = *((D3DXVECTOR3*)&TempTarget->VertexList[NextVertex]) - *((D3DXVECTOR3*)&TempTarget->VertexList[TP]);

                D3DXVec3Cross(&Normal, &EdgeVector1, &EdgeVector2); // create normal
                D3DXVec3Normalize(&TempPlane.Normal, &Normal);      // make it a unit normal

                TempPlane.PointOnPlane = *(D3DXVECTOR3*)&TempSource->VertexList[SV]; // Any vertex will do

                // Now its time to see if this temp plane divides both portals into two different subspaces

                if (ClassifyPoly(&TempPlane, (POLYGON*)TempSource) == CP_FRONT) {
                    if (ClassifyPoly(&TempPlane, (POLYGON*)TempTarget) == CP_BACK) {
                        ClipPlanes.Planes[ClipPlanes.NumberOfPlanes] = TempPlane;
                        ClipPlanes.NumberOfPlanes++;
                        // Save clip plane
                    }
                } else if (ClassifyPoly(&TempPlane, (POLYGON*)TempSource) == CP_BACK) {
                    if (ClassifyPoly(&TempPlane, (POLYGON*)TempTarget) == CP_FRONT) {
                        ClipPlanes.Planes[ClipPlanes.NumberOfPlanes] = TempPlane;
                        ClipPlanes.NumberOfPlanes++;
                        // Save clip plane
                    }
                }
            } // end for tp
        }     // end for sp
    }         // END FOR LOOP

    // OK well now we have the planes that describe how the light shine through the Target portal from the Source Portal
    // Our next jub is to see whether Generator Portal is within this light and clip any parts outside of it.
    for (int i = 0; i < ClipPlanes.NumberOfPlanes; i++) {
        PortalLocation = ClassifyPoly(&ClipPlanes.Planes[i], (POLYGON*)GeneratorPortal);
        int SourcePortalLocation = ClassifyPoly(&ClipPlanes.Planes[i], (POLYGON*)SourcePortal);

        if (PortalLocation == SourcePortalLocation || PortalLocation == CP_ONPLANE) // its entirley out side the anitpenumbra so loose it
        {
            delete GeneratorPortal;
            delete[] ClipPlanes.Planes;
            return NULL;
        }
        if ((PortalLocation == CP_BACK && SourcePortalLocation == CP_FRONT) ||
            (PortalLocation == CP_FRONT &&
             SourcePortalLocation == CP_BACK)) // its passed this test so do nothing and move onto next clip plane
        {
            // keep it
            continue; // move to next clip plane loop
        }
        if (PortalLocation == CP_SPANNING) {
            // Clip the portal
            FrontSplit = new PORTAL;
            BackSplit = new PORTAL;
            ZeroMemory(FrontSplit, sizeof(PORTAL));
            ZeroMemory(BackSplit, sizeof(PORTAL));
            SplitPortal(GeneratorPortal, &ClipPlanes.Planes[i], FrontSplit, BackSplit);

            if (SourcePortalLocation == CP_FRONT) {
                delete FrontSplit;
                delete GeneratorPortal;
                GeneratorPortal = BackSplit;
            } else if (SourcePortalLocation == CP_BACK) {
                delete BackSplit;
                delete GeneratorPortal;
                GeneratorPortal = FrontSplit;
            }
        }
    }

    delete[] ClipPlanes.Planes;
    return GeneratorPortal;

} // end function

//*******************************************************************************************************************
//
//                                  SET PVS BITs
//
//********************************************************************************************************************

void SetPVSBit(BYTE* VisArray, long DestLeaf) {
    long ByteToSet = DestLeaf >> 3;
    BYTE BitToSet = (BYTE)(DestLeaf - (ByteToSet << 3));
    VisArray[ByteToSet] |= 1 << BitToSet;
}

//***********************************************************************************************
//                      COMPRESS EACH LEAFS PVS SET AND STORE IN MASTER PVS ARRAY
//***********************************************************************************************

long CompressLeafSet(BYTE* VisArray, long WritePos) {
    int j;
    int rep;

    BYTE* dest = &PVSData[WritePos];
    BYTE* dest_p;

    dest_p = dest;

    for (j = 0; j < BytesPerSet; j++) {
        *dest_p++ = VisArray[j];
        if (VisArray[j])
            continue;

        rep = 1;
        for (j++; j < BytesPerSet; j++)
            if (VisArray[j] || rep == 255)
                break;
            else
                rep++;
        *dest_p++ = rep;
        j--;
    }

    return dest_p - dest;
}
