#pragma once

#include <embree3/rtcore_device.h>
#include <embree3/rtcore_geometry.h>
#include <vector>
#include <string>
// #include <embree3/rtcore.h>
#include "camera.hpp"
#include "material.hpp"
#include "simplegui.hpp"
#include "surface.hpp"
// #include "surface.h"
// #include "camera.h"

/*! \class Raytracer
\brief General ray tracer class.

\author Tomáš Fabián
\version 0.1
\date 2018
*/
class Raytracer : public SimpleGui
{
public:
    Raytracer( const int width, const int height,
        const float fov_y, const Vector3 view_from, const Vector3 view_at,
        const char * config = "threads=0,verbose=3" );
    ~Raytracer();

    int InitDeviceAndScene( const char * config );

    int ReleaseDeviceAndScene();

    void LoadScene( const std::string file_name );

    Color4f get_pixel( const int x, const int y, const float t = 0.0f ) override;

    int Ui();

private:
    std::vector<Surface *> surfaces_;
    std::vector<Material *> materials_;

    RTCDevice device_;
    RTCScene scene_;
    Camera camera_;
};
