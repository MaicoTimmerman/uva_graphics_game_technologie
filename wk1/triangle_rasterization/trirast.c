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
float f01(float x0, float y0, float x1, float y1,
        float x, float y) {
    return (y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0;
}
float f12(float x1, float y1, float x2, float y2,
        float x, float y) {
    return (y1-y2)*x + (x2-x1)*y + x1*y2 - x2*y1;
}
float f20(float x0, float y0, float x2, float y2,
        float x, float y) {
    return (y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2;
}

/* This function is used to compute alpga, beta and gamma. */
void compute(float x0, float y0, float x1, float y1, float x2, float y2,
        int x, int y,
        float* alpha, float* beta, float* gamma) {
    *alpha = f12(x1, y1, x2, y2, x, y)/ f12(x1, y1, x2, y2, x0, y0);
    *beta = f20(x0, y0, x2, y2, x, y)/ f20(x0, y0, x2, y2, x1, y1);
    *gamma = f01(x0, y0, x1, y1, x, y)/ f01(x0, y0, x1, y1, x2, y2);
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
                if ((alpha > 0 || f12(x1, y1, x2, y2, x0, y0) * 
                        f12(x1, y1, x2, y2, off_screen_x, off_screen_y) > 0)
                    && (beta > 0 || f20(x0, y0, x2, y2, x1, y1) * 
                        f20(x0, y0, x2, y2, off_screen_x, off_screen_y) > 0)
                    && (gamma > 0 || f01(x0, y0, x1, y1, x2, y2) * 
                        f01(x0, y0, x1, y1, off_screen_x, off_screen_y) > 0)) {
                    PutPixel(x, y, alpha*r, beta*g, gamma*b);
                }
            }
        }
    }
}

/* This function draws a triangle for the given points with the given color
 * per point. This is the optimized version. */
void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float alpha2, beta2, gamma2;
    
    int off_screen_x = -1;
    int off_screen_y = -1;
    
    /* The following calculations are used inside the for loops below. */
    float f_alpha = f12(x1, y1, x2, y2, x0, y0);
    float f_beta = f20(x0, y0, x2, y2, x1, y1);
    float f_gamma = f01(x0, y0, x1, y1, x2, y2);
    float f_alpha_f12 = f_alpha * f12(x1, y1, x2, y2, off_screen_x, off_screen_y);
    float f_beta_f20 = f_beta * f20(x0, y0, x2, y2, off_screen_x, off_screen_y);
    float f_gamma_f01 = f_gamma * f01(x0, y0, x1, y1, off_screen_x, off_screen_y);
    
    /* Below we determine the min and max values for x and y, floored for min
     * and ceiled for max. These are used to set the bounding box. */
    int x_min = (int) x0 < x1 && x0 < x2 ? x0 : x1 < x2 ? x1 : x2;
    int x_max = (int) ceil(x0 > x1 && x0 > x2 ? x0 : x1 > x2 ? x1 : x2);
    int y_min = (int) y0 < y1 && y0 < y2 ? y0 : y1 < y2 ? y1 : y2;
    int y_max = (int) ceil(y0 > y1 && y0 > y2 ? y0 : y1 > y2 ? y1 : y2);
    
    float alpha = f12(x1, y1, x2, y2, x_min, y_min);
    float beta = f20(x0, y0, x2, y2, x_min, y_min);
    float gamma = f01(x0, y0, x1, y1, x_min, y_min);
    
    int alpha_partial_y = (x2-x1);
    int beta_partial_y = (x0-x2);
    int gamma_partial_y = (x1-x0);
    
    int alpha_partial_x = (y1-y2);
    int beta_partial_x = (y2-y0);
    int gamma_partial_x = (y0-y1);
    
    int d_alpha_y_loop = ((y_max-y_min+1)*alpha_partial_y);
    int d_beta_y_loop = ((y_max-y_min+1)*beta_partial_y);
    int d_gamma_y_loop = ((y_max-y_min+1)*gamma_partial_y);
    
    /* The loop runs over each x and y in the bounding box, uses the
     * barycentric coordinates to determine whether to draw it and for what
     * color. Edges are detected using the off-screen point method. */
    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            alpha2 = alpha / f_alpha;
            beta2 = beta / f_beta;
            gamma2 = gamma / f_gamma;
            if (alpha2 >= 0 && beta2 >= 0 && gamma2 >= 0) {
                if ((alpha2 > 0 || f_alpha_f12 > 0)
                    && (beta2 > 0 || f_beta_f20 > 0)
                    && (gamma2 > 0 || f_gamma_f01 > 0)) {
                        PutPixel(x, y, alpha2*r, beta2*g, gamma2*b);
                    }
            }
            alpha += alpha_partial_y;
            beta += beta_partial_y;
            gamma += gamma_partial_y;
        }
        alpha += alpha_partial_x - d_alpha_y_loop;
        beta += beta_partial_x  - d_beta_y_loop;
        gamma += gamma_partial_x - d_gamma_y_loop;
    }
}
