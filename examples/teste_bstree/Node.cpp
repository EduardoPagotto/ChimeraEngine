#include <stdio.h>

#include "Node.hpp"

Node::Node() {
    Node::frontNode = NULL;
    Node::backNode = NULL;
    v = true;
}

SIDE Node::classifyPolygon(Polygon* cPolygon) {
    float A = partition.normal.x;
    float B = partition.normal.y;
    float C = partition.normal.z;
    float D = -(A * partition.vertices[0].x + B * partition.vertices[0].y + C * partition.vertices[0].z);
    float p[3];
    for (int i = 0; i < 3; i++)
        p[i] = A * cPolygon->vertices[i].x + B * cPolygon->vertices[i].y + C * cPolygon->vertices[i].z + D;
    if (p[0] > 0 && p[1] > 0 && p[2] > 0)
        return IS_INFRONT;
    else if (p[0] == 0 && p[1] == 0 && p[2] == 0)
        return IS_COPLANAR;
    else if (p[0] < 0 && p[1] < 0 && p[2] < 0)
        return IS_BEHIND;
    else
        return IS_SPANNING;
}

float Node::classify(glm::vec3* normal, glm::vec3* eye) {
    float A = normal->x;
    float B = normal->y;
    float C = normal->z;
    float D = -(A * normal->x + B * normal->y + C * normal->z);
    float p = A * eye->x + B * eye->y + C * eye->z + D;
    return p;
}

void Node::BuildTree(Node* tree, List _polygons) {
    _polygons.nextindex = 0;
    // Initilizing the first root node.
    Polygon* root = _polygons.Next();
    tree->partition = *root;
    if (v) {
        printf("ROOT:- %d\n", root->id);
        // printf("<---\n[%f %f %f] \n[%f %f %f] \n[%f %f
        // %f]\n--->\n",root->vertices[0].x,root->vertices[0].y,root->vertices[0].z,root->vertices[1].x,root->vertices[1].y,root->vertices[1].z,root->vertices[2].x,root->vertices[2].y,root->vertices[2].z);
    }
    // Add Plane to planar list.
    tree->polygons.Add(root);
    List front;
    List behind;
    Polygon* poly;
    while ((poly = _polygons.Next()) != NULL) {
        SIDE result = tree->classifyPolygon(poly);
        if (v) {
            if (result == IS_INFRONT)
                printf("FRONT:- %d\n", poly->id);
            else if (result == IS_BEHIND)
                printf("BEHIND:- %d\n", poly->id);
            else if (result == IS_COPLANAR)
                printf("COPLANAR:- %d\n", poly->id);
            else if (result == IS_SPANNING)
                printf("SPANNING:- %d\n", poly->id);
            // printf("<---\n[%f %f %f] \n[%f %f %f] \n[%f %f
            // %f]\n--->\n",poly->vertices[0].x,poly->vertices[0].y,poly->vertices[0].z,poly->vertices[1].x,poly->vertices[1].y,poly->vertices[1].z,poly->vertices[2].x,poly->vertices[2].y,poly->vertices[2].z);
        }
        switch (result) {
            case (IS_INFRONT):
                front.Add(poly);
                break;
            case (IS_BEHIND):
                behind.Add(poly);
                break;
            case (IS_SPANNING):
                behind.Add(poly);
                break;
            case (IS_COPLANAR):
                tree->polygons.Add(poly);
                break;
            default:
                break;
        }
    }
    // If there are polygons in front side node.
    if (front.noofplanes > 0) {
        Node* f = new Node();
        tree->frontNode = f;
        BuildTree(tree->frontNode, front);
    }

    // If there are polygons in back side node.
    if (behind.noofplanes > 0) {
        Node* b = new Node();
        tree->backNode = b;
        BuildTree(tree->backNode, behind);
    }
}

void Node::DrawTree(Node* finalTree, glm::vec3* eyePoint, List* finalFaces) {
    if (finalTree == NULL) {
        return;
    } else {
        Polygon part = finalTree->partition;
        float s = finalTree->classify(&part.normal, eyePoint);
        if (v)
            printf("%f\n", s);
        if (s > 0) // TRAVERSE BACK
        {
            DrawTree(finalTree->backNode, eyePoint, finalFaces);
            // finalFaces->Add(&finalTree->partition);
            // STORE THE COORDINATES
            finalTree->polygons.nextindex = 0;
            Polygon* p = new Polygon();
            while ((p = finalTree->polygons.Next()) != NULL) {
                finalFaces->Add(p);
            }
            finalTree->polygons.nextindex = 0;
            // END OF STORING
            DrawTree(finalTree->frontNode, eyePoint, finalFaces);

        } else // TRAVERSE FRONT
        {
            DrawTree(finalTree->frontNode, eyePoint, finalFaces);
            // finalFaces->Add(&finalTree->partition);
            // STORE THE COORDINATES
            finalTree->polygons.nextindex = 0;
            Polygon* p = new Polygon();
            while ((p = finalTree->polygons.Next()) != NULL) {
                finalFaces->Add(p);
            }
            finalTree->polygons.nextindex = 0;
            // END OF STORING
            DrawTree(finalTree->backNode, eyePoint, finalFaces);
        }
    }
}
