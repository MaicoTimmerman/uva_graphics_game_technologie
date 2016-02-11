/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name .... Tim van Zalingen & Maico Timmerman
 * Student email ... timvzalingen@gmail.com & maico.timmerman@gmail.com
 * Collegekaart .... 10784012 & 10542590
 * Date ............ 11 Februari 2016
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

/* This function is used for debugging and prints a 4x4 matrix when given a
 * GLfloat pointer. */
void print_4x4_matrix(GLfloat *A) {
    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0 && i != 0)
            printf("\n");
        else if (i != 0)
            printf(", ");
        printf("%f", A[i]);
        if (i == 15)
            printf("\n");
    }
}

/* This function sets the scaling matrix for a given x, y and z scaling. */
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

/* This function sets the translation for a given translation on x, y and z. */
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

/* This function sets the rotation for a given angle around a vector given by
 * x, y and z. */
void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];

    // 1. Create the orthonormal basis

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
    u[0] = w[1]*t[2] - w[2]*t[1];
    u[1] = w[2]*t[0] - w[0]*t[2];
    u[2] = w[0]*t[1] - w[1]*t[0];

    // Normalize u
    GLfloat len_u = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
    u[0] = u[0] / len_u;
    u[1] = u[1] / len_u;
    u[2] = u[2] / len_u;

    // Compute v = w x u
    v[0] = w[1]*u[2] - w[2]*u[1];
    v[1] = w[2]*u[0] - w[0]*u[2];
    v[2] = w[0]*u[1] - w[1]*u[0];

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.

    // 2. Set up the three matrices making up the rotation

    // Specify matrix A
    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
    angle = angle * M_PI / 180;

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
        u[0], v[0], w[0], 0.0,
        u[1], v[1], w[1], 0.0,
        u[2], v[2], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // 3. Apply the matrices to get the combined rotation

    glMultMatrixf(A);
    glMultMatrixf(B);
    glMultMatrixf(C);
}

