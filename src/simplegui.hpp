#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include "structs.hpp"
#include <SDL2/SDL_video.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>



struct PixelDataBuffers {
    std::vector<float> buffer1;
    std::vector<float> buffer2;
    std::vector<float>* currentBuffer;    // Buffer being written by worker
    std::vector<float>* readyBuffer;      // Buffer ready for rendering

    std::mutex mtx;
    bool dataReady;

    PixelDataBuffers(int width, int height)
        : buffer1(width * height * 4, 0.0f),
          buffer2(width * height * 4, 0.0f),
          currentBuffer(&buffer1),
          readyBuffer(&buffer2),
          dataReady(false)
    {}
};



struct SimpleGui
{
    SimpleGui( const int width, const int height );
    ~SimpleGui();

    int MainLoop();


protected:
    int Init();
    int Cleanup();

    virtual int Ui();
    virtual Color4f get_pixel( const int x, const int y, const float t = 0.0f );

    void Producer();

    int width() const;
    int height() const;

    double render_fps() const;

    bool vsync_{ true };
    double delta_time_{ 0.0f };

private:
    int width_{ 640 };
    int height_{ 480 };

    PixelDataBuffers pixelDataBuffers;


    GLuint create_texture( const int width, const int height);
    void update_texture( const GLuint tex, const int width, const int height, const float* const data);

    std::atomic<bool> producer_done_{ false };


    SDL_Window* window_{ nullptr };
    SDL_GLContext gl_context_{ nullptr };
};
