#include "raytracer.hpp"
#include "3rdparty/imgui/imgui.h"
#include "objloader.hpp"
#include "tutorials.hpp"

Raytracer::Raytracer( const int width, const int height,
    const float fov_y, const Vector3 view_from, const Vector3 view_at,
    const char * config ) : SimpleGui( width, height )
{
    InitDeviceAndScene( config );

    camera_ = Camera( width, height, fov_y, view_from, view_at );
}

Raytracer::~Raytracer()
{
    ReleaseDeviceAndScene();

}

int Raytracer::InitDeviceAndScene( const char * config )
{
    device_ = rtcNewDevice( config );
    error_handler( nullptr, rtcGetDeviceError( device_ ), "Unable to create a new device.\n" );
    rtcSetDeviceErrorFunction( device_, error_handler, nullptr );

    ssize_t triangle_supported = rtcGetDeviceProperty( device_, RTC_DEVICE_PROPERTY_TRIANGLE_GEOMETRY_SUPPORTED );

    // create a new scene bound to the specified device
    scene_ = rtcNewScene( device_ );

    return 0;
}

int Raytracer::ReleaseDeviceAndScene()
{
    rtcReleaseScene( scene_ );
    rtcReleaseDevice( device_ );

    return 0;
}

void Raytracer::LoadScene( const std::string file_name )
{
    const int no_surfaces = LoadOBJ( file_name.c_str(), surfaces_, materials_ );

    // surfaces loop
    for ( auto surface : surfaces_ )
    {
        RTCGeometry mesh = rtcNewGeometry( device_, RTC_GEOMETRY_TYPE_TRIANGLE );

        Vertex3f * vertices = ( Vertex3f * )rtcSetNewGeometryBuffer(
            mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
            sizeof( Vertex3f ), 3 * surface->no_triangles() );

        Triangle3ui * triangles = ( Triangle3ui * )rtcSetNewGeometryBuffer(
            mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
            sizeof( Triangle3ui ), surface->no_triangles() );

        rtcSetGeometryUserData( mesh, ( void* )( surface->get_material() ) );

        rtcSetGeometryVertexAttributeCount( mesh, 2 );

        Normal3f * normals = ( Normal3f * )rtcSetNewGeometryBuffer(
            mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3,
            sizeof( Normal3f ), 3 * surface->no_triangles() );

        Coord2f * tex_coords = ( Coord2f * )rtcSetNewGeometryBuffer(
            mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 1, RTC_FORMAT_FLOAT2,
            sizeof( Coord2f ), 3 * surface->no_triangles() );

        // triangles loop
        for ( int i = 0, k = 0; i < surface->no_triangles(); ++i )
        {
            Triangle & triangle = surface->get_triangle( i );

            // vertices loop
            for ( int j = 0; j < 3; ++j, ++k )
            {
                const Vertex & vertex = triangle.vertex( j );

                vertices[k].x = vertex.position.x;
                vertices[k].y = vertex.position.y;
                vertices[k].z = vertex.position.z;

                normals[k].x = vertex.normal.x;
                normals[k].y = vertex.normal.y;
                normals[k].z = vertex.normal.z;

                tex_coords[k].u = vertex.texture_coords[0].u;
                tex_coords[k].v = vertex.texture_coords[0].v;
            } // end of vertices loop

            triangles[i].v0 = k - 3;
            triangles[i].v1 = k - 2;
            triangles[i].v2 = k - 1;
        } // end of triangles loop

        rtcCommitGeometry( mesh );
        unsigned int geom_id = rtcAttachGeometry( scene_, mesh );
        rtcReleaseGeometry( mesh );
    } // end of surfaces loop

    rtcCommitScene( scene_ );
}

Color4f Raytracer::get_pixel( const int x, const int y, const float t )
{
    // TODO generate primary ray and perform ray cast on the scene
    // return Color4f{ 1.0f, 0.0f, 1.0f, 1.0f };

    float cicrcle_x = std::sin( t ) * 100;
    float cicrcle_y = std::cos( t ) * 100;

    float center_x = width() / 2.0f;
    float center_y = height() / 2.0f;

    float dx = x - center_x - cicrcle_x;
    float dy = y - center_y - cicrcle_y;

    float radius = 100.0f;

    if (dx * dx + dy * dy < radius * radius)
        return Color4f{ 1.0f, 0.0f, 1.0f, 1.0f };

    return Color4f{ 0.0f, 0.0f, 0.0f, 1.0f };
}

int Raytracer::Ui()
{
    static float f = 0.0f;
    static int counter = 0;

    // Use a Begin/End pair to created a named window
    ImGui::Begin( "Ray Tracer Params" );

    ImGui::Text( "Surfaces = %zd", surfaces_.size() );
    ImGui::Text( "Materials = %zd", materials_.size() );
    ImGui::Separator();
    ImGui::Checkbox( "Vsync", &vsync_ );

    //ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing our window open/close state
    //ImGui::Checkbox( "Another Window", &show_another_window );

    ImGui::SliderFloat( "float", &f, 0.0f, 1.0f ); // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3( "clear color", ( float* )&clear_color ); // Edit 3 floats representing a color

    // Buttons return true when clicked (most widgets return true when edited/activated)
    if ( ImGui::Button( "Button" ) )
        counter++;
    ImGui::SameLine();
    ImGui::Text( "counter = %d", counter );

    ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", ImGui::GetIO().DeltaTime, ImGui::GetIO().Framerate );
    ImGui::Text("Render %.3f ms/frame (%.1f FPS)", delta_time_, render_fps()) ;
    ImGui::End();

    // 3. Show another simple window.
    /*if ( show_another_window )
    {
    ImGui::Begin( "Another Window", &show_another_window ); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text( "Hello from another window!" );
    if ( ImGui::Button( "Close Me" ) )
    show_another_window = false;
    ImGui::End();
    }*/

    return 0;
}
