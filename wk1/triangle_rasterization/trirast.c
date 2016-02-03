/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
 * Created by ...... Paul Melis
 *
 * Student name .... Maico Timmerman, Tim van Zalingen
 * Student email ... maico.timmerman@gmail.com, timvzalingen@gmail.com
 * Collegekaart .... 10542590, 10784012
 * Date ............ 5 februari 2015
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 *
 * red = p0
 * green = p1
 * blue = p2
 */

float f01(float x0, float y0, float x1, float y1, float x2, float y2,
        float i, float j) {
    return (y0-y1)*i + (x1-x0)*j + x0*y1 - x1*y0;
}
float f12(float x0, float y0, float x1, float y1, float x2, float y2,
        float i, float j) {
    return (y1-y2)*i + (x2-x1)*j + x1*y2 - x2*y1;
}
float f20(float x0, float y0, float x1, float y1, float x2, float y2,
        float i, float j) {
    return (y2-y0)*i + (x0-x2)*j + x2*y0 - x0*y2;
}


void compute(float x0, float y0, float x1, float y1, float x2, float y2,
        int i, int j,
        float* a, float* b, float* y) {
    *a = f12(x0, y0, x1, y1, x2, y2, i, j)/ f12(x0, y0, x1, y1, x2, y2, x0, y0);
    *b = f20(x0, y0, x1, y1, x2, y2, i, j)/ f20(x0, y0, x1, y1, x2, y2, x1, y1);
    *y = f01(x0, y0, x1, y1, x2, y2, i, j)/ f01(x0, y0, x1, y1, x2, y2, x2, y2);

}

void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float alpha, beta, gamma;

    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            compute(x0, y0, x1, y1, x2, y2, i, j, &alpha, &beta, &gamma);
            if ((alpha <= 1 || alpha >= 0)
                    && (beta <= 1 || beta >= 0)
                    && (gamma <= 1 || gamma >= 0)) {
                PutPixel(i, j, (byte)alpha*r, (byte)beta*g, (byte)gamma*b);
            }
        }
    }

    //PutPixel(1,1, 0, 255, 0);
    //PutPixel(2,2, 0, 255, 0);
    //PutPixel(1,2, 0, 255, 0);
    //PutPixel(2,1, 0, 255, 0);
}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    PutPixel(1,1, 255, 0, 0);
    PutPixel(2,2, 255, 0, 0);
    PutPixel(1,2, 255, 0, 0);
    PutPixel(2,1, 255, 0, 0);
}
