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
 *
 *
 * (always fill in these fields before submitting!!)
 */
#include <GL/glut.h>   
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define CROSS(a, b) {a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], \
                     a[0]*b[1]-a[1]*b[0]}
#define NORMALIZE(a, l) a[0] = a[0] / l; \
                        a[1] = a[1] / l; \
                        a[2] = a[2] / l;
 
/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ) {
    glPushMatrix();
    glTranslatef(-eyeX, -eyeY, -eyeZ);
    glPopMatrix();
    
    GLdouble eye[3] = {eyeX, eyeY, eyeZ};
    GLdouble center[3] = {centerX, centerY, centerZ};
    GLdouble up[3] = {upX, upY, upZ};
    
    GLdouble cz[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    NORMALIZE(cz, sqrt(cz[0]*cz[0] + cz[1]*cz[1] + cz[2]*cz[2]));
    
    GLdouble cx[3] = CROSS(up, cz);
    NORMALIZE(cx, sqrt(cx[0]*cx[0] + cx[1]*cx[1] + cx[2]*cx[2]));

    GLdouble cy[3] = CROSS(cz, cx);
    NORMALIZE(cy, sqrt(cy[0]*cy[0] + cy[1]*cy[1] + cy[2]*cy[2]));
    
//     NORMALIZE(eye, sqrt(eye[0]*eye[0] + eye[1]*eye[1] + eye[2]*eye[2]))
//     GLdouble dx[3] = {-eye[0]*u[0], -eye[1]*u[1], -eye[2]*u[2]}
//     GLdouble dy[3] = {-eye[0]*v[0], -eye[1]*v[1], -eye[2]*v[2]}
//     GLdouble dz[3] = {-eye[0]*n[0], -eye[1]*n[1], -eye[2]*n[2]}
    
    GLdouble V[16] = {
        -cx[0], cy[0], -cz[0], 0.0,
        -cx[1], cy[1], -cz[1], 0.0,
        -cx[2], cy[2], -cz[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    glMultMatrixd(V);
}
