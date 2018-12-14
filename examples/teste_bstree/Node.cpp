#include <stdio.h>

#include "Node.hpp"

Node::Node() {
    Node::frontNode = NULL;
    Node::backNode = NULL;
    v = true;
}

SIDE Node::classifyPolygon(Polygon* cPolygon) {

    glm::vec3 n1 = partition.getNormal();
    glm::vec3 v1 = partition.getVertices()[0];
    float dotVal1 = -(glm::dot(n1, v1));

    float p[3];
    for (int i = 0; i < 3; i++)
        p[i] = glm::dot(n1, cPolygon->getVertices()[i]) + dotVal1;

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

    // FIXME: esta errado, verificar original!!!
    float A = normal->x;
    float B = normal->y;
    float C = normal->z;
    float D = -(A * normal->x + B * normal->y + C * normal->z);
    float p = A * eye->x + B * eye->y + C * eye->z + D;
    return p;
}

void Node::BuildTree(Node* tree, ListPolygon _polygons) {

    _polygons.resetNext();
    // Initilizing the first root node.
    Polygon* root = _polygons.Next();
    tree->partition = *root;
    if (v) {
        printf("ROOT:- %d\n", root->getId());
        printf("<---\n[%f %f %f] \n[%f %f %f] \n[%f %f
        
        %f]\n--->\n",root->vertices[0].x,root->vertices[0].y,root->vertices[0].z,root->vertices[1].x,root->vertices[1].y,root->vertices[1].z,root->vertices[2].x,root->vertices[2].y,root->vertices[2].z);
    }

    // Add Plane to planar list.
    tree->polygons.addToList(root);
    ListPolygon front;
    ListPolygon behind;
    Polygon* poly;
    while ((poly = _polygons.Next()) != NULL) {
        SIDE result = tree->classifyPolygon(poly);
        if (v) {
            if (result == IS_INFRONT)
                printf("FRONT:- %d\n", poly->getId());
            else if (result == IS_BEHIND)
                printf("BEHIND:- %d\n", poly->getId());
            else if (result == IS_COPLANAR)
                printf("COPLANAR:- %d\n", poly->getId());
            else if (result == IS_SPANNING)
                printf("SPANNING:- %d\n", poly->getId());
            printf("<---\n[%f %f %f] \n[%f %f %f] \n[%f %f%f]\n--->\n", poly->vertices[0].x, poly->vertices[0].y,
                   poly->vertices[0].z, poly->vertices[1].x, poly->vertices[1].y, poly->vertices[1].z,
                   poly->vertices[2].x, poly->vertices[2].y, poly->vertices[2].z);
        }
        switch (result) {
            case (IS_INFRONT):
                front.addToList(poly);
                break;
            case (IS_BEHIND):
                behind.addToList(poly);
                break;
            case (IS_SPANNING):
                behind.addToList(poly);
                break;
            case (IS_COPLANAR):
                tree->polygons.addToList(poly);
                break;
            default:
                break;
        }
    }

    // If there are polygons in front side node.
    if (front.size() > 0) {
        tree->frontNode = new Node();
        BuildTree(tree->frontNode, front);
    }

    // If there are polygons in back side node.
    if (behind.size() > 0) {
        tree->backNode = new Node();
        BuildTree(tree->backNode, behind);
    }
}

void Node::DrawTree(Node* finalTree, glm::vec3* eyePoint, ListPolygon* finalFaces) {

    if (finalTree == NULL)
        return;

    // Polygon part = finalTree->partition;
    glm::vec3 part = finalTree->partition.getNormal();
    float s = finalTree->classify(&part, eyePoint);

    if (v)
        printf("%f\n", s);

    Polygon* p = nullptr;

    if (s > 0) {
        // TRAVERSE BACK
        DrawTree(finalTree->backNode, eyePoint, finalFaces);

        // finalFaces->Add(&finalTree->partition);
        // STORE THE COORDINATES
        finalTree->polygons.begin();
        while ((p = finalTree->polygons.next()) != NULL)
            finalFaces->addToList(p);

        // END OF STORING
        finalTree->polygons.begin();
        DrawTree(finalTree->frontNode, eyePoint, finalFaces);

    } else {
        // TRAVERSE FRONT
        DrawTree(finalTree->frontNode, eyePoint, finalFaces);

        // finalFaces->Add(&finalTree->partition);
        // STORE THE COORDINATES
        finalTree->polygons.begin();
        while ((p = finalTree->polygons.next()) != NULL)
            finalFaces->addToList(p);

        // END OF STORING
        finalTree->polygons.begin();
        DrawTree(finalTree->backNode, eyePoint, finalFaces);
    }
}
