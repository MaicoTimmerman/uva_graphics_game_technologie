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

#include <stdio.h>
#include <stdlib.h>

#include "volume.h"

/* The voxels of the volume dataset, stored as a one-dimensional array */
unsigned char   *volume;

/* The dimensions of the volume dataset */
int     nx, ny, nz;

/* The size of a voxel */
float   sizex, sizey, sizez;

/* Utility function to convert the index of a voxel
   into an index in the volume array above */
int
voxel2idx(int i, int j, int k)
{
    return (k*ny + j)*nx + i;
}


/* Extract a cell from the volume, so that datapoint 0 of the
   cell corresponds to voxel (i, j, k), datapoint 1 to voxel (i+1, j, k),
   etc. See the assignment. */
cell
get_cell(int i, int j, int k)
{
    cell c;
    vec3 cell_corners[8] = {
        v3_create(0, 0, 0),
        v3_create(1, 0, 0),
        v3_create(0, 1, 0),
        v3_create(1, 1, 0),
        v3_create(0, 0, 1),
        v3_create(1, 0, 1),
        v3_create(0, 1, 1),
        v3_create(1, 1, 1)
    };

    // cell.p contains the 8 cornerpoints of the cell, in specific order
    vec3 root = v3_create(i, j, k);

    for (int l = 0; l < 8; l++) {
        c.p[l] = v3_add(root, cell_corners[l]);
        if (i+1 >= nx || j+1 >= ny || k+1 >= nz) {
            c.value[l] = 0;
        } else {
            c.value[l] = volume[voxel2idx(c.p[l].x, c.p[l].y, c.p[l].z)];
        }
    }
    return c;
}

/* Utility function to read a volume dataset from a VTK file.
   This will store the data in the "volume" array and update the dimension
   and size values. */

void
read_volume(const char *fname)
{
    FILE *f;
    char s[256];
    int nvoxels;

    printf("Reading %s\n", fname);
    f = fopen(fname, "rb");

    if (!f)
    {
        fprintf(stderr, "read_volume(): Could not open file '%s' for reading!\n", fname);
        exit(-1);
    }

    // header line
    fgets(s, 255, f);

    // comment line
    fgets(s, 255, f);

    // BINARY
    fgets(s, 255, f);

    // DATASET STRUCTURED_POINTS
    fgets(s, 255, f);

    // DIMENSIONS %d %d %d
    fscanf(f, "%s %d %d %d\n", s, &nx, &ny, &nz);
    printf("%d x %d x %d voxels\n", nx, ny, nz);

    // ASPECT_RATIO/SPACING %f %f %f
    fscanf(f, "%s %f %f %f\n", s, &sizex, &sizey, &sizez);
    printf("voxel sizes: %.3f, %.3f, %.3f\n", sizex, sizey, sizez);

    // ORIGIN ...
    fgets(s, 255, f);

    // POINT_DATA ...
    fgets(s, 255, f);

    // SCALARS ...
    fgets(s, 255, f);

    // LOOKUP_TABLE ...
    fgets(s, 255, f);

    // allocate memory to hold the volume data and read it from file
    nvoxels = nx * ny * nz;
    volume = (unsigned char*)malloc(nvoxels);

    if (fread(volume, 1, nvoxels, f) < (size_t)nvoxels)
    {
        printf("WARNING: not all data could be read!\n");
    }

    fclose(f);
}
