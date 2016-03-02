/* Computer Graphics and Game Technology, Assignment Ray-tracing
 *
 * Student name .... Maico Timmerman, Tim van Zalingen
 * Student email ... maico.timmerman@gmail.com, timvzalingen@gmail.com
 * Collegekaart .... 10542590, 10784012
 * Date ............ 4 maart 2015
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.
vec3 shade_constant(intersection_point ip) {
    return v3_create(1, 0, 0);
}

// Calculates color of a reflection in the intersection point. The returned
// light is a fraction of the original light intensity based on the dot product
// of the normal vector of the intersection point.
vec3 shade_matte(intersection_point ip) {
    float ci = scene_ambient_light;
    for (int i = 0; i < scene_num_lights; ++i) {
        vec3 light_pos = scene_lights[i].position;

        // Calculate the vector from the point to the light source.
        vec3 li = v3_normalize(v3_subtract(scene_lights[i].position, ip.p));

        if (!shadow_check(v3_multiply(ip.p, 1.001), li)) {
            ci += fmax(0, scene_lights[i].intensity * v3_dotprod(ip.n, li));
        }
    }

    // Cap the maximum light at 100%.
    if (ci > 1.0) ci = 1.0;

    return v3_create(ci, ci, ci);
}

// Calculate the color of a reflection in the intersection point based on the
// Blinn-Phong algorithm. The algorithm exists of an ambient, diffuse and
// specular component.
vec3 shade_blinn_phong(intersection_point ip) {
    float light_diffuse = scene_ambient_light;
    float light_specular = 0;

    float cur_diffuse = 0;
    float cur_specular = 0;

    vec3 cd = v3_create(1.0, 0.0, 0.0);
    vec3 cs = v3_create(1.0, 1.0, 1.0);

    // For all the light sources calculate the diffuse and specular components.
    for (int i = 0; i < scene_num_lights; ++i) {

        // Calculate the vector from the point to the light source.
        vec3 li = v3_normalize(v3_subtract(scene_lights[i].position, ip.p));

        // If there is no shadow interception by an object, calculated the
        // cummulative light strength
        if (!shadow_check(v3_multiply(ip.p, 1.001), li)) {
            cur_diffuse += scene_lights[i].intensity * fmax(0, v3_dotprod(ip.n, li));
        }

        // calulate the halfway vector
        vec3 h = v3_normalize(v3_add(ip.n, li));
        if (!shadow_check(v3_multiply(ip.p, 1.001), h)) {
            cur_specular += scene_lights[i].intensity * pow(fmax(0, v3_dotprod(ip.n, li)), 50);
        }
    }

    light_diffuse += (0.8 * cur_diffuse);
    light_specular += (0.5 * cur_specular);

    return v3_create(light_diffuse+light_specular, 0, 0);
}

// TODO proper comments
vec3 shade_reflection(intersection_point ip) {
    vec3 r = v3_subtract(v3_multiply(ip.n, 2 * v3_dotprod(ip.i, ip.n)), ip.i);

    // Matte reflection makes up for 75% of the of the colour returned.
    vec3 matte = shade_matte(ip);

    // The reflection colour is a new ray shot from the intersection point to
    // the reflection direction r. This makes up for 25% of the colour.
    vec3 refl = ray_color(ip.ray_level + 1, v3_multiply(ip.p, 1.001), r);

    return v3_add(v3_multiply(matte, 0.75), v3_multiply(refl, 0.25));
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
