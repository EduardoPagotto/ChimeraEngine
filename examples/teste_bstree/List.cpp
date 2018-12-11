#include <malloc.h>

#include "List.hpp"

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