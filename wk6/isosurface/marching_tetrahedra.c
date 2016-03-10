/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

tetrahedron T[6] = {
    {.p[0] = 0, .p[1] = 1, .p[2] = 3, .p[3] = 7}, // T1
    {.p[0] = 0, .p[1] = 2, .p[2] = 6, .p[3] = 7}, // T2
    {.p[0] = 0, .p[1] = 1, .p[2] = 5, .p[3] = 7}, // T3
    {.p[0] = 0, .p[1] = 2, .p[2] = 3, .p[3] = 7}, // T4
    {.p[0] = 0, .p[1] = 4, .p[2] = 5, .p[3] = 7}, // T5
    {.p[0] = 0, .p[1] = 4, .p[2] = 6, .p[3] = 7}  // T6
};

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */

    static vec3
interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2, unsigned char v1, unsigned char v2)
{
    /* Initially, simply return the midpoint between p1 and p2.
       So no real interpolation is done yet */

    return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));

    float fv1 = (float) v1;
    float fv2 = (float) v2;
    float iso = (float) isovalue;

    float length = abs(fv1 - fv2);
    float ratio_p1 = (abs(fv1 - iso) / length);
    float ratio_p2 = (abs(fv2 - iso) / length);

    return v3_add(v3_multiply(p1, ratio_p1), v3_multiply(p2, ratio_p2));
}


/* Generate the triangle located in the corner of v0 */
static triangle generate_corner_triangle(unsigned char isovalue, cell c,
        int v0, int v1, int v2, int v3) {
    triangle t;
    t.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
    t.p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2], c.value[v0], c.value[v2]);
    t.p[2] = interpolate_points(isovalue, c.p[v0], c.p[v3], c.value[v0], c.value[v3]);

    // TODO calc normal vectors
    return t;
}

static triangle generate_squared_triangle(unsigned char isovalue, cell c,
        int v0, int v1, int v2, int v3) {
    triangle t;
    t.p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
    t.p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
    t.p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);

    // TODO calc normal vectors
    return t;
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

Note: the output array "triangles" should have space for at least
2 triangles.
*/
    static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue,
        cell c, int v0, int v1, int v2, int v3)
{
    unsigned char bitmask = 0x0;



    if (isovalue < c.value[v0]) bitmask |= 0x1;
    if (isovalue < c.value[v1]) bitmask |= 0x2;
    if (isovalue < c.value[v2]) bitmask |= 0x4;
    if (isovalue < c.value[v3]) bitmask |= 0x8;

    // The first 8 cases (0x0 -> 0x7) are interchangeable with the last cases
    /* printf("c.value[v0]: %lf\n", c.value[v0]); */
    /* printf("c.value[v1]: %lf\n", c.value[v1]); */
    /* printf("c.value[v2]: %lf\n", c.value[v2]); */
    /* printf("c.value[v3]: %lf\n", c.value[v3]); */
    /* printf("isovalue: %d\n", isovalue); */
    /* printf("bitmask before: %d\n", bitmask); */
    /* if (bitmask > 0x7) bitmask = ~bitmask; */
    /* printf("bitmask after: %d\n", bitmask); */
    fflush(stdout);

    if (bitmask == 0x0 || bitmask == 0xF) { // 0000 and 1111
        return 0;
    }
    if (bitmask == 0x1 || bitmask == 0xE) { // 0001 and 1110
        *triangles = generate_corner_triangle(isovalue, c, v0, v1, v2, v3);
        return 1;
    }
    if (bitmask == 0x2 || bitmask == 0xD) { // 0010 and 1101
        *triangles = generate_corner_triangle(isovalue, c, v1, v0, v2, v3);
        return 1;
    }
    if (bitmask == 0x3 || bitmask == 0xC) { // 0011 and 1100
        *triangles = generate_corner_triangle(isovalue, c, v2, v0, v3, v1);
        *(triangles+1) = generate_corner_triangle(isovalue, c, v3, v1, v2, v0);
        return 2;
    }
    if (bitmask == 0x4 || bitmask == 0xB) { // 0100 and 1011
        *triangles = generate_corner_triangle(isovalue, c, v2, v0, v1, v3);
        return 1;
    }
    if (bitmask == 0x5 || bitmask == 0xA) { // 0101 and 1010
        *triangles = generate_corner_triangle(isovalue, c, v2, v3, v0, v1);
        *(triangles+1) = generate_corner_triangle(isovalue, c, v3, v2, v1, v0);
        return 2;
    }
    if (bitmask == 0x6 || bitmask == 0x9) { // 0110 and 1001
        *triangles = generate_corner_triangle(isovalue, c, v3, v1, v0, v2);
        *(triangles+1) = generate_corner_triangle(isovalue, c, v1, v3, v2, v0);
        return 2;
    }
    if (bitmask == 0x7 || bitmask == 0x8) { // 0111 and 0111
        *triangles = generate_corner_triangle(isovalue, c, v3, v0, v1, v2);
        return 1;
    }

    return 0;
}

/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
   */

int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue) {

    int num_triangles = 0;

    for (int i = 0; i < 8; ++i) {
        /* printf("\tvolume: %lf\n", c.value[i]); */
    }

    // For all the tetrahedra, call the triangle function for all the corners.
    for (int i = 0; i < 6; i++) {
        num_triangles += generate_tetrahedron_triangles(triangles+num_triangles,
                isovalue, c, T[i].p[0], T[i].p[1],T[i].p[2],T[i].p[3]);
    }

    return num_triangles;
}
