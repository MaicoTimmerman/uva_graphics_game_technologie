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
        float x, float y) {
    return (y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0;
}
float f12(float x0, float y0, float x1, float y1, float x2, float y2,
        float x, float y) {
    return (y1-y2)*x + (x2-x1)*y + x1*y2 - x2*y1;
}
float f20(float x0, float y0, float x1, float y1, float x2, float y2,
        float x, float y) {
    return (y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2;
}


void compute(float x0, float y0, float x1, float y1, float x2, float y2,
        int x, int y,
        float* alpha, float* beta, float* gamma) {
    *alpha = f12(x0, y0, x1, y1, x2, y2, x, y)/ f12(x0, y0, x1, y1, x2, y2, x0, y0);
    *beta = f20(x0, y0, x1, y1, x2, y2, x, y)/ f20(x0, y0, x1, y1, x2, y2, x1, y1);
    *gamma = f01(x0, y0, x1, y1, x2, y2, x, y)/ f01(x0, y0, x1, y1, x2, y2, x2, y2);
}

void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b) {

    float alpha, beta, gamma;

    for (int x = 0; x < 128; ++x) {
        for (int y = 0; y < 64; ++y) {
            compute(x0, y0, x1, y1, x2, y2, x, y, &alpha, &beta, &gamma);
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                PutPixel(x, y, alpha*r, beta*g, gamma*b);
            }
        }
    }
}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float alpha, beta, gamma;

    float f_alpha = f12(x0, y0, x1, y1, x2, y2, x0, y0);
    float f_beta = f20(x0, y0, x1, y1, x2, y2, x1, y1);
    float f_gamma = f01(x0, y0, x1, y1, x2, y2, x2, y2);
    
    int x_min = (int) x0 < x1 && x0 < x2 ? x0 : x1 < x2 ? x1 : x2;
    int x_max = (int) ceil(x0 > x1 && x0 > x2 ? x0 : x1 > x2 ? x1 : x2);
    int y_min = (int) y0 < y1 && y0 < y2 ? y0 : y1 < y2 ? y1 : y2;
    int y_max = (int) ceil(y0 > y1 && y0 > y2 ? y0 : y1 > y2 ? y1 : y2);
    
    for (int x = x_min; x < x_max; ++x) {
        for (int y = y_min; y < y_max; ++y) {
            alpha = f12(x0, y0, x1, y1, x2, y2, x, y)/ f_alpha;
            beta = f20(x0, y0, x1, y1, x2, y2, x, y)/ f_beta;
            gamma = f01(x0, y0, x1, y1, x2, y2, x, y)/ f_gamma;
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                PutPixel(x, y, alpha*r, beta*g, gamma*b);
            }
        }
    }
}
