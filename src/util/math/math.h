//
// Created by sneeuwpop on 23-6-19.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <string>
#include "../external/unit_test.hpp"
using namespace glm;

#define S(X) std::to_string(X).substr(0, 4)


namespace MATH
{
    const float PI = 3.1415926535f;

    inline float random()
    {
        return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }

    inline float random(const float min, const float max)
    {
        return min + static_cast <float> (rand()) /  static_cast <float> (RAND_MAX / (max - min));
    }

    /// This is based on Method 10. Polar from:
    /// http://extremelearning.com.au/how-to-generate-uniformly-random-points-on-n-spheres-and-n-balls/
    inline vec3 randomPointOnSphere(float maxDegreeTop=180, float maxDegreeSide=360)
    {
        float u = random(), v = random();

        float theta = 2*PI * u;
        theta *= (maxDegreeTop / 360.0f);
        theta += (1.0f / 2.0f) * PI; // rotate to the top

        float // 2 circle
            x = cos(theta),
            y = sin(theta),
            z = 0;

        float phi = 2*PI * v;
        phi *= (maxDegreeSide / 360.0f);
        phi *= ((y < 0)? 1.0f : -1.0f); // flip the angle so it stays on the same side

        return glm::rotate(vec3(x,y,z), phi, vec3(0,sin(theta),0));
    }
    TEST(randomPointOnSphere,  return abs(glm::distance(vec3(0), randomPointOnSphere()) -1.0f) < 0.001f; )
    TEST(randomPointOnSphere2, return abs(glm::distance(vec3(0), randomPointOnSphere(rand()/360)) -1.0f) < 0.001f; )
    TEST(randomPointOnSphere3, return abs(glm::distance(vec3(0), randomPointOnSphere(rand()/360,rand()/360)) -1.0f) < 0.001f; )
}


namespace VEC3
{
    const vec3
            ZERO = vec3(0, 0, 0),
            ONE  = vec3(1, 1, 1),
            X    = vec3(1, 0, 0),
            Y    = vec3(0, 1, 0),
            Z    = vec3(0, 0, 1);

    inline std::string toString(vec3 vec)
    {
        return "("+S(vec.x)+", "+S(vec.y)+", "+S(vec.z)+")";
    }
}


namespace COLOR
{
    const vec4
            DEFAULT    = vec4(1,0,1,1), // pink is the default
            BLACK      = vec4(0,0,0,1),
            WHITE      = vec4(1,1,1,1),
            RED        = vec4(1,0,0,1),
            GREEN      = vec4(0,1,0,1),
            BLUE       = vec4(0,0,1,1),
            YELLOW     = vec4(1,1,0,1),
            PINK       = vec4(1,0,1,1),
            LIGHT_BLUE = vec4(0,1,1,1);
}


namespace MAT4
{
    const mat4 IDENTITY = mat4(1.0);

    inline std::string toString(mat4 mat)
    {
        const float *m = (const float*)glm::value_ptr(mat);
        return "["+S(m[0 ])+", "+S(m[1 ])+", "+S(m[2 ])+", "+S(m[3 ])+", \n"+
               " "+S(m[4 ])+", "+S(m[5 ])+", "+S(m[6 ])+", "+S(m[7 ])+", \n"+
               " "+S(m[8 ])+", "+S(m[9 ])+", "+S(m[10])+", "+S(m[11])+", \n"+
               " "+S(m[12])+", "+S(m[13])+", "+S(m[14])+", "+S(m[15])+"] \n";
    }
}