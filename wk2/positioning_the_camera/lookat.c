/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Tim van Zalingen & Maico Timmerman
 * Student email ... timvzalingen@gmail.com & maico.timmerman@gmail.com
 * Collegekaart .... 10784012 & 10542590
 * Date ............ 12 Februari 2016
 */
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI 3.14159265358979323846  /* pi */
#endif

/* Take the crossproduct of vectors a and b and place the results in the
 * out vector */
void cross(GLdouble *a, GLdouble *b, GLdouble *out) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

/* Normalize the vector a by dividing all components by the vectors length. */
void normalize(GLdouble *a) {
    GLdouble norm = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);

    a[0] = a[0] / norm;
    a[1] = a[1] / norm;
    a[2] = a[2] / norm;
}

/* Translate coordinates of the world to coordinates for the camera */
void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ) {

    GLdouble up[3] = {upX, upY, upZ};
    GLdouble cz[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    GLdouble cx[3], cy[3];

    /* Translate the camera to the origin */
    glPushMatrix();
    glTranslatef(-eyeX, -eyeY, -eyeZ);
    glPopMatrix();

    normalize(cz);

    // cx = up x cz
    cross(up, cz, cx);
    normalize(cx);


    // cy = cz x cx
    cross(cz, cx, cy);
    normalize(cy);

    GLdouble V[16] = {
        -cx[0], cy[0], -cz[0], 0.0,
        -cx[1], cy[1], -cz[1], 0.0,
        -cx[2], cy[2], -cz[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixd(V);
}
