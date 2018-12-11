#include "BSPTree.hpp"
#include <malloc.h>
#include <stdio.h>

//  --List

bool v = false;
List::List() {
    List::noofplanes = 0;
    List::nextindex = 0;
}

void List::Add(Polygon* plane) {
    if (List::noofplanes == 0) {
        noofplanes++;
        List::planes = (Polygon*)malloc(noofplanes * sizeof(Polygon));
    } else {
        noofplanes++;
        List::planes = (Polygon*)realloc(List::planes, noofplanes * sizeof(Polygon));
    }
    for (int i = 0; i < 3; i++) {
        List::planes[noofplanes - 1].vertices[i].x = plane->vertices[i].x;
        List::planes[noofplanes - 1].vertices[i].y = plane->vertices[i].y;
        List::planes[noofplanes - 1].vertices[i].z = plane->vertices[i].z;
    }
    List::planes[noofplanes - 1].id = plane->id;
    List::planes[noofplanes - 1].setNormals(plane->normal);
    List::planes[noofplanes - 1].setColor(plane->color);
}

Polygon* List::Next() {
    Polygon* p = NULL;
    if (nextindex == noofplanes)
        return p;
    p = (Polygon*)malloc(sizeof(Polygon));
    for (int i = 0; i < 3; i++) {
        p->vertices[i].x = List::planes[nextindex].vertices[i].x;
        p->vertices[i].y = List::planes[nextindex].vertices[i].y;
        p->vertices[i].z = List::planes[nextindex].vertices[i].z;
    }
    p->id = planes[nextindex].id;
    p->normal.x = List::planes[nextindex].normal.x;
    p->normal.y = List::planes[nextindex].normal.y;
    p->normal.z = List::planes[nextindex].normal.z;
    p->color.x = List::planes[nextindex].color.x;
    p->color.y = List::planes[nextindex].color.y;
    p->color.z = List::planes[nextindex].color.z;
    List::nextindex++;
    return p;
}

//  --Polygon

Polygon::Polygon() {}

Polygon::~Polygon() {}

void Polygon::setNormals(Vertex n) { Polygon::normal = n; }

void Polygon::setVertices(Vertex p1, Vertex p2, Vertex p3) {
    Polygon::vertices[0].setPoint(p1.x, p1.y, p1.z);
    Polygon::vertices[1].setPoint(p2.x, p2.y, p2.z);
    Polygon::vertices[2].setPoint(p3.x, p3.y, p3.z);
}

Vertex Polygon::getColor() { return Polygon::color; }

void Polygon::setColor(Vertex col) {
    Polygon::color.x = col.x;
    Polygon::color.y = col.y;
    Polygon::color.z = col.z;
}

Vertex* Polygon::getVertices() { return Polygon::vertices; }
Vertex Polygon::getNormals() { return Polygon::normal; }

//  --Cube

Cube::Cube() {}
Cube::~Cube() {}
Polygon* Cube::getFaces() { return Cube::faces; }
Vertex* Cube::getNormal() { return Cube::normals; }
void Cube::setFaces(Polygon* face) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            Cube::faces[i].vertices[j].x = face[i].vertices[j].x;
            Cube::faces[i].vertices[j].y = face[i].vertices[j].y;
            Cube::faces[i].vertices[j].z = face[i].vertices[j].z;
        }
    }
}
void Cube::setNormal(Vertex* normal) {
    for (int i = 0; i < 10; i++) {
        Cube::normals[i].x = normal[i].x;
        Cube::normals[i].y = normal[i].y;
        Cube::normals[i].z = normal[i].z;
    }
}

Vertex* Cube::getColors() { return Cube::colors; }

void Cube::setColors(Vertex* cols) {
    for (int i = 0; i < 10; i++)
        Cube::colors[i].setPoint(cols[i].x, cols[i].y, cols[i].z);
}

//  --Vertex

Vertex::Vertex() {}
Vertex::~Vertex() {
    Vertex::x = 0;
    Vertex::y = 0;
    Vertex::z = 0;
}

Vertex::Vertex(float x, float y, float z) {
    Vertex::x = x;
    Vertex::y = y;
    Vertex::z = z;
}

float* Vertex::getPoint() {
    float vertices[3];
    vertices[0] = Vertex::x;
    vertices[1] = Vertex::y;
    vertices[2] = Vertex::z;
    return vertices;
}

void Vertex::setPoint(float x, float y, float z) {
    Vertex::x = x;
    Vertex::y = y;
    Vertex::z = z;
}

// --Octahedron

Octahedron::Octahedron() {}

Octahedron::~Octahedron() {}

Polygon* Octahedron::getFaces() { return Octahedron::faces; }
void Octahedron::setFaces(Polygon* inface) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            Octahedron::faces[i].vertices[j].x = inface[i].vertices[j].x;
            Octahedron::faces[i].vertices[j].y = inface[i].vertices[j].y;
            Octahedron::faces[i].vertices[j].z = inface[i].vertices[j].z;
        }
        Octahedron::faces[i].normal.x = inface[i].normal.x;
        Octahedron::faces[i].normal.y = inface[i].normal.y;
        Octahedron::faces[i].normal.z = inface[i].normal.z;
    }
}

Vertex* Octahedron::getVertices() { return Octahedron::vertices; }

void Octahedron::setVertices(Vertex* verts) {
    for (int i = 0; i < 6; i++) {
        Octahedron::vertices[i].x = verts[i].x;
        Octahedron::vertices[i].y = verts[i].y;
        Octahedron::vertices[i].z = verts[i].z;
    }
}

//  --Node

Node::Node() {
    Node::frontNode = NULL;
    Node::backNode = NULL;
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

float Node::classify(Vertex* normal, Vertex* eye) {
    float A = normal->x;
    float B = normal->y;
    float C = normal->z;
    float D = -(A * normal->x + B * normal->y + C * normal->z);
    float p = A * eye->x + B * eye->y + C * eye->z + D;
    return p;
}

void Node::BuildTree(Node* tree, List polygons) {
    polygons.nextindex = 0;
    // Initilizing the first root node.
    Polygon* root = polygons.Next();
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
    while ((poly = polygons.Next()) != NULL) {
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

void Node::DrawTree(Node* finalTree, Vertex* eyePoint, List* finalFaces) {
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
