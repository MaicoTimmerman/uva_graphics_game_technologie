/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
 *
 * Student name .... Maico Timmerman, Tim van Zalingen
 * Student email ... maico.timmerman@gmail.com, timvzalingen@gmail.com
 * Collegekaart .... 10542590, 10784012
 * Date ............ 4 februari 2015
 */

#include <math.h>
#include "bezier.h"
#include <stdio.h>

int fact(int n) {
    int fact = 1;
    if (n <= 1) return 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

float binomial(int n, int k) {
    return (float)fact(n) / (float)(fact(k) * fact(n-k));
}


/* Given a Bezier curve defined by the 'num_points' control points
 * in 'p' compute the position of the point on the curve for parameter
 * value 'u', which is the timestep in the curve.
 *
 * Return the x and y values of the point by setting *x and *y,
 * respectively.
 */
void evaluate_bezier_curve(float *x, float *y, control_point p[],
        int num_points, float u) {

    *x = 0.0;
    *y = 0.0;
    float coef;

    printf("x: %f, y: %f\n", *x, *y);
    for (int i = 0; i < num_points; i++) {
        coef = binomial(num_points-1, i);
        printf("control_x: %f\n", (p+i)->x);
        printf("control_y: %f\n", (p+i)->y);
        printf("bern: %f", coef * pow(u, i) * pow(1-u, (num_points-1-i)));
        *x += (coef * pow(u, i) * pow(1-u, (num_points-1-i))) * (p+i)->x;
        *y += (coef * pow(u, i) * pow(1-u, (num_points-1-i))) * (p+i)->y;
    }
    printf("x: %f, y: %f\n", *x, *y);
    return;
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 * Draw the line segments you compute directly on the screen
 * as a single GL_LINE_STRIP. This is as simple as using
 *
 *      glBegin(GL_LINE_STRIP);
 *      glVertex2f(..., ...);
 *      ...
 *      glEnd();
 *
 * DO NOT SET ANY COLOR!
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 */
void draw_bezier_curve(int num_segments, control_point p[], int num_points) {

    float dx = ((p+num_points-1)->x - p->x) / num_segments;
    float x, y = 0;

    glBegin(GL_LINE_STRIP);
    for (x = (float)p->x; x <= (p+num_points-1)->x; x+=dx) {
        evaluate_bezier_curve(&x, &y, p, num_points, x);
        glVertex2f(x, y);
    }
    glEnd();
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
   */
int intersect_cubic_bezier_curve(float *y, control_point p[], float x) {
    printf("%f  %f %f", *y, p->x, x);
    return 0;
}

