#ifndef MARCHING_TETRAHEDRA_H
#define MARCHING_TETRAHEDRA_H

#include "v3math.h"
#include "volume.h"

typedef struct
{
    vec3    p[3];
    vec3    n[3];
}
triangle;


typedef struct
{
    int p[4];
} tetrahedron;

static inline tetrahedron
th_create(int v0, int v1, int v2, int v3) {
    tetrahedron t;
    t.p[0] = v0;
    t.p[1] = v1;
    t.p[2] = v2;
    t.p[3] = v3;
    return t;
}

int generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue);

#endif
