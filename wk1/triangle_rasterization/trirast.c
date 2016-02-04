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

/* These three functions, f12, f20 and f01 are used below to compute alpha,
 * beta, gamma respectively. */
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

/* This function is used to compute alpga, beta and gamma. */
void compute(float x0, float y0, float x1, float y1, float x2, float y2,
        int x, int y,
        float* alpha, float* beta, float* gamma) {
    *alpha = f12(x0, y0, x1, y1, x2, y2, x, y)/ f12(x0, y0, x1, y1, x2, y2, x0, y0);
    *beta = f20(x0, y0, x1, y1, x2, y2, x, y)/ f20(x0, y0, x1, y1, x2, y2, x1, y1);
    *gamma = f01(x0, y0, x1, y1, x2, y2, x, y)/ f01(x0, y0, x1, y1, x2, y2, x2, y2);
}

/* This function draws a triangle for the given points with the given color
 * per point. This is the unoptimized version. */
void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b) {

    float alpha, beta, gamma;
    
    int off_screen_x = -1;
    int off_screen_y = -1;

    /* The loop runs over each x and then each y for the full framebuffer, uses
     * the barycentric coordinates to determine whether to draw it and for what
     * color. Edges are detected using the off-screen point method. */
    for (int x = 0; x < 128; ++x) {
        for (int y = 0; y < 64; ++y) {
            compute(x0, y0, x1, y1, x2, y2, x, y, &alpha, &beta, &gamma);
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                if ((alpha > 0 || f12(x0, y0, x1, y1, x2, y2, x0, y0) * 
                        f12(x0, y0, x1, y1, x2, y2, off_screen_x, off_screen_y) > 0)
                    && (beta > 0 || f20(x0, y0, x1, y1, x2, y2, x1, y1) * 
                        f20(x0, y0, x1, y1, x2, y2, off_screen_x, off_screen_y) > 0)
                    && (gamma > 0 || f01(x0, y0, x1, y1, x2, y2, x2, y2) * 
                        f01(x0, y0, x1, y1, x2, y2, off_screen_x, off_screen_y) > 0)) {
                    PutPixel(x, y, alpha*r, beta*g, gamma*b);
                }
            }
        }
    }
}

/* These three functions, f12_opt, f20_opt and f01_opt are used below to
 * compute alpha, beta, gamma respectively. For the optimized version. */
float f01_opt(float x0, float y0, float x1, float y1, float x0y1_x1y0,
        float x, float y) {
    return (y0-y1)*x + (x1-x0)*y + x0y1_x1y0;
}
float f12_opt(float x1, float y1, float x2, float y2, float x1y2_x2y1,
        float x, float y) {
    return (y1-y2)*x + (x2-x1)*y + x1y2_x2y1;
}
float f20_opt(float x0, float y0, float x2, float y2, float x2y0_x0y2,
        float x, float y) {
    return (y2-y0)*x + (x0-x2)*y + x2y0_x0y2;
}

/* This function draws a triangle for the given points with the given color
 * per point. This is the optimized version. */
void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float alpha, beta, gamma;
    
    int off_screen_x = -1;
    int off_screen_y = -1;
    
    /* The following calculations are used inside the for loops below. */
    float x0y1_x1y0 = x0*y1 - x1*y0;
    float x1y2_x2y1 = x1*y2 - x2*y1;
    float x2y0_x0y2 = x2*y0 - x0*y2;
    float f_alpha = f12_opt(x1, y1, x2, y2, x1y2_x2y1, x0, y0);
    float f_beta = f20_opt(x0, y0, x2, y2, x2y0_x0y2, x1, y1);
    float f_gamma = f01_opt(x0, y0, x1, y1, x0y1_x1y0, x2, y2);
    float f_alpha_f12 = f_alpha * f12_opt(x1, y1, x2, y2, x1y2_x2y1, off_screen_x, off_screen_y);
    float f_beta_f20 = f_beta * f20_opt(x0, y0, x2, y2, x2y0_x0y2, off_screen_x, off_screen_y);
    float f_gamma_f01 = f_gamma * f01_opt(x0, y0, x1, y1, x0y1_x1y0, off_screen_x, off_screen_y);
    
    /* Below we determine the min and max values for x and y, floored for min
     * and ceiled for max. These are used to set the bounding box. */
    int x_min = (int) x0 < x1 && x0 < x2 ? x0 : x1 < x2 ? x1 : x2;
    int x_max = (int) ceil(x0 > x1 && x0 > x2 ? x0 : x1 > x2 ? x1 : x2);
    int y_min = (int) y0 < y1 && y0 < y2 ? y0 : y1 < y2 ? y1 : y2;
    int y_max = (int) ceil(y0 > y1 && y0 > y2 ? y0 : y1 > y2 ? y1 : y2);
    
    /* The loop runs over each x and y in the bounding box, uses the
     * barycentric coordinates to determine whether to draw it and for what
     * color. Edges are detected using the off-screen point method. */
    for (int x = x_min; x <= x_max; ++x) {
        for (int y = y_min; y <= y_max; ++y) {
            alpha = f12_opt(x1, y1, x2, y2, x1y2_x2y1, x, y)/ f_alpha;
            beta = f20_opt(x0, y0, x2, y2, x2y0_x0y2, x, y)/ f_beta;
            gamma = f01_opt(x0, y0, x1, y1, x0y1_x1y0, x, y)/ f_gamma;
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                if ((alpha > 0 || f_alpha_f12 > 0)
                    && (beta > 0 || f_beta_f20 > 0)
                    && (gamma > 0 || f_gamma_f01 > 0)) {
                    PutPixel(x, y, alpha*r, beta*g, gamma*b);
                }
            }
        }
    }
}
