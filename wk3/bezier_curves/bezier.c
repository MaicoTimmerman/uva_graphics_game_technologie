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

/* Calculate the factorial of n, fact(0) is 1 */
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

    for (int i = 0; i < num_points; i++) {
        coef = binomial(num_points-1, i);
        *x += (coef * pow(u, i) * pow(1-u, (num_points-1-i))) * (p+i)->x;
        *y += (coef * pow(u, i) * pow(1-u, (num_points-1-i))) * (p+i)->y;
    }
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

    float dx = (float)1 / num_segments;
    float x = 0;
    float y = 0;

    glBegin(GL_LINE_STRIP);
    for (float i = 0; i <= 1.0; i += dx) {
        evaluate_bezier_curve(&x, &y, p, num_points, i);
        glVertex2f(x, y);
    }
    glEnd();
}

void debug_control_point_print(control_point p[]) {
    printf("0: \tx=%f, \ty=%f\n",p->x, p->y);
    printf("1: \tx=%f, \ty=%f\n",(p+1)->x, (p+1)->y);
    printf("2: \tx=%f, \ty=%f\n",(p+2)->x, (p+2)->y);
    printf("3: \tx=%f, \ty=%f\n",(p+3)->x, (p+3)->y);
    printf("4: \tx=%f, \ty=%f\n",(p+4)->x, (p+4)->y);
    printf("5: \tx=%f, \ty=%f\n",(p+5)->x, (p+5)->y);
    printf("6: \tx=%f, \ty=%f\n",(p+6)->x, (p+6)->y);
    printf("7: \tx=%f, \ty=%f\n",(p+7)->x, (p+7)->y);
    printf("8: \tx=%f, \ty=%f\n",(p+8)->x, (p+8)->y);
    printf("9: \tx=%f, \ty=%f\n",(p+9)->x, (p+9)->y);
    printf("10: \tx=%f, \ty=%f\n",(p+10)->x, (p+10)->y);
    printf("11: \tx=%f, \ty=%f\n",(p+11)->x, (p+11)->y);
    printf("12: \tx=%f, \ty=%f\n",(p+12)->x, (p+12)->y);
    printf("13: \tx=%f, \ty=%f\n",(p+13)->x, (p+13)->y);
    printf("14: \tx=%f, \ty=%f\n",(p+14)->x, (p+14)->y);
    printf("15: \tx=%f, \ty=%f\n",(p+15)->x, (p+15)->y);
    printf("16: \tx=%f, \ty=%f\n",(p+16)->x, (p+16)->y);
    printf("17: \tx=%f, \ty=%f\n",(p+17)->x, (p+17)->y);
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
   */
int intersect_cubic_bezier_curve(float *y, control_point p[], float x) {


    /* Check if the points are actually in the curve. */
    if (!(x >= p->x && x <= (p+3)->x)) {
        return 0;
    }

    float nx, ny;
    /* double min = 0.0; */
    /* double max = 1.0; */
    /* double u = (max - min) / 2; */

    /* Calculate the initial u */
    for (float u = 0.0; u < 1.0; u+=0.0001) {
        evaluate_bezier_curve(&nx, &ny, p, 4, u);
        if (fabs(nx-x) < 0.0001) {
            *y = ny;
            return 1;
        }
    }
    return 0;


    /* while (!(fabs(nx - x) < 0.001)) { */
    /*     printf("fabs(xn-x): %f, max: %f, min %f\n", fabs(nx - x), max, min); */
    /*     fflush(stdout); */
    /*     if (nx > x) { */
    /*         max = u; */
    /*     } else { */
    /*         min = u; */
    /*     } */
    /*     u = (max - min) / 2; */
    /*     evaluate_bezier_curve(&nx, &ny, p, 4, u); */
    /* } */
    /*  */
    /* return 0; */
    /* *y = ny; */
    /* return 1; */
}

