#include "simplegui.hpp"


#include "3rdparty/imgui/imgui_impl_sdl2.h"
#include "3rdparty/imgui/imgui_impl_opengl3.h"
#include "imgui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <mutex>
#include <thread>

SimpleGui::SimpleGui( const int width, const int height): pixelDataBuffers(width, height)
{
    width_ = width;
    height_ = height;

    Init();
}


SimpleGui::~SimpleGui()
{
    Cleanup();

}

int SimpleGui::Init()
{
    // ******* SDL ************

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window_ = SDL_CreateWindow(
        "PG1 Ray Tracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width_,
        height_,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );

    if (!window_)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }


    gl_context_ = SDL_GL_CreateContext(window_);

    if (!gl_context_)
    {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_MakeCurrent(window_, gl_context_);
    SDL_GL_SetSwapInterval(1);

    // ******* GLEW ************

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW could not be initialized!" << std::endl;
        return 1;
    }

    glViewport(0, 0, width_, height_);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // glEnable(GL_STENCIL_TEST);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // glStencilMask(0xFF);

    // ******* ImGui ************

    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext())
    {
        std::cerr << "ImGui context could not be created!" << std::endl;
        return 1;
    }

    if (!ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_))
    {
        std::cerr << "ImGui SDL2 OpenGL init failed!" << std::endl;
        return 1;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
    {
        std::cerr << "ImGui OpenGL3 init failed!" << std::endl;
        return 1;
    }

    ImGui::StyleColorsDark();

    return 0;
}

int SimpleGui::Cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_Quit();

    return 0;
}

void SimpleGui::Producer()
{
    double t = 0.0f; // time
	auto last_time = std::chrono::steady_clock::now();

    while (!producer_done_.load(std::memory_order_acquire))
    {
        auto current_time = std::chrono::steady_clock::now();
        delta_time_ = std::chrono::duration<double>(current_time - last_time).count();
        last_time = current_time;
        t += delta_time_;


        for (int y = 0; y < height_; ++y)
        {
            for (int x = 0; x < width_; ++x)
            {
                Color4f color = get_pixel(x, y, t);
                pixelDataBuffers.currentBuffer->operator[](4 * (y * width_ + x) + 0) = color.r;
                pixelDataBuffers.currentBuffer->operator[](4 * (y * width_ + x) + 1) = color.g;
                pixelDataBuffers.currentBuffer->operator[](4 * (y * width_ + x) + 2) = color.b;
                pixelDataBuffers.currentBuffer->operator[](4 * (y * width_ + x) + 3) = color.a;
            }
        }

        {
            std::scoped_lock<std::mutex> lock(pixelDataBuffers.mtx);
            std::swap(pixelDataBuffers.currentBuffer, pixelDataBuffers.readyBuffer);
            pixelDataBuffers.dataReady = true;
        }

    }
}

int SimpleGui::MainLoop()
{
    SDL_Event event;
    bool done = false;

    std::thread producer_thread( &SimpleGui::Producer, this );
    GLuint texture = create_texture(width_, height_);

    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                done = true;

            if (event.type == SDL_WINDOWEVENT_RESIZED)
                glViewport(0, 0, event.window.data1, event.window.data2);

            // process other events here
        }

        {
            std::scoped_lock<std::mutex> lock(pixelDataBuffers.mtx);
            if (pixelDataBuffers.dataReady)
            {
                update_texture(texture, width_, height_, pixelDataBuffers.readyBuffer->data());
                pixelDataBuffers.dataReady = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
        Ui();

        ImGui::SetNextWindowSize(ImVec2( float( width_ )+ 20, float( height_ ) + 20 ));
        ImGui::Begin( "Image", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar );
		ImGui::Image( ImTextureID(texture), ImVec2( float( width_ ), float( height_ ) ) );
		ImGui::End();

        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.1f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SetSwapInterval(vsync_ ? 1 : 0);

        SDL_GL_SwapWindow(window_);
    }

    producer_done_.store(true, std::memory_order_release);
    producer_thread.join();

    return 0;
}

GLuint SimpleGui::create_texture( const int width, const int height )
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,
        width,
        height,
        0,
        GL_RGBA,
        GL_FLOAT,
        nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return tex;
}

void SimpleGui::update_texture( const GLuint tex, const int width, const int height, const float* const data )
{
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        height,
        GL_RGBA,
        GL_FLOAT,
        data
    );
}

// abstract method reimplemented in the descendant
int SimpleGui::Ui()
{
    return 0;
}

Color4f SimpleGui::get_pixel( const int x, const int y, const float t )
{
    return Color4f{ 1.0f, 0.0f, 1.0f, 1.0f };
}

int SimpleGui::width() const
{
    return width_;
}

int SimpleGui::height() const
{
    return height_;
}

double SimpleGui::render_fps() const
{
    return 1.0 / delta_time_;
}
