

#include "pvs_bsp.h"

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
