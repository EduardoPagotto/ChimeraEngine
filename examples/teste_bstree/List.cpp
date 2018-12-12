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

    for (int i = 0; i < 3; i++)
        List::planes[noofplanes - 1].vertices[i] = plane->vertices[i];

    List::planes[noofplanes - 1].id = plane->id;
    List::planes[noofplanes - 1].normal = plane->normal;
    List::planes[noofplanes - 1].color = plane->color;
}

Polygon* List::Next() {

    Polygon* p = NULL;
    if (nextindex == noofplanes)
        return p;

    p = (Polygon*)malloc(sizeof(Polygon));
    for (int i = 0; i < 3; i++)
        p->vertices[i] = List::planes[nextindex].vertices[i];

    p->id = planes[nextindex].id;
    p->normal = List::planes[nextindex].normal;
    p->color = List::planes[nextindex].color;

    List::nextindex++;
    return p;
}