/* Computer Graphics, Assignment 5, Orthogonal projection
 *
 * Filename ........ ortho.c
 * Description ..... Contains the re-programmed orthogonal projection matrix
 * Date ............ 01.09.2006
 * Created by ...... Jurgen Sturm 
 *
 * Student name .... Tim van Zalingen & Maico Timmerman
 * Student email ... timvzalingen@gmail.com & maico.timmerman@gmail.com
 * Collegekaart .... 10784012 & 10542590
 * Date ............ 11 Februari 2016
 *
 * This file has the myOrtho function. This function sets a matrix that is used
 * to set up the orthogonal projection. It is used by calling it with the left,
 * right, bottom, top, near and far bounds.
 * 
 * void myOrtho(GLdouble left,
 *              GLdouble right,
 *              GLdouble bottom,
 *              GLdouble top,
 *              GLdouble near,
 *              GLdouble far);
 *
 */
#include <GL/glut.h>   
#include <GL/gl.h>
#include <GL/glu.h>

#define sqr(x) ((x)*(x))

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

/* This function sets a matrix to set up the orthogonal projection for given
 * left, right, bottom, top, near and far points bounds.*/
void myOrtho(GLdouble left,
             GLdouble right,
             GLdouble bottom,
             GLdouble top,
             GLdouble near,
             GLdouble far) {
    GLdouble r_min_l = right - left;
    GLdouble t_min_b = top - bottom;
    GLdouble n_min_f = near - far;
    
    /* This is the M_orth matrix, a combination of a translation to origin
     * matrix and then a scale matrix. */
    GLdouble M_orth[16] = {
        2.0/(r_min_l), 0.0,           0.0,           (right+left)/(right-left),
        0.0,           2.0/(t_min_b), 0.0,           (top+bottom)/(top-bottom),
        0.0,           0.0,           2.0/(n_min_f), (near+far)/(near-far),
        0.0,           0.0,           0.0,           1.0
    };
    glMultMatrixd(M_orth);
}
