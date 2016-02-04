/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Maico Timmerman, Tim van Zalingen
 * Student email ... maico.timmerman@gmail.com, timvzalingen@gmail.com
 * Collegekaart .... 10542590, 10784012
 * Date ............ 5 februari 2015
 */

#include "SDL.h"
#include "init.h"
#include "math.h"

#define OCTANT_0 0
#define OCTANT_1 1
#define OCTANT_2 2
#define OCTANT_3 3
#define OCTANT_4 4
#define OCTANT_5 5
#define OCTANT_6 6
#define OCTANT_7 7

/*
 * Midpoint Line Algorithm
 *
 * Draws a line from (x0, y0) to (x1, y1)  using the Besenham's algorithm.
 * origin. The Besenham's algorithm works for lines in the direction of octant
 * 0. Therefor all lines in other directions are converted to octant 0, by
 * translating the line to the origin, rotating the the line, calculating the
 * new line. Before drawing the pixel, the pixel is rotated back and translated
 * to its' original location.
 */

void to_octant_zero_from(int octant, int x, int y, int* oct0_x, int* oct0_y) {
    /* Convert given x and y from an given octant to octant 0. */

    switch (octant) {
        case 0: *oct0_x =  x; *oct0_y =  y; return;
        case 1: *oct0_x =  x; *oct0_y = -y; return;
        case 2: *oct0_x = -y; *oct0_y =  x; return;
        case 3: *oct0_x = -y; *oct0_y = -x; return;
        case 4: *oct0_x = -x; *oct0_y = -y; return;
        case 5: *oct0_x = -x; *oct0_y =  y; return;
        case 6: *oct0_x =  y; *oct0_y = -x; return;
        case 7: *oct0_x =  y; *oct0_y =  x; return;
    }
}

void from_octant_zero_to(int octant, int x, int y, int* octn_x, int* octn_y) {
    /* Convert given x and y from octant zero to a given octant. */

    switch(octant) {
        case 0: *octn_x =  x; *octn_y =  y; return;
        case 1: *octn_x =  x; *octn_y = -y; return;
        case 2: *octn_x =  y; *octn_y = -x; return;
        case 3: *octn_x = -y; *octn_y = -x; return;
        case 4: *octn_x = -x; *octn_y = -y; return;
        case 5: *octn_x = -x; *octn_y =  y; return;
        case 6: *octn_x = -y; *octn_y =  x; return;
        case 7: *octn_x =  y; *octn_y =  x; return;
    }
}

void translate_coords_to_origin(int x0, int x1, int y0, int y1,
        int* tx1, int* ty1) {
    *tx1 = x1 - x0;
    *ty1 = y1 - y0;
}

/*
 * Octants:
 *  \3|2/
 *  4\|/1
 *  --+--
 *  5/|\0
 *  /6|7\
 */
int detect_octant(int x0, int x1, int y0, int y1) {
    int x_diff = x1 - x0;
    int y_diff = y1 - y0;
    int d_diff = abs(x_diff) - abs(y_diff);

    if (x_diff >= 0) {
        // Direction right of p0
        if (y_diff >= 0) {
            // Direction under p0
            if (d_diff >= 0) {
                return OCTANT_0; //
            } else {
                return OCTANT_7; //
            }
        } else{
            // Direction above p0
            if (d_diff >= 0) {
                return OCTANT_1;
            } else {
                return OCTANT_2;
            }
        }
    } else {
        // Direction left of p0
        if (y_diff >= 0) {
            // Direction under p0
            if (d_diff >= 0) {
                return OCTANT_5;
            } else {
                return OCTANT_6;
            }
        } else{
            // Direction above p0
            if (d_diff >= 0) {
                return OCTANT_4;
            } else {
                return OCTANT_3;
            }
        }
    }
    return -1;
}

float calc_d(int x0, int y0, int x1, int y1) {
    float fy0 = (float) y0;

    return (fy0-y1)*(x0+1) + (x1-x0)*(y0+0.5) + x0*y1 - x1*y0;
}

void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
    int octant;
    int oct0_x1, oct0_y1;
    int ori_x, ori_y;
    int ox, oy;
    int tx1, ty1;

    octant = detect_octant(x0, x1, y0, y1);
    translate_coords_to_origin(x0, x1, y0, y1, &tx1, &ty1);

    to_octant_zero_from(octant, tx1, ty1, &oct0_x1, &oct0_y1);

    int d = (int)roundf(calc_d(0, 0, oct0_x1, oct0_y1));
    int y = 0;

    /* Loop over all projected x values of a line to determine the projected y
     * value and draw this pixel after converting back to the original octant.
     */
    for (int x = 0; x <= oct0_x1; x++) {

        from_octant_zero_to(octant, x, y, &ori_x, &ori_y);
        translate_coords_to_origin(-1*x0, ori_x, -1*y0, ori_y, &ox, &oy);

        PutPixel(s, ox, oy, colour);
        /* Set the check value for the pixel to the right. If the line is going through the current y+1, if true, raise the y value accordingly */
        if (d < 0) {
            y++;
            d += (oct0_x1-x) + (y-oct0_y1);
        } else {
            d += (y-oct0_y1);
        }
    }

    return;
}
