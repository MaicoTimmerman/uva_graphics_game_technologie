/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name .... Tim van Zalingen & Maico Timmerman
 * Student email ... timvzalingen@gmail.com & maico.timmerman@gmail.com
 * Collegekaart .... 10784012 & 10542590
 * Date ............ 12 Februari 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "transformations.h"

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myScalef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixf(M);
}


void myTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    };

    glMultMatrixf(M);
}

void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];

    //
    // 1. Create the orthonormal basis
    //

    // Store the incoming rotation axis in w and normalize w
    GLfloat ori_angle = angle;
    GLfloat ori_x = x;
    GLfloat ori_y = y;
    GLfloat ori_z = z;

    // Compute the norm
    GLfloat len_a = sqrt(x*x + y*y + z*z);
    //
    // Normalize the vector
    w[0] = x/len_a;
    w[1] = y/len_a;
    w[2] = z/len_a;

    t[0] = w[0];
    t[1] = w[1];
    t[2] = w[2];

    // Set the smallest component of t to 1.
    t[0] < t[1] && t[0] < t[2] ? (t[0] = 1) : t[1] < t[2] ? (t[1] = 1) : (t[2] = 1);


    // Compute u = t x w
    u[0] = t[1]*w[2] - t[2]*w[1];
    u[1] = t[2]*w[0] - t[0]*w[2];
    u[2] = t[0]*w[1] - t[1]*w[0];

    // Normalize u
    GLfloat len_u = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
    u[0] = u[0] / len_u;
    u[1] = u[1] / len_u;
    u[2] = u[2] / len_u;

    // Compute v = w x u
    v[0] = w[1]*u[2] - w[2]*u[1];
    v[1] = w[2]*u[0] - w[0]*u[2];
    v[2] = w[0]*u[1] - w[1]*u[0];

    // Normalize v
    // Not neccessary?
    /* GLfloat len_v = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); */
    /* v[0] = v[0] / len_v; */
    /* v[1] = v[1] / len_v; */
    /* v[2] = v[2] / len_v; */


    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.

    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A
    printf("u: (%f, %f, %f)", u[0], u[1], u[2]);
    printf("v: (%f, %f, %f)", v[0], v[1], v[2]);
    printf("w: (%f, %f, %f)", w[0], w[1], w[2]);

    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians

    // Specify matrix B

    GLfloat B[16] =
    {
        cos(angle), sin(angle), 0.0, 0.0,
        -sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Specify matrix C

    GLfloat C[16] =
    {
        u[0], v[1], w[2], 0.0,
        u[0], v[1], w[2], 0.0,
        u[0], v[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    //
    // 3. Apply the matrices to get the combined rotation
    //

    glMultMatrixf(A);
    glMultMatrixf(B);
    glMultMatrixf(C);
}

