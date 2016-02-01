/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Maico Timmerman, Tim van Zalingen
 * Student email ... maico.timmerman@gmail.com, timvzalingen@gmail.com
 * Collegekaart .... 10542590, 10784012
 * Date ............ 5 februari 2015
 */

#include "SDL.h"
#include "init.h"
#include "math.h"

/*
 * Midpoint Line Algorithm
 *
 * As you probably will have figured out, this is the part where you prove
 * your programming skills. The code in the mla function should draw a direct
 * line between (x0,y0) and (x1,y1) in the specified color.
 *
 * Until now, the example code below draws only a horizontal line between
 * (x0,y0) and (x1,y0) and a vertical line between (x1,y1).
 *
 * And here the challenge begins..
 *
 * Good luck!
 *
 *
 */

float calc_d(int x0, int y0, int x1, int y1) {
    float fx0 = (float) x0;
    float fy0 = (float) y0;
    float fx1 = (float) x1;
    float fy1 = (float) y1;

    return (fy0-fy1)*(fx0+1) + (fx1-fx0)*(fy0+0.5) + fx0*fy1 - fx1*fy0;
}

void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
    int d = (int)roundf(calc_d(x0, y0, x1, y1));
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        printf("Drawing %d, %d and d = %d\n", x, y, d);
        PutPixel(s, x, y, colour);
        if (d < 0) {
            y++;
            d += (x1-x0) + (y0-y1);
        } else {
            d += (y0-y1);
        }
    }

    /* int ix,iy; */
    /* int x,y; */
    /*  */
    /* PutPixel(s,x0,y0,colour); */
    /* PutPixel(s,x1,y1,colour); */
    /*  */
    /* if(x1>x0) ix=1; else ix=-1; */
    /* for(x=x0;x!=x1;x+=ix) */
    /*     PutPixel(s,x,y0,colour); */
    /*  */
    /* if(y1>y0) iy=1; else iy=-1; */
    /* for(y=y0;y!=y1;y+=iy) */
    /*     PutPixel(s,x1,y,colour); */


    return;
}

